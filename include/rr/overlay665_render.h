#ifndef RR_OVERLAY665_RENDER_H
#define RR_OVERLAY665_RENDER_H
#include "rr/overlay665_visual.h"
#include "rr/renderer_setup.h"

#include "rr/overlay665_render_types.h"

extern s32 D_800F2FD8;
void func_8006E1AC(void);
void func_8006EF8C(s32 mode);
void func_800912B4(s32 x,s32 y);
void func_8002E828(s32 scale,void *matrix);
void func_8005599C(char *view,char *lighting,
    RrMatrixWords32 *first,RrMatrixWords32 *second);
u32 *func_80090F74(u32 *matrix,s32 scale[3]);
void func_800912D4(s32 depth);
void func_800701C4(s32 object);
void func_8006EE3C(s16 *output,s16 *view,s32 *matrix);
void func_8006E030(s16 *matrix,s32 angle);
void func_8006E108(void *matrix,s32 angle);
void func_80058768(RrPreviewRenderScratch *scratch,s32 object,s32 blend,s32 depth);
void ovl665_func_8011643C(void);
#endif
