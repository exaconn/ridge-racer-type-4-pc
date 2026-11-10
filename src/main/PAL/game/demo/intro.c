/* RODATA 0x80011270 0x2c */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"
#include "rr/line_emit.h"
#include "game/effect/effect.h"
#include "game/effect/fxspr.h"
#include "game/gfx/fadesel.h"
#include "game/gfx/fadeset.h"
#include "game/gfx/fadespr.h"
#include "game/sound/sndvab.h"
#include "rr/driver_control.h"
#include "rr/bios_devices.h"
#include "rr/course_choices.h"
#include "rr/fade_sprite.h"
#include "rr/trig.h"
#include "rr/badge_rows.h"
#include "game/data/trkld1.h"

struct Entry {
    int frame;
    int cmd;
};

struct Buf {
    char unk00[0x19];
    unsigned char r;
    unsigned char g;
    unsigned char b;
    char unk1C[0x22778 - 0x1C];
};

extern struct Entry IntroTimeline[];
extern int FrameCounter;
extern struct Buf DrawBuffers[];
extern FadeRaceContext RaceContext;
#include "rr/cd_directory_buffer.h"
extern s32 D_800ACA94;
extern s16 ModeIndex;
extern s16 ModeGroup;
extern s16 SubmodeState;

int IntroScriptStep(void);
void IntroInit(void);

