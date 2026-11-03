#include "rr/fade_sprite.h"
#include "game/effect/ptinit.h"
#include "game/sound/cdmain.h"
#include "psyq/libsnd/ssmvol.h"

struct V4 { int a[4]; };

extern char D_800AD528[];
extern int func_80096434();
extern void func_8007F910(int);
extern int func_8008A674(t);
extern char D_800ACA30[];
extern int D_800F7620;
extern struct V4 D_80011260;
extern struct { char pad[0x24]; short s[4]; char pad2[0x2864 - 0x2C]; int i; } D_800F4DB8;
extern unsigned char D_800A1FC4[];
extern unsigned char D_800AC650[];
extern RrSpritePacket *func_8001FF84(u32 *, RrSpritePacket *, s32, s32,
                                  RrUiUvRect *, u16, s32);

void func_8005C0C4(void);
void func_8005C13C(int a0);
void func_8005C26C(int a0);
void func_8005C37C(s32 index, s32 x, s32 y, s32 intensity);

void func_8005C0C4(void) {
    int i;
    char *b;
    char *q;

    i = 0;
    b = D_800AD528;
    q = b;
    do {
        *(short *)(b + i * 4) = 0;
        *(short *)(b + i * 4 + 2) = 0;
        *(short *)(q + i * 16 + 0x10) = 1;
        *(short *)(q + i * 16 + 0x12) = 1;
        *(short *)(q + i * 16 + 0x14) = 0;
        b[i * 4 + 0x28] = 1;
        b[i * 4 + 0x29] = 0;
        b[i * 4 + 0x2A] = 2;
        b[i * 4 + 0x2B] = 0;
        i++;
    } while (i < 2);
    D_800AD528[0x30] = 0x10;
    D_800AD528[0x31] = 0x10;
    *((short *) (D_800AD528 + 0x32)) = 0;
    *((int *) (D_800AD528 + 0x34)) = 0;
    *((int *) (D_800AD528 + 0x38)) = 0x10;
}

void func_8005C13C(int a0) {
    struct V4 v;
    int i, t;
    char *p = D_800ACA30;
    v = D_80011260;
    func_80096434(p, 0x64);
    p -= 0x3E0;
    *(int *)(p + 0x400) = 0xCE;
    *(int *)(p + 0x42C) = a0;
    *(int *)(p + 0x3E8) = 0x100;
    *(int *)(p + 0xB0) = 0;
    func_80053E2C();
    i = 0;
    if (D_800F4DB8.i >= 0) {
        func_80083C74(*(short *)&D_800F4DB8.i);
        func_8007F910(*(short *)&D_800F4DB8.i);
        D_800F4DB8.i = -1;
    }
    for (i = 0; i < 4; i++) {
        t = D_800F4DB8.s[v.a[i]];
        if (t >= 0) {
            func_8008A674(t);
            D_800F4DB8.s[v.a[i]] = -1;
        }
    }
    func_80053A80();
    D_800F7620 = -1;
}

void func_8005C26C(int a0) {
    int v1, a, v2;
    D_800AC650[0x3F0] = (D_800A1FC4[0] * a0) / 255;
    D_800AC650[0x3F1] = (D_800A1FC4[1] * a0) / 255;
    v1 = D_800AC650[0x3F0] * *(int *)(D_800AC650 + 0x3E8);
    D_800AC650[0x3F2] = (D_800A1FC4[2] * a0) / 255;
    if (v1 < 0) v1 += 0xFF;
    a = D_800AC650[0x3F1] * *(int *)(D_800AC650 + 0x3E8);
    D_800AC650[0x3F0] = v1 >> 8;
    if (a < 0) a += 0xFF;
    v2 = D_800AC650[0x3F2] * *(int *)(D_800AC650 + 0x3E8);
    D_800AC650[0x3F1] = a >> 8;
    if (v2 < 0) v2 += 0xFF;
    D_800AC650[0x3F2] = v2 >> 8;
}

