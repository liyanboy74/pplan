#include <stdio.h>
#include <stdlib.h>
#include "pplan.h"

#ifdef _WIN32
#include <utilapiset.h>
#else
#include "main.h"
#endif

#define PPLAN_SHEEP_FIRST_POSITION 8
#define PPLAN_SCORE_HART_MINOR 100

enum pplan_type{
PPLAN_TYPE_NULL=0,
PPLAN_TYPE_BOMB_MAJOR,
PPLAN_TYPE_SHEEP,
PPLAN_TYPE_HART_MINOR,
PPLAN_TYPE_BOMB_MINOR,
};

enum pplan_message{
PPLAN_MESSAGE_START=0,
PPLAN_MESSAGE_END,
PPLAN_MESSAGE_EXIT
};

enum pplan_sound{
PPLAN_SOUND_START=0,
PPLAN_SOUND_END,
PPLAN_SOUND_HART,
PPLAN_SOUND_BOMB
};

static long int score=0;
static uint8_t pplan[8][16];
static uint8_t pplan_index;
const uint8_t pplan_icon[][8]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},                                                  //0: Null
{0b10000001,0b01000010,0b00011000,0b00111100,0b00111100,0b00011000,0b01000010,0b10000001},  //1: Bomb
{0b00111100,0b00010010,0b01111001,0b01001101,0b01001101,0b01111001,0b00010010,0b00111100},  //2: Sheep
{0b00011100,0b00100010,0b01000010,0b10001100,0b10001100,0b01000010,0b00100010,0b00011100},  //3: Hart
{0b10000001,0b01000010,0b00011000,0b00100100,0b00100100,0b00011000,0b01000010,0b10000001},  //4: BombMinor
{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};

static pplan_config_t *pplanc;

void pplan_init(pplan_config_t *obj)
{
  pplanc=obj;
}

void pplan_play_sound(enum pplan_sound id)
{
  if(*pplanc->sound_mute)return;
  #ifdef _WIN32
  switch(id)
  {
    case PPLAN_SOUND_START:
    case PPLAN_SOUND_END:
    case PPLAN_SOUND_HART:
    case PPLAN_SOUND_BOMB: Beep(1000,100);break;
  }
  #else
  if(id==PPLAN_SOUND_START)
  {
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    pplanc->delay_ms(50);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
  }
  else if(id==PPLAN_SOUND_END)
  {
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    pplanc->delay_ms(50);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
    pplanc->delay_ms(50);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    pplanc->delay_ms(200);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
  }
  else if(id==PPLAN_SOUND_HART || id==PPLAN_SOUND_BOMB)
  {
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    pplanc->delay_ms(5);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
  }
  #endif
}

void pplan_show_message_box(enum pplan_message msgid)
{
  pplanc->screen_buf_clear();
  pplanc->screen_draw_rectangle(20,20,127-(20*2),15);
  switch(msgid)
  {
    case PPLAN_MESSAGE_START  :sprintf(pplanc->Buffer," PPlan ");   break;
    case PPLAN_MESSAGE_END    :sprintf(pplanc->Buffer,"Game Over"); break;
    case PPLAN_MESSAGE_EXIT   :sprintf(pplanc->Buffer,"Goodbye!");  break;
  }
  pplanc->screen_font_print(5,3,pplanc->Buffer);

  if(msgid==PPLAN_MESSAGE_END)
  {
    sprintf(pplanc->Buffer," %05ld ",score);
    pplanc->screen_font_print(5,5,pplanc->Buffer);
  }
  pplanc->screen_update();
}

void pplan_print()
{
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<16;j++)
    {
      for(int k=0;k<8;k++)pplanc->GBuf[((i*128)+(j*8))+k]=pplan_icon[pplan[i][j]][k];
    }
  }
}

void pplan_clear()
{
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<16;j++)
    {
      pplan[i][j]=PPLAN_TYPE_NULL;
    }
  }
}

