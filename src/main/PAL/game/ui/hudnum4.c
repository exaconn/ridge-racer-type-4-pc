#include "types.h"
#include "rr/frame_storage_types.h"
#include "scratchpad.h"
#include "game/ui/window2.h"
#include "game/race/objsim.h"
#include "psyq/libgpu/primattr.h"
#include "rr/hud_number.h"
#include "rr/fade_sprite.h"
#include "rr/audio_loop.h"
#include "rr/race_types.h"
#include "rr/gpu.h"
#include "game/data/globals.h"
#include "game/data/screenframe.h"
#include "rr/race_setup.h"
#include "game/ui/badge1.h"
#include "rr/trig.h"

extern short NumCars;
extern unsigned char D_8009EE40[];
extern unsigned char D_8009EE44[];
extern RaceVehicle PlayerCars[2];
extern int NumLaps;

void DrawHudDigitMod10(s32 arg0, s32 arg1);
void UpdateHudRankDisplay();
void UpdateFinalLapHudSprite(int a0);

void DrawHudDigitMod10(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_v1;
    char *temp_a3;
    char *b;
    s32 cur, m, r;
    s32 a = arg0;
    b = (char *)HudNumberTargets;
    temp_v1 = arg1 * 0x14;
    temp_a3 = b + temp_v1;
    cur = *(s32 *)SCRATCHPAD;
    temp_s0 = ((s32 *)(DrawBufIndex * 4 + temp_v1 + b))[0];
    m = a % 10;
    r = EmitHudGlyph12Alt(temp_s0, cur, ((s16 *)temp_a3)[6], ((s16 *)temp_a3)[7], m, 0x7EFB);
    *(s32 *)SCRATCHPAD = AddDrawModePrim(temp_s0, r, 5);
}

void UpdateHudRankDisplay(int a0, int a1) {
    int base, a, s0, s1, v;
    SpritePacket *s2, *p2;
    base = CurrentDrawBuf;
    s2 = (SpritePacket *)(base + 0x226B0);
    ((SpritePacket *)(a1 * 0x78 + (int)s2))->u = (a0 + 1) * 0x18;
    p2 = (SpritePacket *)(base + 0x226D8);
    if (GameModeType == 4) {
        a = D_80107178;
    } else {
        if (GameModeType == 3) {
            a = 2;
        } else {
            a = NumCars;
        }
    }
    ((SpritePacket *)(a1 * 0x78 + (int)p2))->u = D_8009EE44[a - 1];
    if (GameModeType == 1) {
        s0 = 0x40;
        s1 = 0;
        if (D_800AC0E6 <= D_8009EE40[D_800AD4EF / 2]) {
            s1 = 1;
            v = rsin((ScreenFrame & 0x3F) << 6);
            if (v < 0) v += 0x3F;
            s0 = v >> 6;
            if (s0 < 0) {
                s1 = 0;
                s0 = -s0;
            }
        }
        s0 += 0x40;
        s2->clut = s1 + 0x7EF8;
        SetShadeTex(s2, 0);
        s2->r = s0;
        s2->g = s0;
        s2->b = s0;
    }
}

void UpdateFinalLapHudSprite(int a0) {
    int a1, a2;
    SpritePacket *p;
    a2 = a0 - 1;
    if (a0 == 0) {
        a1 = 7;
        a2 = 0;
    } else {
        a1 = a2 * 6 + 1;
    }
    p = (SpritePacket *)((char *)CurrentDrawBuf + (a1 * 0x14 + 0x22688));
    if (PlayerCars[a2].control.dynamics94.steering.count22 < NumLaps) {
        p->w = 0;
        return;
    }
    p->w = 0x14;
}
