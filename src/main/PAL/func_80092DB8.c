/* CC1_FLAGS: -mno-split-addresses -fno-schedule-insns */
#include "rr/gpu.h"
#include "rr/library.h"
extern char D_80011EA8[],D_80011EB4[],D_80011EC8[];
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