int IntroScriptStep(void) {
    short sp28[4];
    unsigned char sp30[4];
    int i;
    int v;
    int cmd;
    int ret = 0;

    cmd = -1;
    for (i = 0; IntroTimeline[i].frame >= 0; i++) {
        if (IntroTimeline[i].frame == RaceContext.f3E4) {
            cmd = IntroTimeline[i].cmd;
            break;
        }
    }

    switch (cmd) {
    case 0:
        RaceContext.f3F4 = 1;
        break;
    case 1:
        SetParticleBrightFade(0x20, 0x100, 0);
        break;
    case 2:
        RaceContext.f3F8 = 1;
        break;
    case 3:
        RaceContext.f3F8 = 2;
        RaceContext.f3FC = 0;
        break;
    case 4:
        RaceContext.f408 = 1;
        break;
    case 5:
        RaceContext.f408 = 2;
        break;
    case 6:
        RaceContext.f410 = 1;
        break;
    case 7:
        RaceContext.f418 = 1;
        break;
    case 8:
        RaceContext.f414 = 1;
        RaceContext.f41C = 1;
        RaceContext.f424 = 0;
        break;
    case 9:
        RaceContext.f434 = 1;
        RaceContext.f43C = 0;
        RaceContext.f438 = 0;
        break;
    case 10:
        RaceContext.f434 = 2;
        RaceContext.f43C = 0;
        break;
    }

    if (RaceContext.f428 == 1) {
        RaceContext.scale3E8 -= 0x10;
        if (RaceContext.scale3E8 <= 0x80) {
            RaceContext.scale3E8 = 0x80;
            RaceContext.f428 = 2;
        }
    }

    switch (RaceContext.f434) {
    case 1:
        RaceContext.f43C++;
        RaceContext.f440 = (RaceContext.f43C * 0xFF) / 16;
        if (RaceContext.f43C >= 0x10) {
            RaceContext.f434 = 0;
            RaceContext.f440 = 0xFF;
        }
        break;
    case 2:
        RaceContext.f43C++;
        RaceContext.f440 = ((0x10 - RaceContext.f43C) * 0xFF) / 16;
        if (RaceContext.f43C >= 0x10) {
            RaceContext.f434 = 0;
            RaceContext.f440 = 0;
        }
        break;
    }

    if (RaceContext.f440 > 0) {
        DrawIntroRippleSprites();
    }

    if (RaceContext.f3F4) {
        UpdateParticles();
    }

    switch (RaceContext.f3F8) {
    case 1:
        RaceContext.f404 += 8;
        if (RaceContext.f404 >= 0x100) {
            RaceContext.f404 = 0x100;
            RaceContext.f3F8 = 0;
        }
        break;
    case 2:
        RaceContext.f3FC++;
        if (RaceContext.f3FC >= 0x10) {
            RaceContext.f3F8 = 0;
            RaceContext.f3FC = 0x10;
            if (RaceContext.f430 == 0) {
                PlaySfxBank2(1);
                RaceContext.f430 = 1;
            }
        }
        RaceContext.f400 = 0xCE - (RaceContext.f3FC * 0x42) / 16;
        break;
    case 3:
        RaceContext.f404 -= 8;
        if (RaceContext.f404 <= 0) {
            RaceContext.f404 = 0;
            RaceContext.f3F8 = 0;
        }
        break;
    }

    EmitFadeSprite(0, 0x2C, RaceContext.f400, RaceContext.f404);

    if (RaceContext.f410) {
        int c;
        RaceContext.f412++;
        if (RaceContext.f412 > 0x10) {
            RaceContext.f412 = 0x10;
        }
        c = 0xFF - RaceContext.f3EC;
        c *= RaceContext.f404;
        if (c < 0) {
            c += 0xFF;
        }
        c >>= 8;
        c *= RaceContext.scale3E8;
        if (c < 0) {
            c += 0xFF;
        }
        c >>= 8;
        sp30[0] = sp30[1] = sp30[2] = c;
        sp28[0] = 0x10;
        sp28[2] = RaceContext.f412 * 18 + 0x10;
        for (i = 0; i < 2; i++) {
            sp28[1] = sp28[3] = i * 40 + 0x172;
            AddLineF2Prim(sp28, sp30, 0, 10);
            sp28[1] = sp28[3] = i * 40 + 0x173;
            AddLineF2Prim(sp28, sp30, 0, 10);
        }
    }

    if (RaceContext.f418) {
        int v2;
        RaceContext.f41A++;
        if (RaceContext.f41A > 0x10) {
            RaceContext.f41A = 0x10;
            v2 = 0x100;
        } else {
            v2 = RaceContext.f41A * 16;
        }
        v2 = (v2 * RaceContext.f404) / 256;
        EmitFadeSprite(3, 0x42, 0x17C, v2);
    }

    if (RaceContext.f414) {
        RaceContext.f416++;
        if (RaceContext.f416 > 0x10) {
            RaceContext.f416 = 0x10;
            v = 0x100;
        } else {
            v = RaceContext.f416 * 16;
        }
        v = (v * RaceContext.f404) / 256;
        EmitFadeSprite(2, 0x68, 0x13C, v);
        if (RaceContext.f428 == 0) {
            v = (rsin((FrameCounter << 7) & 0xF80) * v) / 4096;
        }
        EmitFadeSprite(1, 0x68, 0x13C, v);
    }

    switch (RaceContext.f408) {
    case 1:
        RaceContext.f3EC += 8;
        if (RaceContext.f3EC >= 0xFF) {
            RaceContext.f3EC = 0xFF;
            RaceContext.f408 = 0;
            RaceContext.f3F4 = 0;
        }
        break;
    case 2:
        RaceContext.f3EC -= 8;
        if (RaceContext.f3EC <= 0) {
            RaceContext.f3EC = 0;
            RaceContext.f408 = 0;
        }
        break;
    }

    UpdateFadeTint(RaceContext.f3EC);
    *(void **)SCRATCHPAD = AddTileGrid2x2(&CurrentDrawBuf->buffer.ordering70.words[703], *(int *)SCRATCHPAD, 0, 0, 0x140, 0x200,
                                       RaceContext.r3F0, RaceContext.g3F1, RaceContext.b3F2);
    for (i = 0; i < 2; i++) {
        DrawBuffers[i].r = RaceContext.r3F0;
        DrawBuffers[i].g = RaceContext.g3F1;
        DrawBuffers[i].b = RaceContext.b3F2;
    }

    RaceContext.f3E4++;
    if (RaceContext.f3E4 > 3000) {
        RaceContext.f3E4 = 3000;
    }

    if (RaceContext.f41C == 1) {
        RaceContext.f424++;
        if (RaceContext.f424 >= 1000) {
            if (RaceContext.f3E0 == 0) {
                RaceContext.f408 = 2;
                RaceContext.f3E0 = 1;
                SetParticleBrightFade(0x20, 0, 0x100);
                RaceContext.f3F4 = 1;
            } else {
                RaceContext.f408 = 1;
                RaceContext.f3E0 = 0;
                SetParticleBrightFade(0x20, 0x100, 0);
            }
            RaceContext.f424 = 0;
        }
        ret = 1;
    } else if (RaceContext.f41C == 3) {
        RaceContext.f420++;
        if (RaceContext.f420 > 20) {
            RaceContext.f420 = 20;
            ret = 2;
        } else {
            ret = 1;
        }
    }

    return ret;
}

void IntroInit(void) {
    SetDispMask(0);
    PsyqBzero(&D_800ACA94, 0x78);
    InitDemoFadeContext(0);
    SelectDemoFadePreset(0);
    LoadCursor = CdDirectoryBuffer;
    RaceLoadReset();
    ModeGroup = 2;
    ModeIndex = 6;
    SubmodeState = 0;
}
