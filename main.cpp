#include "simlcd.h"
#include "dispcolor.h"
#include "font.h"
#include "color.h"

#include "pplan.h"

simlcd_buffer_t LCD_BUFFER;

uint8_t sound_mute=0;
char Buffer[64];
uint8_t GBuf[64*128*8];


void screen_update(void)
{

}
void screen_buf_clear(void)
{
    
}
void screen_font_print(uint8_t x,uint8_t y,char * String)
{

}
void screen_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{

}

int main(int argc,char *argv[])
{
    
    pplan_config_t pplan_config={&simlcd_delay,&sound_mute,&screen_update,&screen_buf_clear\
    ,&screen_font_print,&screen_draw_rectangle,Buffer,GBuf};

    dispcolor_Init(128,64);
    dispcolor_FillScreen(color_24_to_16(90,150,90));
    dispcolor_Update();

    pplan_init(&pplan_config);

    simlcd_play();
    return 0;
}

int loop(int key)
{
    if(key==80)key='*';//Left
    else if(key==79)key='#';//Right
    else if(key==41)key='0';//Esc

    return pplan_go(key);
}

void simlcd_exit()
{

}
