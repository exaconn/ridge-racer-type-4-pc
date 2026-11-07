/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/draw_environment.h"

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

/* legacy typing of shared globals in this part of the unit */
#define func_80096444 ((int (*)())func_80096444)

#undef func_80096444

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define GPU_STATUS ((s32 *)GPU_STATUS)

#undef GPU_STATUS

/* legacy typing of shared globals in this part of the unit */
#define GPU_STATUS ((u32 *)GPU_STATUS)

#undef GPU_STATUS

/* legacy typing of shared globals in this part of the unit */
#define GPU_DATA ((int *)GPU_DATA)

#define GPU_STATUS ((int *)GPU_STATUS)

#undef GPU_DATA

#undef GPU_STATUS

unsigned get_ofs(int a, int b) { return 0xE5000000 | (b & 0x7FF) << 11 | (a & 0x7FF); }
