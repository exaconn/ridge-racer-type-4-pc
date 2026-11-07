/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define GetClut ((int (*)())GetClut)

#undef GetClut

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*)[])&D_800ABAC2)

#define GPU_printf (*(void (*(*))(const char *, s32, s32, s32))&GPU_printf)

#undef D_800ABAC2

#undef GPU_printf

/* legacy typing of shared globals in this part of the unit */
#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAB8 ((int *)D_800ABAB8)

#define DMACallback ((int (*)())DMACallback)

#undef GPU_printf

#undef D_800ABAB8

#undef DMACallback

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#define GPU_printf (*(void (*(*))())&GPU_printf)

#undef D_800ABAC2

#undef GPU_printf

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#define GPU_printf (*(void (*(*))())&GPU_printf)

#define D_800ABAB8 (*(struct { char pad[0x10]; int (*f)(); } *(*))&D_800ABAB8)

#define GPU_memset ((int (*)())GPU_memset)

#undef D_800ABAC2

#undef GPU_printf

#undef D_800ABAB8

#undef GPU_memset

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#define GPU_printf (*(M2C_UNK (*(*))(M2C_UNK *, s32))&GPU_printf)

#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABB60 (*(B (*))&D_800ABB60)

#define D_800ABAB8 ((int *)D_800ABAB8)

#undef D_800ABB60

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((RrLibGraphicsDriverPrefix *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((int *)D_800ABAB8)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

extern int GetVideoMode();

char *SetDefDrawEnv(char *p, int a, int b, int c, int d) {
    int t = GetVideoMode();
    *(short *)p = a;
    *(short *)(p + 2) = b;
    *(short *)(p + 4) = c;
    *(short *)(p + 0xC) = 0;
    *(short *)(p + 0xE) = 0;
    *(short *)(p + 0x10) = 0;
    *(short *)(p + 0x12) = 0;
    p[0x19] = 0;
    p[0x1A] = 0;
    p[0x1B] = 0;
    p[0x16] = 1;
    *(short *)(p + 6) = d;
    if (t) p[0x17] = d < 0x121; else p[0x17] = d < 0x101;
    *(short *)(p + 8) = a;
    *(short *)(p + 0xA) = b;
    *(short *)(p + 0x14) = 0xA;
    p[0x18] = 0;
    return p;
}
