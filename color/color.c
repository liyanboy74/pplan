/*
 By Liyanboy74
 https://github.com/liyanboy74
*/

#include <stdio.h>
#include <math.h>

#include "color.h"

color_rgb_s color_rgba_to_rgb_ss(color_rgb_s background,color_rgba_s color)
{
    color_rgb_s c;

    float falpha=color.a/(float)255;

    c.b=(1-falpha)*background.b+falpha*color.b;
    c.g=(1-falpha)*background.g+falpha*color.g;
    c.r=(1-falpha)*background.r+falpha*color.r;

    return c;
}

uint16_t color_24_to_16_s(color_rgb_s color)
{
    uint16_t cc=0;

    cc|=(((int)round((color.b*(float)0.1215686)))&0x1f)<<0 ;
    cc|=(((int)round((color.g*(float)0.2470588)))&0x3f)<<5 ;
    cc|=(((int)round((color.r*(float)0.1215686)))&0x1f)<<11;

    return cc;
}

uint16_t color_24_to_16(uint8_t r,uint8_t g,uint8_t b)
{
    color_rgb888_s c;

    c.b=b;
    c.g=g;
    c.r=r;

    return color_24_to_16_s(c);
}

color_rgb_s color_16_to_24_su(color_rgb565_u color)
{
    color_rgb888_s c;

    c.b=color.rgb.b;
    c.g=color.rgb.g;
    c.r=color.rgb.r;

    c.b=(int)round(8.2258*c.b);
    c.g=(int)round(4.0476*c.g);
    c.r=(int)round(8.2258*c.r);

    return c;
}

color_rgb_s color_16_to_24_s(uint16_t color)
{
    color_rgb565_u c;

    c.word=color;

    return color_16_to_24_su(c);
}

color_rgb_s color_s(uint8_t r,uint8_t g,uint8_t b)
{
    color_rgb_s c;

    c.b=b;
    c.g=g;
    c.r=r;

    return c;
}
