#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/fade_sprite.h"
#include "game/effect/ptinit.h"
#include "game/sound/cdmain.h"
#include "psyq/libsnd/ssmvol.h"
#include "game/camera/campath.h"
#include "game/gfx/fadespr.h"
#include "rr/audio_state_types.h"
#include "rr/bios_devices.h"
#include "rr/library.h"
#include "psyq/libsnd/vmcfg.h"

struct V4 { int a[4]; };

typedef struct {
    short s00, s02;
    s32 unk04[3];
    short s10, s12, s14, s16;
    s32 unk18[4];
    char b28, b29, b2A, b2B;
    s32 unk2C;
    char b30, b31;
    short h32;
    int w34, w38;
} OptionsState3C;

extern OptionsState3C Options;
extern char D_800ACA30[];
extern struct V4 D_80011260;
extern unsigned char D_800A1FC4[];
extern FadeRaceContext RaceContext;
extern SpritePacket *AddSpritePrim(u32 *, SpritePacket *, s32, s32,
                                  UiUvRect *, u16, s32);

void EmitFadeSprite(s32 index, s32 x, s32 y, s32 intensity);

void InitOptionsDefaults(void) {
    int i;
    char *b;
    char *q;

    i = 0;
    b = (char *)&Options;
    q = b;
    do {
        ((OptionsState3C *)(b + i * 4))->s00 = 0;
        ((OptionsState3C *)(b + i * 4))->s02 = 0;
        ((OptionsState3C *)(q + i * 16))->s10 = 1;
        ((OptionsState3C *)(q + i * 16))->s12 = 1;
        ((OptionsState3C *)(q + i * 16))->s14 = 0;
        ((OptionsState3C *)(b + i * 4))->b28 = 1;
        ((OptionsState3C *)(b + i * 4))->b29 = 0;
        ((OptionsState3C *)(b + i * 4))->b2A = 2;
        ((OptionsState3C *)(b + i * 4))->b2B = 0;
        i++;
    } while (i < 2);
    Options.b30 = 0x10;
    Options.b31 = 0x10;
    Options.h32 = 0;
    Options.w34 = 0;
    Options.w38 = 0x10;
}

void InitDemoFadeContext(int a0) {
    struct V4 v;
    int i;
    s16 t;
    FadeRaceContext *p = (FadeRaceContext *)D_800ACA30;
    v = D_80011260;
    PsyqBzero((char *)p, 0x64);
    p = (FadeRaceContext *)((char *)p - 0x3E0);
    p->f400 = 0xCE;
    p->f42C = a0;
    p->scale3E8 = 0x100;
    p->unkB0 = 0;
    InitParticles();
    i = 0;
    if (SoundState.seqHandle2864 >= 0) {
        SsSeqStop(*(short *)&SoundState.seqHandle2864);
        SsSeqClose(*(short *)&SoundState.seqHandle2864);
        SoundState.seqHandle2864 = -1;
    }
    for (i = 0; i < 4; i++) {
        t = SoundState.bank24[v.a[i]];
        if (t >= 0) {
            SsVabClose(t);
            SoundState.bank24[v.a[i]] = -1;
        }
    }
    ResetSeqFadeState();
    MotionSequenceIndex = -1;
}

void UpdateFadeTint(int a0) {
    int v1, a, v2;
    RaceContext.r3F0 = (D_800A1FC4[0] * a0) / 255;
    RaceContext.g3F1 = (D_800A1FC4[1] * a0) / 255;
    v1 = RaceContext.r3F0 * RaceContext.scale3E8;
    RaceContext.b3F2 = (D_800A1FC4[2] * a0) / 255;
    if (v1 < 0) v1 += 0xFF;
    a = RaceContext.g3F1 * RaceContext.scale3E8;
    RaceContext.r3F0 = v1 >> 8;
    if (a < 0) a += 0xFF;
    v2 = RaceContext.b3F2 * RaceContext.scale3E8;
    RaceContext.g3F1 = a >> 8;
    if (v2 < 0) v2 += 0xFF;
    RaceContext.b3F2 = v2 >> 8;
}

void EmitFadeSprite(s32 index, s32 x, s32 y, s32 intensity) {
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
    register GpuScratchPointers *scratch __asm__("$16");
    register FrameStorage **slot;
    /* ASM_BARRIER: the input cursor dies at the helper call; its returned
     * packet starts a separate t0 lifetime after that call. */
    register GpuSingleCommandPacket *p __asm__("$8");
    register SpritePacket *cursor __asm__("$8");
    register FrameStorage *context;
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
    compared = index ^ 4U;
    captured = D_800ACA3C;
    phase = captured;
    selector = compared == 0;
    if (phase >= 128) {
        selector += 2;
        scale = (phase - 127U);
    } else {
        product = 128;
        scale = ((u32)product - phase);
    }
    scale = (s32)((u32)scale * istage) / 256;
    product = ((u32)scale * D_800ACA38);
    scale = product >> 8;
    if (product < 0) {
        scale = (s32)(product + 255U) >> 8;
    }
    scratch = (GpuScratchPointers *)SCRATCHPAD;
    if (scale > 0) {
        cursor = (SpritePacket *)scratch->cursor;
        slot = &CurrentDrawBuf;
        p = (GpuSingleCommandPacket *)AddSpritePrim(
            ({
                ordering = &(*slot)->buffer.ordering70.words[10];
                /* ASM_BARRIER: finish a0 within argument evaluation, leaving
                 * the independent cursor move for the call delay slot. */
                __asm__("" : "+r"(ordering));
                ordering;
            }),
            cursor, xstage, ystage, &D_800A1FB0[index], 0x7F38,
            ((scale * 2U) | 1U));
        command = 0xE1000200U;
        low = 0xFFFFFFU;
        modes = D_80011248;
        mode = selector * 4U;
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
        tag = (tag & high) | (context->buffer.ordering70.words[10] & low);
        p->tag = tag;
        context->buffer.ordering70.words[10] = (context->buffer.ordering70.words[10] & high) |
                          ((unsigned long)p & low);
        scratch->cursor = (u8 *)p + sizeof(*p);
        /* ASM_BARRIER: keep the returned packet live until cursor publication. */
        __asm__("" : : "r"(p));
    }
}
