/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define func_80095A24 ((int (*)())func_80095A24)

#undef func_80095A24

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*)[])&D_800ABAC2)

#define D_800ABABC (*(void (*(*))(const char *, s32, s32, s32))&D_800ABABC)

#undef D_800ABAC2

#undef D_800ABABC

/* legacy typing of shared globals in this part of the unit */
#define D_800ABABC (*(int (*(*))())&D_800ABABC)

#define D_800ABAB8 ((int *)D_800ABAB8)

#define func_8008B3C4 ((int (*)())func_8008B3C4)

#undef D_800ABABC

#undef D_800ABAB8

#undef func_8008B3C4

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#define D_800ABABC (*(void (*(*))())&D_800ABABC)

#undef D_800ABAC2

#undef D_800ABABC

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#define D_800ABABC (*(void (*(*))())&D_800ABABC)

#define D_800ABAB8 (*(struct { char pad[0x10]; int (*f)(); } *(*))&D_800ABAB8)

#define GPU_memset ((int (*)())GPU_memset)

#undef D_800ABAC2

#undef D_800ABABC

#undef D_800ABAB8

#undef GPU_memset

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#define D_800ABABC (*(M2C_UNK (*(*))(M2C_UNK *, s32))&D_800ABABC)

#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef D_800ABAB8

#undef D_800ABABC

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
#define D_800ABABC (*(int (*(*))())&D_800ABABC)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABABC

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#define D_800ABABC (*(int (*(*))())&D_800ABABC)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABAB8

#undef D_800ABABC

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((RrLibGraphicsDriverPrefix *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((int *)D_800ABAB8)

#define D_800ABABC (*(int (*(*))())&D_800ABABC)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABAB8

#undef D_800ABABC

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#define D_800ABABC (*(int (*(*))())&D_800ABABC)

#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#undef D_800ABAB8

#undef D_800ABABC

#undef D_800ABAC2

typedef struct { short a,b,c,d,e,f,g,h; char i,j,k,l; } W3L2_A;

W3L2_A *SetDefDispEnv(W3L2_A *p, int a, int b, int c, int d) {
    p->a = a; p->b = b; p->c = c;
    p->e = 0; p->f = 0; p->g = 0; p->h = 0;
    p->j = 0; p->i = 0; p->l = 0; p->k = 0;
    p->d = d;
    return p;
}
