#ifndef RR_MENU_SELECTION_H
#define RR_MENU_SELECTION_H
#include "rr/layout.h"
extern u8 D_8009F64C[8];
extern s16 D_800AD522,D_800AC648,D_800F3914;
extern s32 D_800AC5D4;
/* Word result0..255: the original loads the byte with zero extension. */
s32 func_80032E9C(s32 selection);
typedef struct {s16 first,second;} RrMenuGridPoint;
extern RrMenuGridPoint D_800ACBD0[4];
RR_ASSERT_SIZE(RrMenuGridPoint,4);
RR_ASSERT_OFFSET(RrMenuGridPoint,second,2);
#endif
