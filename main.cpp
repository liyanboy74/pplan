/**
 * That file writen for Run PPlan game using SDL2
 * By Liyanboy74
 */
#include "simlcd.h"
#include "dispcolor.h"
#include "font.h"
#include "color.h"

#include "pplan.h"

#define BGCOLOR 90,150,90
#define FRCOLOR 0,0,0

simlcd_buffer_t LCD_BUFFER;

uint8_t sound_mute=0;
char Buffer[64];
uint8_t GBuf[128*8];

void screen_update(void)
{
	simlcd_set_color(&LCD_BUFFER,FRCOLOR);

    for(int i=0;i<8;i++)
    for(int j=0;j<128;j++)
    {
        for(int b=0;b<8;b++)
            if((GBuf[(i*128)+j]>>b)&1) simlcd_draw_point(&LCD_BUFFER,j,(i*8)+b);
    }
    dispcolor_Update();
    dispcolor_FillScreen(color_24_to_16(BGCOLOR));
}
void screen_buf_clear(void)
{
    memset(GBuf,0,sizeof(GBuf));
}
void screen_font_print(uint8_t x,uint8_t y,char * String)
{
    dispcolor_DrawString(x*8,y*8,0,String,0);
}
void screen_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    dispcolor_DrawRectangle(x,y,x+w,y+h,0);
}

int main(int argc,char *argv[])
{
    
    pplan_config_t pplan_config={&simlcd_delay,&sound_mute,&screen_update,&screen_buf_clear\
    ,&screen_font_print,&screen_draw_rectangle,Buffer,GBuf};

    dispcolor_Init(128,64);
    dispcolor_FillScreen(color_24_to_16(BGCOLOR));
    dispcolor_Update();

    pplan_init(&pplan_config);
    pplan_go(0);

    simlcd_play();
    return 0;
}

int loop(int key)
{
    uint8_t ret;
    static int keyo=1;

    if(key==80)keyo='*';        //Left
    else if(key==79)keyo='#';   //Right
    else if(key==41)keyo='0';   //Esc
    else if(key==-1)keyo=1;     //Release Key
    else if(key==16)            //M
    sound_mute=1;

    ret=pplan_go(keyo);
    simlcd_delay(5);

    return ret;
}

void simlcd_exit()
{

}
