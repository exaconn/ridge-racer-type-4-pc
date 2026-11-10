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

#define GpuFuncTable ((int *)GpuFuncTable)

#define DMACallback ((int (*)())DMACallback)

#undef GPU_printf

#undef GpuFuncTable

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

#define GpuFuncTable (*(struct { char pad[0x10]; int (*f)(); } *(*))&GpuFuncTable)

#define GPU_memset ((int (*)())GPU_memset)

#undef D_800ABAC2

#undef GPU_printf

#undef GpuFuncTable

#undef GPU_memset

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((void *)GpuFuncTable)

#define GPU_printf (*(M2C_UNK (*(*))(M2C_UNK *, s32))&GPU_printf)

#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef GpuFuncTable

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((char *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((char *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((void *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((char *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GpuMovePayload (*(B (*))&GpuMovePayload)

#define GpuFuncTable ((int *)GpuFuncTable)

#undef GpuMovePayload

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((char *)GpuFuncTable)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef GpuFuncTable

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((char *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((LibGraphicsDriverPrefix *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((int *)GpuFuncTable)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef GpuFuncTable

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((char *)GpuFuncTable)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#undef GpuFuncTable

#undef GPU_printf

#undef D_800ABAC2

typedef struct { short a,b,c,d,e,f,g,h; char i,j,k,l; } W3L2_A;

W3L2_A *SetDefDispEnv(W3L2_A *p, int a, int b, int c, int d) {
    p->a = a; p->b = b; p->c = c;
    p->e = 0; p->f = 0; p->g = 0; p->h = 0;
    p->j = 0; p->i = 0; p->l = 0; p->k = 0;
    p->d = d;
    return p;
}
