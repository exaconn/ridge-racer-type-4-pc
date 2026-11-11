#include "game/system/pad.h"
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/controller_hud.h"
#include "game/ui/hudctrl.h"

struct HudTilePacket16 {
    unsigned w0;
    char b4, b5, b6, b7;
    short x, y, w, h;
};


void DrawSteeringIndicator(u32 *ordering, s32 layout);
void DrawSplitScreenDivider(int a0);

void DrawRaceMinimap(s32 arg0) {
    s32 var_s2;
    s32 temp_hi;
    void *var_a1;
    s32 var_s0;
    s32 var_s0_2;
    u32 *var_s3;
    s32 var_s4;
    s32 var_v0;
    RaceVehicle *temp_a0;

    var_s4 = 0;
    if (arg0 == 0) {
        var_s3 = CurrentDrawBuf->ordering.ordering70.words;
        if (LinkHostFlag >= 0) {
            var_s2 = (s16)((u16)LinkHostFlag ^ 1);
        } else {
            var_s2 = 0;
        }
    } else {
        var_s3 = CurrentDrawBuf->buffer.secondaryOrderingB70.words;
        if (arg0 == 1) {
            var_s4 = -0x80;
        }
        if (LinkHostFlag >= 0) {
            {s32 step=((((u16)LinkHostFlag^1)<<16)>>15);step=step-1;var_s2=arg0+step;}
        } else {
            var_s2 = arg0 - 1;
        }
    }
    var_a1 = RR_HUD_SCRATCH->cursor;
    var_s0 = 0;
    if (NumCars > 0) {
        s32 yBase;UiUvRect *const glyph=&HudCheckeredFlagUv;
        do {
            temp_hi = (var_s2 + var_s0) % (s16) NumCars;
            temp_a0 = CarTable[temp_hi];
            if ((u16)temp_a0->flags1E4 & 0x10) {
                u16 clut=(temp_hi<<6)+0x1A16;
                s32 yNumerator=temp_a0->position20.words[2]; s32 xNumerator=temp_a0->position20.words[0];
                s32 xValue;
                xValue=xNumerator/1150+4;
                yBase=var_s4+242;
                var_v0=yBase-yNumerator/1150;
                var_a1 = AddSpritePrim(var_s3, var_a1, xValue, var_v0-4, glyph, clut, 0);
            }
            var_s0 += 1;
        } while (var_s0 < NumCars);
    }
    var_s0_2 = 0;
    if ((arg0 + 1) > 0) {
        s16 originX = 8;
        do {
            s32 x = originX;
            var_v0 = var_s4 + 0x3E;
            if (var_s0_2 <= 0) {
                var_v0 = var_s4 + 0xBE;
            }
            var_a1 = GameQueueSprite(var_s3, var_a1, x, var_v0, 0x34, 0x34, 0x4C, 0x6C, 0x1BD6);
            var_s0_2 += 1;
        } while (var_s0_2 < (arg0 + 1));
    }
    RR_HUD_SCRATCH->cursor = AddDrawModePrim(var_s3, var_a1, 0x25);
}

void DrawSteeringIndicator(u32 *ordering, s32 layout) {
    HudTriangle triangle;
    s16 *outputPoint;
    s32 status;

    void *modeCursor;
    void *spriteCursor;
    s32 parity;
    void *cursor;
    s32 shapeOffset;
    s32 i;
    s32 tickX;
    s32 player;
    s32 baselineY;
    s32 offsetX;
    s32 widthBase,heightBase,color;
    u16 *toggle;
    u8 *shapeY;
    u8 *shapeX;
    u8 encodedX;
    ControllerState5C *input;
    ControllerState5C *inputBase;
    u8 *shapeBase;

    player = 0;
    baselineY = 0x70;
    if (layout != 1) {
        baselineY = 0xF0;
        player = layout == 2;
    }
    inputBase = PlayerControllers;

    input = inputBase + player;
    status = input->protocol10;
    if (status == 2) {
        if (input->edges08.fields.pressed & 0x100) {
            toggle = (u16 *)&HudLapFlashToggles[player];
            *toggle ^= 1;
        }
        i = GetControllerWarning(player);
        if (i > 0) {
            if ((ScreenFrame & 3) >= 2U || i == status) {
                modeCursor = AddDrawModePrim(ordering, RR_HUD_SCRATCH->cursor, 0x25);
                RR_HUD_SCRATCH->cursor = modeCursor;
                spriteCursor = GameQueueSprite(ordering, (SpritePacket *)RR_HUD_SCRATCH->cursor, 0x80, baselineY - 0xA, 0x44, 8, 0x80, 0x40, 0x1B16);
                RR_HUD_SCRATCH->cursor = spriteCursor;
                RR_HUD_SCRATCH->cursor = AddDrawModePrim(ordering, spriteCursor, 0x25);
            }
        }
        i = 0;
        if (HudLapFlashToggles[player] != 0) {
            cursor = RR_HUD_SCRATCH->cursor;
            widthBase = 2;
            heightBase = 6;
            color = 255;
            tickX = 0x63;
            do {
                parity = i & 1;
                offsetX = widthBase - parity;
                cursor = AddTilePrim(ordering, cursor, tickX, baselineY | (parity * 2), offsetX, heightBase - (parity * 4), color, color, color);
                i += 1;
                tickX += 0xF;
            } while (i < 9);
            offsetX = (PlayerControllers[player].steering3C * 0x3C) / 127;
            if (offsetX >= 0x3D) {
                offsetX = 0x3C;
                goto drawNeedle;
            }
            i = 0;
            if (offsetX < -0x3C) {
                offsetX = -0x3C;
drawNeedle:
                i = 0;
            }
            shapeBase = HudRotatingQuadVerts;
            shapeOffset = 1;
            outputPoint = triangle.half;
            shapeX = shapeBase;
            do {
                shapeY = (u8 *)(shapeOffset + (unsigned long)shapeBase);
                shapeOffset += 2;
                encodedX = *shapeX;
                shapeX += 2;
                outputPoint[0] = (s16) (offsetX + (s16)((s8) encodedX + 0xA0));
                i += 1;
                outputPoint[1] = (s16) ((s8) *shapeY + baselineY + 5);
                outputPoint += 2;
            } while (i < 3);
            RR_HUD_SCRATCH->cursor = (u8 *)EmitPolyF3(ordering, cursor, triangle.word[0], triangle.word[1], triangle.word[2], HudIndicatorTriColor, 0);
        }
    }
}

void DrawSplitScreenDivider(int a0)
{
    struct HudTilePacket16 *p;
    FrameStorage *o;
    p = *(struct HudTilePacket16 **)SCRATCHPAD;
    ((char *)p)[3] = 3;
    p->b7 = 0x60;
    if (a0 == 0) {
        p->y = 0x7E;
        p->w = 0x140;
        p->x = 0;
        p->h = 4;
    } else {
        p->x = 0x9E;
        p->w = 4;
        p->y = 0;
        p->h = 0x100;
    }
    p->b4 = 8;
    p->b5 = 8;
    p->b6 = 8;
    o = CurrentDrawBuf;
    p->w0 = (p->w0 & 0xFF000000) | (o->buffer.secondaryOrderingB70.words[0] & 0xFFFFFF);
    *(unsigned *)&o->buffer.secondaryOrderingB70.words[0] =
        (o->buffer.secondaryOrderingB70.words[0] & 0xFF000000) | ((unsigned)p++ & 0xFFFFFF);
    *(struct HudTilePacket16 **)SCRATCHPAD = p;
}
