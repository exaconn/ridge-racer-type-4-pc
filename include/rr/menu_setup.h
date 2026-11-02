#ifndef RR_MENU_SETUP_H
#define RR_MENU_SETUP_H
#include "rr/menu_choice.h"
extern s32 D_800F2DE4,D_800ABEB0;
extern s16 D_800AD520;
/* Clear the resident32-byte and12-byte UI work buffers. */
void func_8002A604(void);
void func_8003E040(s32 index,s32 control,s32 glyph,s32 style);
void func_8003DC9C(s32 index,s16 a,s16 b,s16 c,s16 d,s16 e,s16 f);
void func_8003D228(s32 index,s32 first,s16 x,s32 y,u16 width,u16 mode,u16 flags);
#endif