uint8_t pplan_move()
{
  for(int i=6;i>=0;i--)
  {
    for(int j=0;j<16;j++)
    {
      if(pplan[i+1][j]==PPLAN_TYPE_SHEEP && pplan[i][j]==PPLAN_TYPE_BOMB_MAJOR)return 1;
      if(pplan[i+1][j]==PPLAN_TYPE_SHEEP && pplan[i][j]==PPLAN_TYPE_BOMB_MINOR)
      {
        if(score>PPLAN_SCORE_HART_MINOR)
        {
          score-=PPLAN_SCORE_HART_MINOR;
          pplan_play_sound(PPLAN_SOUND_BOMB);
        }
        else return 1;
      }
      if(pplan[i+1][j]==PPLAN_TYPE_SHEEP && pplan[i][j]==PPLAN_TYPE_HART_MINOR)
      {
        score+=PPLAN_SCORE_HART_MINOR;
        pplan_play_sound(PPLAN_SOUND_HART);
      }
      if(pplan[i+1][j]!=PPLAN_TYPE_SHEEP)pplan[i+1][j]=pplan[i][j];
    }
  }

  for(int j=0;j<16;j++)
  {
    if(rand()%50==0)pplan[0][j]=PPLAN_TYPE_BOMB_MINOR;
    else if(rand()%100==0)pplan[0][j]=PPLAN_TYPE_HART_MINOR;
    else if(rand()%200==0)pplan[0][j]=PPLAN_TYPE_BOMB_MAJOR;
    else pplan[0][j]=PPLAN_TYPE_NULL;
  }

  return 0;
}

uint8_t pplan_sheep_move(int index)
{
  static uint8_t old_pplan_index=PPLAN_SHEEP_FIRST_POSITION;
  if(index>0){if(pplan_index<15)pplan_index++;}
  else if(index<0){if(pplan_index>0)pplan_index--;}
  

  if(pplan[7][pplan_index]==PPLAN_TYPE_BOMB_MAJOR)return 1;
  else
  {
    if(pplan[7][pplan_index]==PPLAN_TYPE_BOMB_MINOR)
    {
      if(score>PPLAN_SCORE_HART_MINOR)
      {
        score-=PPLAN_SCORE_HART_MINOR;
        pplan_play_sound(PPLAN_SOUND_BOMB);
      }
      else return 1;
    }
    else if(pplan[7][pplan_index]==PPLAN_TYPE_HART_MINOR)
    {
      score+=PPLAN_SCORE_HART_MINOR;
      pplan_play_sound(PPLAN_SOUND_HART);
    }
    pplan[7][pplan_index]=PPLAN_TYPE_SHEEP;
    if(pplan_index!=old_pplan_index)pplan[7][old_pplan_index]=PPLAN_TYPE_NULL;
  }

  old_pplan_index=pplan_index;
  return 0;
}

void pplan_print_score()
{
  sprintf(pplanc->Buffer,"%05ld",score);
  pplanc->screen_font_print(0,0,pplanc->Buffer);
}

/**
 * Run Game PPlan
 * Key:
 *  0 = new game
 * '0'= Exit
 * '*'= Left
 * '#'= Right
 */
uint8_t pplan_go(uint8_t key)
{
  static int gdata=0;
  static int gkdata=0;
  uint8_t ret=0;

  //init
  if(key==0)
  {
    pplan_index=PPLAN_SHEEP_FIRST_POSITION;
    gdata=0;
    score=0;

    pplan_show_message_box(PPLAN_MESSAGE_START);
    pplan_play_sound(PPLAN_SOUND_START);
    pplanc->delay_ms(500);
    pplan_clear();
    pplan_sheep_move(0);
    pplanc->screen_buf_clear();
    pplan_print();
    pplan_print_score();
    pplanc->screen_update();  
  }

  if(gkdata)gkdata--;
  if(gkdata==0)
  {
    if( key=='*' || key=='#')
    {
      if(key=='*')ret+=pplan_sheep_move(-1);
      else if(key=='#')ret+=pplan_sheep_move(+1);
      pplan_print();
      pplan_print_score();
      pplanc->screen_update();
      gkdata=16;
    }
  }

  if(gdata%40==0)
  {
    ret+=pplan_move();
    pplan_print();
    score++;
    pplan_print_score();
    pplanc->screen_update();
  }


  if(ret!=0)
  {
    pplan_show_message_box(PPLAN_MESSAGE_END);
    pplanc->delay_ms(100);
    pplan_play_sound(PPLAN_SOUND_END);
    pplanc->delay_ms(3000);

  }

  if(key=='0')
  {
    pplan_show_message_box(PPLAN_MESSAGE_EXIT);
    pplanc->delay_ms(1000);
    ret+=1;
  }

  gdata++;
  return ret;
}
