/* RODATA 0x800103e0 0x1c */
/* CC1_FLAGS: -fno-strength-reduce */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "scratchpad.h"
#include "rr/hud_rotating_quad.h"
#include "rr/trig.h"
#include "rr/hud_number.h"
#include "game/ui/window2.h"
#include "game/ui/window1.h"
#include "rr/race_setup.h"
#include "game/ui/badge1.h"

struct HudNumberTargetPair28 { char *p0; char *p4; short s8, sA, sC, sE, s10, s12; char *p14; char *p18; short s1C, s1E, s20, s22, s24, s26; };

extern char D_800ADB60[];
extern char D_800D02D8[];
extern char D_800AE660[];
extern char D_800D0DD8[];
extern char UiRowDefaultPresets[];
extern char GlyphRowDefaultPresets[];

void DrawViewportBorderFrames();
void DrawHudRotatingQuad(s32 phase,s32 index);
void *EmitHudGlyph12(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void *EmitHudGlyph12Alt(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5);
void DrawHudNumber3(int a0, int a1);

void InitHudNumberTargets(int a0) {
    switch (a0) {
    case 0: case 1: case 5: break;
    case 2: case 3: case 4: case 6:
        if (a0 == 4 && LinkPlayerCount == 2) {
        } else if (a0 != 6) {
            goto w3g0b;
        }
    }
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->p0 = D_800ADB60;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->p4 = D_800D02D8;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s8 = 0x106;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->sA = 0xE6;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->sC = 0x122;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->sE = 0xDC;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s10 = 0x112;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s12 = 0xCE;
    return;
w3g0b:
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->p0 = D_800AE660;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->p4 = D_800D0DD8;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s8 = 0xFC;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->sA = 0x58;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->sC = 0x128;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->sE = 0x4A;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s10 = 0xFC;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s12 = 0x68;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->p14 = D_800AE660;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->p18 = D_800D0DD8;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s1C = 0xFC;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s1E = 0xD8;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s20 = 0x122;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s22 = 0xCA;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s24 = 0xFC;
    ((struct HudNumberTargetPair28 *)HudNumberTargets)->s26 = 0xE8;
}

void DrawViewportBorderFrames(int a0) {
    unsigned *s0;
    unsigned *s2;
    char *s1;
    char *s4;
    int s3, s5, c3;
    unsigned m1;
    unsigned h1;
    unsigned m2;
    unsigned h2;

    s0 = *(unsigned **)SCRATCHPAD;
    if (a0 >= 2) {
        s2 = (unsigned *)(&CurrentDrawBuf->buffer.secondaryOrderingB70.words[0]);
        s5 = 4;
        if (a0 == 3) s5 = 2;
        s3 = 0;
        if (s5 != 0) {
            m1 = 0xFFFFFF;
            h1 = 0xFF000000;
            s1 = GlyphRowDefaultPresets;
            do {
                InitHudSpritePacket(s0, s1);
                s1 += 0x14;
                s3++;
                *s0 = (*s0 & h1) | (*s2 & m1);
                *s2 = (*s2 & h1) | ((unsigned)s0 & m1);
                s0 += 5;
            } while (s3 < s5);
        }
    } else {
        s3 = 0;
        m2 = 0xFFFFFF;
        h2 = 0xFF000000;
        s4 = UiRowDefaultPresets;
        s2 = (unsigned *)(&CurrentDrawBuf->buffer.ordering70.words[0]);
        s1 = (char *)s0 + 0xA;
        do {
            InitHudSpritePacket(s0, s4);
            __asm__ volatile("" : "=r"(c3) : "0"(3)); /* ASM_BARRIER: keep li 3 inside the loop (not hoisted) */
            if (a0 == c3) *(unsigned short *)s1 -= 0x80;
            s1 += 0x14;
            s4 += 0x14;
            s3++;
            *s0 = (*s0 & h2) | (*s2 & m2);
            *s2 = (*s2 & h2) | ((unsigned)s0 & m2);
            s0 += 5;
        } while (s3 < 3);
    }
    *(void **)SCRATCHPAD = AddDrawModePrim(s2, s0, 5);
}

void DrawHudRotatingQuad(s32 phase,s32 index) {
    HudQuadColor colors[4];
    u32 sine;
    s32 angle;
    u32 cosine;
    s32 i;
    HudQuadColor *color;
    HudQuadCorner *corner;
    NumberTarget *target;
    u32 *head;
    HudQuadEmission *packet;
    u8 *output;
    NumberTarget (*base)[];
    unsigned long row;
    u32 x;
    u32 y;
    colors[0].rgb.r=0xD0;
    colors[0].rgb.g=24;
    colors[0].rgb.b=0x30;
    colors[1].rgb.r=0x68;
    colors[1].rgb.g=12;
    colors[1].rgb.b=24;
    colors[2].rgb.r=0x8A;
    colors[2].rgb.g=0x12;
    colors[2].rgb.b=0x24;
    colors[3].rgb.r=0x34;
    colors[3].rgb.g=6;
    colors[3].rgb.b=12;
    angle=(s32)(phase*0xA00U)/10000+0xE00;
    sine=rsin(angle);
    cosine=rcos(angle);
    i=0;color=colors;corner=UiBoxCornerOffsets;
    base=&HudNumberTargets;
    row=index*sizeof(NumberTarget);
    target=(NumberTarget *)((u8 *)base+row);
    packet=(HudQuadEmission *)RR_HUD_QUAD_SCRATCH->cursor;
    x=(u16)target->x;y=(u16)target->y;
    /* Full native-width addresses retain the original sum order. */
    head=*(u32 **)(DrawBufIndex*sizeof(u32 *)+row+(unsigned long)base);
    output=(u8 *)packet+8;
    packet->r=0xB0;packet->g=0xB0;packet->b=0;
    do {
        ((s16 *)output)[0]=(s16)(x+((s32)(cosine*corner->x-sine*corner->y)/4096));
        ((s16 *)output)[1]=(s16)(y+((s32)(sine*corner->x+cosine*corner->y)/4096));
        output+=4;
        /* Keep the initialized color destination after the XY writes. */
        *(HudQuadColor *)output=*color;
        color++;corner++;i++;output+=4;
    }while(i<4);
    packet->tag.bytes.length=8;packet->code=0x38;
    packet->tag.bits.address=((GpuDmaTag *)head)->bits.address;
    ((GpuDmaTag *)head)->bits.address=(unsigned long)packet;
    packet=(HudQuadEmission *)((u8 *)packet+36);
    RR_HUD_QUAD_SCRATCH->cursor=(u8 *)packet;
}

void *EmitHudGlyph12(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    ((s8 *)arg1)[3] = 4;
    ((s8 *)arg1)[7] = 0x65;
    ((s16 *)arg1)[8] = 0xC;
    ((s16 *)arg1)[9] = 0x10;
    ((s16 *)arg1)[4] = arg2;
    ((s16 *)arg1)[5] = arg3;
    ((u8 *)arg1)[13] = 0xF0;
    ((s8 *)arg1)[12] = (s8) (arg4 * 0xC);
    ((s16 *)arg1)[7] = (s16) arg5;
    ((s32 *)arg1)[0] = ((((s32 *)arg1)[0] & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

void *EmitHudGlyph12Alt(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5) {
    ((s8 *)arg1)[3] = 4;
    ((s8 *)arg1)[7] = 0x65;
    ((s16 *)arg1)[8] = 0xC;
    ((s16 *)arg1)[9] = 0x10;
    ((s16 *)arg1)[4] = arg2;
    ((s16 *)arg1)[5] = arg3;
    ((u8 *)arg1)[13] = 0xF0;
    ((s8 *)arg1)[12] = (s8) ((arg4 * 0xC) + 0x78);
    ((s16 *)arg1)[7] = (s16) arg5;
    ((s32 *)arg1)[0] = ((((s32 *)arg1)[0] & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

void DrawHudNumber3(int a0, int a1)
{
    char *b = (char *)HudNumberTargets;
    int off = a1 * 0x14;
    char *t = b + off;
    short x = ((short *)t)[4];
    short y = ((short *)t)[5];
    u32 *s1 = *(u32 **)(DrawBufIndex * 4 + off + b);
    int h, tn, on;
    h = a0 / 100;
    tn = (a0 / 10) - h * 10;
    on = a0 % 10;
    *(void **)SCRATCHPAD = AddDrawModePrim(s1, EmitHudGlyph12(s1, EmitHudGlyph12(s1, EmitHudGlyph12(s1, *(void **)SCRATCHPAD, x, y, h, 0x7EFA), x + 8, y, tn, 0x7EFA), x + 0x10, y, on, 0x7EFA), 5);
}
