/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu_reset.h"

#include "rr/gpu.h"

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

extern char D_80011EB4[];

extern char D_80011EC8[];

void func_80092DB8(const char *name,RrGpuRectangle *rectangle) {
    const char *label=name;
    RrGpuRectangle *rect=rectangle;
    RrGpuDebugSettings *settings=&D_800ABAC2;
    const char *format;
    switch(settings->level) {
    case 1: {
        s32 w=rect->rect.w;
        s32 maxw=settings->width;
        s32 x;
        s32 y;
        /* ASM_BARRIER: Preserve the original height temporary through the final validity tests. */
        register s32 h __asm__("$6");
        s32 maxh;
        s32 comparison;
        if(maxw<w) goto invalid;
        x=rect->rect.x;
        comparison=maxw<w+x;
        if(comparison) goto invalid;
        y=rect->rect.y;
        maxh=settings->height;
        if(maxh<y) goto invalid;
        h=rect->rect.h;
        comparison=maxh<y+h;
        if(comparison) goto invalid;
        if(w<=0) goto invalid;
        if(x<0) goto invalid;
        if(y<0) goto invalid;
        if(h>0) return;
    }
invalid:
        format=(const char *)0x80010000U;
        /* ASM_BARRIER: Expose the mapped invalid-message address for the jump delay slot. */
        __asm__("" : "+r"(format));
        format+=0x1EA8;
        goto report;
    case 2:
        format=D_80011EC8;
report:
        D_800ABABC(format,label);
        {
            s32 x=rect->rect.x;
            s32 y=rect->rect.y;
            s32 w=rect->rect.w;
            s32 h=rect->rect.h;
            /* ASM_BARRIER: Reload all rectangle fields before fetching the second diagnostic callback. */
            __asm__("" : : "r"(x),"r"(y),"r"(w),"r"(h) : "$2");
            D_800ABABC(D_80011EB4,x,y,w,h);
        }
    }
}
