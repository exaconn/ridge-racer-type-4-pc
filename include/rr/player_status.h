#ifndef RR_PLAYER_STATUS_H
#define RR_PLAYER_STATUS_H
#include "rr/layout.h"

extern s8 D_800ABEBC[2][2];
extern s32 D_800AC5A4;
extern u16 D_800AD508;
extern u8 D_80010600[12],D_8001060C[28],D_80010628[20];
extern u8 D_8001063C[16],D_8001064C[12],D_80010658[28],D_80010674[4];
void func_8001F40C(s32 x,s32 y,u8 *text);
/* Negative modes draw nothing; active modes0/1 select the two status pairs.
 * The renderer reads a terminated string and may change later global state. */
void func_80035478(s32 mode);
typedef char rr_player_status_native_address_width[
    sizeof(unsigned long)>=sizeof(void*)?1:-1];
#endif
