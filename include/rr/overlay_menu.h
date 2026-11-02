#ifndef RR_OVERLAY_MENU_H
#define RR_OVERLAY_MENU_H
#include "rr/menu_choice.h"

extern s16 D_800AC64A,D_800AC5E0;
void func_80041320(s32 row,s32 first,s32 second);
void func_8002B330(s32 mode,s32 width,char *selection,s32 limit,s32 flags);
void func_8002B12C(s32 x,s32 y,s32 width,s32 mode,s32 selected,s32 first,s32 second);
s32 func_8003E13C(s32 selected);

s32 ovl660_func_801156A8(void);
s32 ovl661_func_80117E10(void);
s32 ovl666_update_retry_choice(s32 mode);
/* Four-position selection, backed by at least three UI rows and four glyph
 * rows. A sound callback may replace the roots; its signed selection is clamped. */
void ovl666_update_course_selection(void);
s32 ovl667_func_801187DC(void);
/* The first machine-word argument is passed by callers but unused.
 * Return the final glyph-step completion value without truncation. */
s32 ovl662_func_8011662C(u32 unused,s32 mode);
#endif
