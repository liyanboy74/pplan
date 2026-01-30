#ifndef _pplan_h
#define _pplan_h

#include <stdint.h>

typedef struct
{
    void (*delay_ms)(uint32_t);
    uint8_t * sound_mute;

    void (*screen_update)(void);
    void (*screen_buf_clear)(void);
    void (*screen_font_print)(uint8_t x,uint8_t y,char * String);
    void (*screen_draw_rectangle)(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    char *Buffer;
    uint8_t *GBuf;
} pplan_config_t;

#ifdef __cplusplus
extern "C" {
#endif

void pplan_init(pplan_config_t *obj);
uint8_t pplan_go(uint8_t key);

#ifdef __cplusplus
}
#endif

#endif