#include "rr/gpu.h"

#include "rr/animated_glyph.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDrawMode ((s32 (*)())SetDrawMode)

#define AddPrim ((s32 (*)())AddPrim)

#define SetSprt ((s32 (*)())SetSprt)

#define D_800ACC14 ((char *)D_800ACC14)

#undef SetDrawMode

#undef AddPrim

#undef SetSprt

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define AddPrim ((void (*)())AddPrim)

#define SetSemiTrans ((void (*)())SetSemiTrans)

#define SetShadeTex ((void (*)())SetShadeTex)

#define SetSprt ((void (*)())SetSprt)

#undef AddPrim

#undef SetSemiTrans

#undef SetShadeTex

#undef SetSprt

void func_8001F808(s32 x, s32 y, s32 glyph, s32 clut, s16 *animation) {
    s16 nextTimer;
    s32 tileU,shadowBrightness;
    s32 drawV;
    s32 yStep;
    s32 row;
    s32 curve;
    s16 displacement;
    s32 pass;
    s32 baseV;
    s16 tileV;
#ifdef __mips__
    register RrSpritePacketSignedClut *sprites __asm__("$16");
#else
    RrSpritePacketSignedClut *sprites;
#endif
    u8 *cursor;
    RrSpritePacketSignedClut *linkPacket;

    cursor = RR_GLYPH_ANIMATION_SCRATCH->cursor;
    row = glyph/32;
    tileU=(glyph-row*32)*8;
    baseV = row * 0x18;
    tileV = baseV;
    if (glyph >= 0x140) {
        tileV = baseV + 0x10;
    }
    tileV=RR_GLYPH_PREF(tileV,"$22");
    displacement = 0;
    pass = 0;
    shadowBrightness = RR_GLYPH_PREF(8,"$23");
    __asm__("" : "+r"(tileU));
    sprites = (RrSpritePacketSignedClut *)cursor;
    /* The original retains a shadow branch, but runs only pass zero. */
    do {
        SetSprt(cursor);
        if (pass == 1) {
            sprites->r = shadowBrightness;
            sprites->g = shadowBrightness;
            sprites->b = shadowBrightness;
        } else {
            displacement = 0;
            if (*animation < 0x80) {
                sprites->r = (u8) *animation;
                sprites->g = (u8) *animation;
                sprites->b = (u8) *animation;
                nextTimer = (u16) *animation + 8;
                *animation = nextTimer;
                curve = *animation/8;
                curve *= curve;
                if(curve<0)curve+=7;
                curve >>= 3;
                if (*animation >= 0x31) {
                    displacement = curve - 4;
                } else {
                    displacement = -curve;
                }
            } else {
                s32 brightness=RR_GLYPH_PREF(128,"$2");
                sprites->r = brightness;
                sprites->g = brightness;
                sprites->b = brightness;
            }
        }
        __asm__("" :: "r"(tileU), "r"(shadowBrightness));
        linkPacket=sprites;
        if ((s16)displacement > 0) {
            displacement = 0;
        }
        cursor += 0x14;
        sprites->x = (s16)((u32)displacement + ((u32)x + (u32)pass));
        yStep = pass * 2;
        __asm__("" : "+r"(sprites) : "r"(yStep),"r"(y));
        sprites->u = (s8)tileU;
        sprites->v = tileV;
        sprites->y = (s16)((u32)y + (u32)yStep);
        sprites->w = 8;
        sprites->h = 0x18;
        sprites->clut = clut;
        sprites++;
        AddPrim(&D_800ACC14->head70, (u32 *)linkPacket);
        pass++;
    } while (pass <= 0);
    drawV = (s16) tileV;
    if (drawV < 0) {
        drawV += 0xFF;
    }
    SetDrawMode(cursor, 0, 1, ((s32) (drawV & 0x100) >> 4) | 0x2F, 0);
    AddPrim(&D_800ACC14->head70, (u32 *)cursor);
    RR_GLYPH_ANIMATION_SCRATCH->cursor = cursor + 0xC;
}
