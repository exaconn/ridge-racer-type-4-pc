/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu_reset.h"

#include "rr/library.h"

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

#define func_800959B8 ((int (*)())func_800959B8)

#undef D_800ABAC2

#undef D_800ABABC

#undef D_800ABAB8

#undef func_800959B8

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

extern char D_80011E80[];

void func_80092CB8(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) D_800ABABC(D_80011E80, a0);
 if (a0 == 0) func_800959B8(p + 0x6A, -1, 0x14);
 D_800ABAB8->f(a0 ? 0x3000000 : 0x3000001); }
