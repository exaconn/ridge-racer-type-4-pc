#ifndef RR_TRIG_H
#define RR_TRIG_H

#include "types.h"

/* PAL quarter-wave table and overlapping symbol views used for indexed loads. */
extern s16 D_800A50C0[], D_800A58C0[], D_800A60C0[], D_800A68C0[];

/* Signed 32-bit angles use 4096 units per turn; results use 4096 as unity. */
s32 func_80090424(s32 angle);
s32 func_800904F4(s32 angle);
/* Internal nonnegative sine lookup: angle must be in 0..4095. */
s32 sin_1(s32 angle);

#endif