void func_8005C37C(s32 index, s32 x, s32 y, s32 intensity) {
    register s32 xstage = x;
    register s32 ystage = y;
    register s32 istage = intensity;
    register s32 phase;
    /* ASM_BARRIER: capture the phase in v1, then end its hard binding before
     * the arithmetic temporaries reuse this register. */
    register s32 captured __asm__("$3");
    register s32 selector;
    register s32 scale;
    /* ASM_BARRIER: keep the scratchpad pointer in s0 through the sprite helper. */
    register RrGpuScratchPointers *scratch __asm__("$16");
    register RrFadeSpriteContext **slot;
    /* ASM_BARRIER: the input cursor dies at the helper call; its returned
     * packet starts a separate t0 lifetime after that call. */
    register RrGpuSingleCommandPacket *p __asm__("$8");
    register RrSpritePacket *cursor __asm__("$8");
    register RrFadeSpriteContext *context;
    register u32 command;
    /* ASM_BARRIER: use a1 for the low-address mask only after the helper. */
    register u32 low __asm__("$5");
    register u32 high;
    register u32 *modes;
    register u32 mode;
    register u32 length;
    register u32 tag;
    register s32 product;
    /* ASM_BARRIER: prepare the outgoing head address in a0. */
    register u32 *ordering __asm__("$4");
    register u32 compared;

    /* ASM_BARRIER: preserve coordinates/intensity before phase capture. */
    __asm__("" : "+r"(xstage), "+r"(ystage), "+r"(istage));
    compared = (u32)index ^ 4U;
    captured = D_800ACA3C;
    phase = captured;
    selector = compared == 0;
    if (phase >= 128) {
        selector += 2;
        scale = (s32)((u32)phase - 127U);
    } else {
        product = 128;
        scale = (s32)((u32)product - (u32)phase);
    }
    scale = (s32)((u32)scale * (u32)istage) / 256;
    product = (s32)((u32)scale * (u32)D_800ACA38);
    scale = product >> 8;
    if (product < 0) {
        scale = (s32)((u32)product + 255U) >> 8;
    }
    scratch = (RrGpuScratchPointers *)0x1F800000U;
    if (scale > 0) {
        cursor = (RrSpritePacket *)scratch->cursor;
        slot = &D_800ACC14;
        p = (RrGpuSingleCommandPacket *)func_8001FF84(
            ({
                ordering = &(*slot)->head98;
                /* ASM_BARRIER: finish a0 within argument evaluation, leaving
                 * the independent cursor move for the call delay slot. */
                __asm__("" : "+r"(ordering));
                ordering;
            }),
            cursor, xstage, ystage, &D_800A1FB0[index], 0x7F38,
            (s32)(((u32)scale << 1) | 1U));
        command = 0xE1000200U;
        low = 0xFFFFFFU;
        modes = D_80011248;
        mode = (u32)selector * 4U;
        mode = *(u32 *)((u8 *)modes + mode);
        /* ASM_BARRIER: capture the mode word before publishing the packet. */
        __asm__("" : "+r"(mode));
        length = 1;
        scratch->cursor = (u8 *)p;
        ((u8 *)p)[3] = length;
        tag = p->tag;
        /* ASM_BARRIER: capture the tag before storing the command word. */
        __asm__("" : "+r"(tag), "+r"(mode));
        mode &= 0x9FFU;
        mode |= command;
        context = *slot;
        high = 0xFF000000U;
        p->command = mode;
        tag = (tag & high) | (context->head98 & low);
        p->tag = tag;
        context->head98 = (context->head98 & high) |
                          ((u32)(unsigned long)p & low);
        scratch->cursor = (u8 *)p + sizeof(*p);
        /* ASM_BARRIER: keep the returned packet live until cursor publication. */
        __asm__("" : : "r"(p));
    }
}
