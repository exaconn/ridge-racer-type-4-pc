/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu_reset.h"

#include "rr/library.h"

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

extern char D_80011E80[];

void SetDispMask(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) GPU_printf(D_80011E80, a0);
 if (a0 == 0) GPU_memset(p + 0x6A, -1, 0x14);
 D_800ABAB8->f(a0 ? 0x3000000 : 0x3000001); }
