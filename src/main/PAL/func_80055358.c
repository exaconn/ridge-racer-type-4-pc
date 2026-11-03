#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 w, h;
} Sprt;

typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u32 code[1];
} DrTPage;

typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u32 code[2];
} DrStp;

typedef struct {
    s16 x, y, w, h;
} Rect;

typedef struct {
    char pad[0x70];
    DrTPage ot;
} Db;

extern Db *D_800ACC14;
extern Rect D_800A0D04[];
extern Rect D_800A0D24[][4];

extern void func_80093BFC(void *, s32);

#define addPrim(ot, p) \
    ((DrTPage *)(p))->addr = (ot)->addr; \
    (ot)->addr = (u32)(p)

void func_80055358(s32 a0, s32 a1, u8 *a2, s32 a3, s32 a4) {
    s32 i;
    Rect *uv;
    Sprt *s2;
    DrStp *s0;
    s16 y, x;
    /* ASM_BARRIER: Keep 0x1F800000 in $s1 inside loop without hoisting */
    register DrStp **p1f80 asm("$17");
    s32 base;
    s32 c;
    s32 offset;

    base = (s32)D_800A0D24;
    for (i = 0; i < 4; i++) {
        s2 = *(Sprt **)0x1F800000;
        s2->len = 4;
        c = 0x64;
        /* ASM_BARRIER: Keep 0x64 after s2->len = 4 */
        __asm__ volatile("" : "+r"(c));
        s2->code = c;
        /* ASM_BARRIER: Clobber c before if(a4 != 0) so cse1/cse2 keep c = 0x64 in else-branch */
        __asm__ volatile("" : "+r"(c));
        if (a4 != 0) {
            c = 0x66;
            s2->code = c;
        } else {
            c = 0x64;
            s2->code = c;
        }
        if (a1 != 0) {
            s2->code |= 1;
        } else {
            s2->code &= ~1;
        }
        s2->r0 = a2[0];
        s2->g0 = a2[1];
        s2->b0 = a2[2];
        s2->x0 = D_800A0D04[i].x;
        s2->y0 = D_800A0D04[i].y;
        s2->w = D_800A0D04[i].w;
        s2->h = D_800A0D04[i].h;
        offset = (i << 3) + (a0 << 5);
        uv = (Rect *)(offset + base);
        s2->u0 = uv->x;
        s2->v0 = uv->y;
        addPrim(&D_800ACC14->ot, s2);
        s2++;
        s0 = (DrStp *)((DrTPage *)s2 + 1);
        ((DrTPage *)s2)->len = 1;
        y = uv->h;
        x = uv->w;
        ((DrTPage *)s2)->code[0] = 0xE1000200 | (((a3 & 3) << 5) | 0x100 | ((s16)((y & 0x100) >> 4)) | ((x & 0x3FF) >> 6) | ((s16)(y & 0x200) << 2));
        addPrim(&D_800ACC14->ot, s2);
        p1f80 = (DrStp **)0x1F800000;
        *p1f80 = s0;
        func_80093BFC(s0, 0);
        addPrim(&D_800ACC14->ot, s0);
        *p1f80 = s0 + 1;
        /* ASM_BARRIER: Keep p1f80 multi-set during loop_optimize; deleted as dead by flow */
        __asm__("" : "=r"(p1f80));
    }
}
