#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu.h"
#include "rr/animated_glyph.h"
#include "game/gfx/fontdraw.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
#include "rr/player_status.h"

extern unsigned char D_8009EDF8[];

void DrawAnimatedGlyph(s32 x, s32 y, s32 glyph, s32 clut, s16 *animation);

void DrawBigFontString(s32 a0, s32 a1, u8 *a2) {
    SpritePacket *s3, *s2;
    s32 c, a, v, row, col, u, vv;
    s3 = *(SpritePacket **)SCRATCHPAD;
    s2 = s3;
    for (; *a2 != 0; a0 += 10) {
        a = *a2 - 0x20;
        a2++;
        if (a != 0) {
            a = FontAtlasTileCoords[a * 2] + (FontAtlasTileCoords[a * 2 + 1] << 5);
            row = a / 21;
            col = a % 21;
            u = col * 12;
            vv = row * 24 + 0x24;
            SetSprt(s3);
            s2->w = 12;
            s2->h = 24;
            SetShadeTex(s3, 1);
            SetSemiTrans(s3, 1);
            s2->x = a0;
            s2->y = a1;
            s2->u = u;
            s2->v = vv;
            s2->clut = 0x5B20;
            AddPrim(&CurrentDrawBuf->buffer.ordering70.words[0], (u32 *)s2++);
            s3++;
        }
    }
    SetDrawMode(s3, 0, 1, 0x58, 0);
    AddPrim(&CurrentDrawBuf->buffer.ordering70.words[0], (u32 *)s3);
    *(char **)SCRATCHPAD = (char *)s3 + 0xC;
}

void *AddTextSprites(u32 *ordering, void *cursor, s32 screenX, s32 screenY,
                             const char *text, s32 clut, s32 spacing, s32 color)
{
    int tileU;
    int tileV;
    int glyph;
    int brightness;
    u32 margin;
    SpritePacket *packet;
    SpritePacket *emitted;
    brightness = color >> 1;
    if ((*text) != 0) {
        packet = cursor;
        do {
            glyph = (u8)*text - 0x20;
            text++;
            if (glyph != 0) {
                tileU = FontAtlasTileCoords[glyph * 2] << 3;
                tileV = FontAtlasTileCoords[(glyph * 2) + 1] << 4;
                SetSprt(cursor);
                if (brightness == 0) {
                    SetShadeTex(cursor, 1);
                } else {
                    packet->r = brightness;
                    packet->g = brightness;
                    packet->b = brightness;
                }
                SetSemiTrans(cursor, color & 1);
                packet->x = screenX;
                packet->y = screenY;
                packet->w = 8;
                packet->u = tileU;
                packet->v = tileV;
                packet->h = 0x10;
                packet->clut = clut;
                emitted = packet;
                packet++;
                cursor = (SpritePacket *)cursor + 1;
                AddPrim(ordering, (u32 *)emitted);
            }
            if (spacing == 1) {
                margin = 2;
                screenX = screenX + margin + D_8009EDF8[((tileU / 8) + ((tileV / 16) * 32)) % 54];
            } else if (spacing == 2) {
                u32 advance = D_8009EDF8[((tileU / 8) + ((tileV / 16) * 32)) % 54];
                screenX += advance;
            } else {
                screenX += 6U;
            }
        } while (*text != 0);
    }
    return cursor;
}

void DrawAnimatedGlyph(s32 x, s32 y, s32 glyph, s32 clut, s16 *animation) {
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
    register SpritePacketSignedClut *sprites __asm__("$16");
#else
    SpritePacketSignedClut *sprites;
#endif
    u8 *cursor;
    SpritePacketSignedClut *linkPacket;

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
    /* ASM_BARRIER: launder a2 so the second (a2 & 0xFE00) is not CSE'd with the 0x6000 test */
    __asm__("" : "+r"(tileU));
    sprites = (SpritePacketSignedClut *)cursor;
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
        /* ASM_BARRIER: launder a2 so the second (a2 & 0xFE00) is not CSE'd with the 0x6000 test */
        __asm__("" :: "r"(tileU), "r"(shadowBrightness));
        linkPacket=sprites;
        if ((s16)displacement > 0) {
            displacement = 0;
        }
        cursor += 0x14;
        sprites->x = (s16)(displacement + (x + pass));
        yStep = pass * 2;
        /* ASM_BARRIER: launder a2 so the second (a2 & 0xFE00) is not CSE'd with the 0x6000 test */
        __asm__("" : "+r"(sprites) : "r"(yStep),"r"(y));
        sprites->u = (s8)tileU;
        sprites->v = tileV;
        sprites->y = (s16)(y + yStep);
        sprites->w = 8;
        sprites->h = 0x18;
        sprites->clut = clut;
        sprites++;
        AddPrim(&CurrentDrawBuf->buffer.ordering70.words[0], (u32 *)linkPacket);
        pass++;
    } while (pass <= 0);
    drawV = (s16) tileV;
    if (drawV < 0) {
        drawV += 0xFF;
    }
    SetDrawMode(cursor, 0, 1, ((drawV & 0x100) >> 4) | 0x2F, 0);
    AddPrim(&CurrentDrawBuf->buffer.ordering70.words[0], (u32 *)cursor);
    RR_GLYPH_ANIMATION_SCRATCH->cursor = cursor + 0xC;
}

void DrawAnimatedText(int arg0, int arg1, MenuTextState *p, int arg3, int arg4) {
    int s7 = 0, s4 = 0, s2 = 0, s3;
    unsigned s6;
    unsigned s0;
    unsigned short *s1;
    unsigned a2, v0; int v1;
    if (arg3 == 1) {
        p->s12 = p->bA - p->bB;
        s6 = p->bA - p->bB;
    } else {
        s6 = p->s12;
    }
    s1 = p->p;
    s0 = 0;
    if (s6 != 0) {
        do {
            a2 = *s1++;
            s3 = 0;
            if (a2 & 0x1000) {
                v0 = a2 & 0xFFF;
                if (v0 == 0) {
                    s2 = s3;
                    s4 += 0x18;
                } else {
                    a2 = v0;
                    s3 = 1;
                    goto L_call;
                }
            } else {
                v1 = a2;
                if ((a2 & 0xFE00) == 0x6000) {
                    s0 -= 1;
                    a2 = *s1++;
                    switch (v1 & 0xFFFF) {
                    default:
                        s0 += 1;
                        continue;
                    case 0x6005:
                        p->sE = a2 & 0xFF;
                        break;
                    case 0x6003:
                        p->b1C = a2;
                        break;
                    case 0x6004:
                        p->s14 = (signed char)a2;
                        if (s0 != s6 - 2) {
                            s0++;
                            continue;
                        }
                        s0 = s6 - 1;
                        break;
                    }
                } else {
                    /* ASM_BARRIER: launder a2 so the second (a2 & 0xFE00) is not CSE'd with the 0x6000 test */
                    __asm__("" : "+r"(a2));
                    if ((int)(a2 & 0xFE00) <= 0) {
                        if (a2 == 0) {
                            s2 += 7;
                        } else {
                        L_call:
                            DrawAnimatedGlyph(arg0 + (short)s2, arg1 + (short)s4, a2 - 1, arg4, p->animation28[s7].halves);
                            s2 += 7;
                            if (s3 != 0) {
                                s2 = 0;
                                s4 += 0x1A;
                            }
                            s7 += 1;
                        }
                    }
                }
            }
            s0 += 1;
        } while (s0 < s6);
    }
}
