#include "game/car/carstat.h"
#include "rr/line_emit.h"
#include "rr/frame_storage_types.h"
#include "types.h"
#include "game/system/pad.h"
#include "rr/ui.h"

/* RaceVehicle / course setup controller (one player). */

typedef struct {
    s16 x00, y02;
    s32 words04[4];
} Ovl665View14;

typedef struct {
    s32 scene00, reserved04, selection08;
    s32 step0C, detail10, next14, kind18;
    s32 angle1C, phase20, fade24;
    u16 buttons28;
    s16 course2A;
    s32 ready2C;
    u8 pad30[8];
    Ovl665View14 view38;
    u8 pad4C[0x3A];
    u16 rotation86;
    u8 pad88[4];
    s16 offset8C, offset8E, offset90;
    u8 pad92[2];
    s16 offset94, offset96, offset98;
    u8 pad9A[6];
} Ovl665CarState;

typedef struct {
    struct { s16 car00, colour02; } a[2];
    struct { s16 h[8]; } b[2];
    struct { s8 c[4]; } c[2];
} Ovl665Settings;

typedef struct {
    u8 byte00, byte01;
    s16 h02;
    s16 x04, y06;
    u8 pad08[8];
    s16 state10;
} Ovl665Panel;

typedef struct {
    u8 pad00[4];
    u8 b4;
} Ovl665Text;


typedef struct {
    u32 tag;
    u32 command;
} Ovl665Packet;

typedef struct {
    u8 a, pad1, b, pad3;
} Ovl665Colour4;

typedef struct {
    s16 x0, y0, x1, y1;
} Ovl665Line;

extern Ovl665CarState ovl665_D_80119768;
extern s32 ovl665_D_80119760;
extern Ovl665Settings Options;
extern Ovl665Panel *BadgeRows;
extern Ovl665Text *CardDialogueStatePtr;
extern s16 FadeCountdown;
extern s32 ActivePlayerIndex;
extern s32 D_800F4CB8;
extern u8 ScreenFadeState;
extern Ovl665View14 ovl665_D_80119698[];
extern Ovl665Colour4 ovl665_D_8011902C[][5];
extern Ovl665Colour4 ovl665_D_801190CC[][5];
extern Ovl665Colour4 ovl665_D_8011916C[][5];
extern void *ovl665_D_80119290[][5];
extern void *ovl665_D_801193E8[][5];
extern void *ovl665_D_80119558[][5];
extern void *ovl665_D_801195F8[][5];
extern s32 ovl665_D_80119740[];
extern s32 ovl665_D_80119724[];
extern u8 ovl665_D_8011973C[];
extern u8 ovl665_D_80119020[], ovl665_D_80119024[];
extern s16 TwistRangeLimits[];
extern s16 PadRangeLimits[];
extern s32 WheelDeadzoneTable[];

extern s32 PadGetState(s32);
extern void PadSetMainMode(s32, s32, s32);
extern s32 LoadTrophyModelStep(void);
extern s32 ovl665_func_8011629C(s32);
extern void SetPresentationDrawEnabled(s32);
extern void StepMenuValue(s32, s32, void *, s32, s32);
/* Sound cue; several call sites pass the freshly computed offset as a
 * second (ignored) argument, so it is declared without a prototype. */
extern void PlaySfx();
extern void ovl665_func_80116AA4(void *, s32);
extern void *AddSpritePrim(void *, void *, s32, s32, void *, s32, s32);
extern void ovl665_func_8011643C(void);
extern void DrawUiRow(s32);
extern void AddScreenFadeTiles(void *, s32, s32);
extern void ovl665_func_80114A24(void);
extern void ClearScreenOverlayState();
extern void InitBadgeRow(s32, s32, s32, s32, s32, s32, s32);
extern void SetMenuFrameOptions(s32, s32, s32);
extern s32 rsin(s32);
extern void PadSetRumbleFlags(s32, s32);
extern void ovl665_func_801169BC(s32);
extern void UpdateBadgeRowAnim(s32);
extern void UpdateDialogue(s32, s32, void *);
extern void UpdateScreenFade(s32);

#define ST ovl665_D_80119768

typedef struct {
    void *cursor;
} Ovl665Scratch;

#define SCRATCH ((Ovl665Scratch *)0x1F800000)

#define CLOSE_ROWS(n)                                   \
    BadgeRows->state10 = 4;                            \
    for (i = 0; i < (n); i++) {                         \
        if (UiRows[i].byte14 == 2) {                \
            UiRows[i].byte14 = 3;                   \
        }                                               \
    }

#define COPY_COLOURS(src)                               \
    for (i = 0; i < 5; i++) {                           \
        UiRows[i].byte15 = (src)->a;                \
        UiRows[i].byte16 = (src)->b;                \
        (src)++;                                        \
    }

/* Links a texture-page command packet into the frame ordering list. */
#define LINK_PACKET()                                                       \
    {                                                                       \
        FrameStorage *frame;                                                 \
        packet = SCRATCH->cursor;                                           \
        ((u8 *)&packet->tag)[3] = 1;                                        \
        frame = CurrentDrawBuf;                                                 \
        packet->command = 0xE1000259;                                       \
        packet->tag = (packet->tag & 0xFF000000) | (frame->buffer.ordering70.words[0] & 0xFFFFFF); \
        frame->buffer.ordering70.words[0] = (frame->buffer.ordering70.words[0] & 0xFF000000) | ((u32)packet & 0xFFFFFF); \
        packet++;                                                           \
        SCRATCH->cursor = packet;                                           \
    }

void ovl665_func_80116D10(s32 player) {
    Ovl665Colour4 *src;
    PadPort28 *entry;
    s32 changed;
    s32 i;
    s32 kind;
    s32 index;
    s32 old;
    s32 value;
    s32 phase;
    s32 wave;
    s32 arg;
    s16 timer;
    void *cursor;
    Ovl665Line line;
    Ovl665Packet *packet;

    ST.buttons28 = PlayerControllers[player].edges08.fields.pressed;
    entry = &PadPorts[player];
    changed = 0;
    switch (ST.step0C) {
    case 0:
        if (ovl665_D_80119760 == 0) {
            if (PlayerControllers[player].flag40 == 1) {
                if (ST.ready2C == 0) {
                    if (PlayerControllers[player].input00.fields.type != 0xE3) {
                        if (PadGetState(player << 4) == 6) {
                            PadSetMainMode(player << 4, 1, 0);
                        }
                    } else {
                        ST.ready2C = 1;
                        ovl665_D_80119760 = 1;
                    }
                }
            } else {
                ovl665_D_80119760 = 1;
            }
        }
        ActivePlayerIndex = player;
        if (LoadTrophyModelStep() == 0) {
            break;
        }
        if (ovl665_D_80119760 == 0) {
            break;
        }
        kind = ovl665_func_8011629C(player);
        ST.kind18 = kind;
        if (kind >= 0) {
            switch (kind) {
            case 0:
            case 1:
                index = Options.a[player].car00;
                src = ovl665_D_8011902C[index];
                index <<= 12;
                ST.rotation86 = index;
                break;
            case 2:
                index = Options.b[player].h[6];
                src = ovl665_D_801190CC[index];
                index <<= 12;
                ST.rotation86 = index;
                break;
            case 3:
                if (PlayerControllers[player].input00.fields.type != 0xE3) {
                    index = Options.a[player].car00;
                    src = ovl665_D_8011902C[index];
                    index <<= 12;
                ST.rotation86 = index;
                } else {
                    index = Options.c[player].c[3];
                    src = ovl665_D_8011916C[index];
                    index <<= 12;
                ST.rotation86 = index;
                }
                ST.course2A = PlayerControllers[player].input00.fields.type;
                break;
            }
            ST.view38 = ovl665_D_80119698[ST.kind18];
            COPY_COLOURS(src);
        } else {
            CLOSE_ROWS(8);
            FadeCountdown = 64;
            ST.step0C = 2;
        }
        ST.detail10 = 0;
        ST.angle1C = 0;
        ST.fade24 = 0;
        ST.offset8C = 0;
        ST.offset8E = 0;
        ST.offset90 = 0;
        ST.offset94 = 0;
        ST.offset96 = 0;
        ST.offset98 = 0;
        SetPresentationDrawEnabled(0);
        ST.step0C++;
        break;
    case 1:
        if (BadgeRows->state10 == 3 && UiRows[0].byte14 == 2) {
            if (ovl665_func_8011629C(player) < 0) {
                CLOSE_ROWS(8);
                FadeCountdown = 64;
                ST.step0C = 2;
                ST.next14 = 0;
            }
            switch (ST.kind18) {
            case 1:
                if (PlayerControllers[player].kind1C != 0) {
                    UiRows[5].cells[0] = 1;
                    UiRows[Options.a[player].colour02 + 6].cells[0] = 0;
                    StepMenuValue(player * 4, 2, &Options.a[player].colour02, 2, 0);
                    UiRows[Options.a[player].colour02 + 6].cells[0] = 2;
                }
            case 0:
                old = Options.a[player].car00;
                StepMenuValue(player * 4 | 1, 2, &Options.a[player].car00, 8, 0);
                if (old != Options.a[player].car00) {
                    if (Options.a[player].car00 < old) {
                        ST.angle1C -= 0x1000;
                    } else {
                        ST.angle1C += 0x1000;
                    }
                    src = ovl665_D_8011902C[Options.a[player].car00];
                    COPY_COLOURS(src);
                    ST.fade24 = 0;
                    SetPadButtonMapLayout(player, Options.a[player].car00, Options.b[player].h[6],
                                  Options.c[player].c[3]);
                }
                if (PlayerControllers[player].action28 != 0) {
                    PlaySfx(1);
                    CLOSE_ROWS(5);
                    FadeCountdown = 64;
                    ST.step0C = 2;
                    ST.next14 = 0;
                }
                ovl665_func_80116AA4(ovl665_D_80119290[Options.a[player].car00], ST.fade24);
                cursor = SCRATCH->cursor;
                if (Options.a[player].car00 > 0) {
                    cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                }
                if (Options.a[player].car00 < 7) {
                    cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                }
                SCRATCH->cursor = cursor;
                break;
            case 2:
                old = Options.b[player].h[6];
                StepMenuValue(player * 4 | 1, 2, &Options.b[player].h[6], 8, 0);
                if (old != Options.b[player].h[6]) {
                    if (Options.b[player].h[6] < old) {
                        ST.angle1C -= 0x1000;
                    } else {
                        ST.angle1C += 0x1000;
                    }
                    src = ovl665_D_801190CC[Options.b[player].h[6]];
                    COPY_COLOURS(src);
                    ST.fade24 = 0;
                    SetPadButtonMapLayout(player, Options.a[player].car00, Options.b[player].h[6],
                                  Options.c[player].c[3]);
                }
                value = PlayerControllers[player].steering20 * 3800 / 256 - 1900;
                ST.offset8C = value / 2;
                ST.offset94 = -value / 2;
                if (PlayerControllers[player].action28 != 0) {
                    PlaySfx(1, value);
                    CLOSE_ROWS(5);
                    FadeCountdown = 64;
                    ST.step0C = 2;
                    ST.next14 = 3;
                }
                ovl665_func_80116AA4(ovl665_D_801193E8[Options.b[player].h[6]], ST.fade24);
                cursor = SCRATCH->cursor;
                if (Options.b[player].h[6] > 0) {
                    cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                }
                if (Options.b[player].h[6] < 7) {
                    cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                }
                SCRATCH->cursor = cursor;
                break;
            case 3:
                if (ST.course2A != PlayerControllers[player].input00.fields.type) {
                    changed = 1;
                    ST.course2A = PlayerControllers[player].input00.fields.type;
                }
                if (PlayerControllers[player].input00.fields.type != 0xE3) {
                    old = Options.a[player].car00;
                    StepMenuValue(player * 4 | 1, 1, &Options.a[player].car00, 8, 0);
                    if (old != Options.a[player].car00) {
                        if (Options.a[player].car00 < old) {
                            ST.angle1C -= 0x1000;
                        } else {
                            ST.angle1C += 0x1000;
                        }
                        changed = 1;
                        SetPadButtonMapLayout(player, Options.a[player].car00, Options.b[player].h[6],
                                      Options.c[player].c[3]);
                    }
                    if (changed) {
                        src = ovl665_D_8011902C[Options.a[player].car00];
                        COPY_COLOURS(src);
                        ST.fade24 = 0;
                    }
                    if (PlayerControllers[player].action28 != 0) {
                        PlaySfx(1);
                        CLOSE_ROWS(5);
                        FadeCountdown = 64;
                        ST.step0C = 2;
                        ST.next14 = 0;
                    }
                    ovl665_func_80116AA4(ovl665_D_80119558[Options.a[player].car00], ST.fade24);
                    cursor = SCRATCH->cursor;
                    if (Options.a[player].car00 > 0) {
                        cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                    }
                    if (Options.a[player].car00 < 7) {
                        cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                    }
                    SCRATCH->cursor = cursor;
                } else {
                    old = Options.c[player].c[3];
                    StepMenuValue(player * 4 | 1, 1, &Options.c[player].c[3], 8, 0);
                    if (old != Options.c[player].c[3]) {
                        if (Options.c[player].c[3] < old) {
                            ST.angle1C -= 0x1000;
                        } else {
                            ST.angle1C += 0x1000;
                        }
                        changed = 1;
                        SetPadButtonMapLayout(player, Options.a[player].car00, Options.b[player].h[6],
                                      Options.c[player].c[3]);
                    }
                    if (changed) {
                        src = ovl665_D_8011916C[Options.c[player].c[3]];
                        COPY_COLOURS(src);
                        ST.fade24 = 0;
                    }
                    value = (s8)PlayerControllers[player].position41 * 16;
                    ST.offset90 = value;
                    if (PlayerControllers[player].action28 != 0) {
                        PlaySfx(1, value);
                        CLOSE_ROWS(5);
                        FadeCountdown = 64;
                        ST.step0C = 2;
                        ST.next14 = 4;
                    }
                    ovl665_func_80116AA4(ovl665_D_801195F8[Options.c[player].c[3]], ST.fade24);
                    cursor = SCRATCH->cursor;
                    if (Options.c[player].c[3] > 0) {
                        cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                    }
                    if (Options.c[player].c[3] < 7) {
                        cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                    }
                    SCRATCH->cursor = cursor;
                }
                break;
            }
            ST.fade24++;
            if (ST.fade24 > 16) {
                ST.fade24 = 16;
            }
        }
        ST.angle1C = ST.angle1C * 15 / 16;
        ST.rotation86 = ST.angle1C;
        if ((ST.angle1C + 128) > 256U) {
            ST.fade24 = 0;
        }
        LINK_PACKET();
        ovl665_func_8011643C();
        for (i = 0; i < 5; i++) {
            DrawUiRow(i);
        }
        if (ST.kind18 == 1 && PlayerControllers[player].kind1C != 0) {
            for (i = 5; i < 8; i++) {
                DrawUiRow(i);
            }
        }
        break;
    case 2:
        timer = FadeCountdown - 1;
        FadeCountdown = timer;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (64 - timer) * 4, 69);
        if (FadeCountdown <= 0) {
            if (ST.next14 == 0) {
                ovl665_func_80114A24();
                ST.step0C = 0;
                ActivePlayerIndex = 0;
                ST.scene00--;
                return;
            }
            ST.step0C = ST.next14;
            ST.detail10 = 0;
            ST.next14 = 0;
        }
        ovl665_func_8011643C();
        for (i = 0; i < 5; i++) {
            DrawUiRow(i);
        }
        break;
    case 3:
        switch (ST.detail10) {
        case 0:
            ClearScreenOverlayState();
            ScreenFadeState = 2;
            InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
            BadgeRows->h02 = 1;
            SetMenuFrameOptions(1, 0, 0);
            ST.view38 = ovl665_D_80119698[4];
            ST.rotation86 = 0;
            ST.angle1C = 0;
            CardDialogueStatePtr->b4 = 9;
            ST.detail10++;
            break;
        case 1:
            value = PlayerControllers[player].steering20 * 3800 / 256 - 1900;
            ST.offset8C = value / 2;
            ST.offset94 = -value / 2;
            if (PlayerControllers[player].edges08.fields.pressed & 0x800) {
                PlaySfx(1, value);
                Options.b[player].h[0] = entry->analog04.bytes[0] - 128;
                Options.b[player].h[1] = entry->analog04.bytes[1];
                Options.b[player].h[2] = entry->analog04.bytes[2];
                Options.b[player].h[3] = entry->analog04.bytes[3];
                CardDialogueStatePtr->b4 = Options.b[player].h[4] + 10;
                ST.view38 = ovl665_D_80119698[5];
                ST.phase20 = 0;
                ST.detail10++;
                SetMenuFrameOptions(1, 1, 0);
            }
            break;
        case 2:
            old = Options.b[player].h[4];
            StepMenuValue(player * 4 | 1, 1, &Options.b[player].h[4], 4, 0);
            if (old != Options.b[player].h[4]) {
                CardDialogueStatePtr->b4 = Options.b[player].h[4] + 10;
            }
            value = rsin(ST.phase20) * ovl665_D_80119740[Options.b[player].h[4]] / 512;
            ST.offset8C = value;
            ST.offset94 = -value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (PlayerControllers[player].edges08.fields.pressed & 0x840) {
                PlaySfx(1, value);
                CardDialogueStatePtr->b4 = Options.b[player].h[5] + 14;
                ST.view38 = ovl665_D_80119698[5];
                ST.phase20 = 0;
                ST.detail10++;
            }
            line.x0 = 148;
            line.x1 = 172;
            line.y0 = line.y1 = ST.view38.y02;
            AddLineF2Prim((const s16 *)&line, ovl665_D_8011973C, 0, 0);
            if (Options.b[player].h[4] > 0) {
                line.y0 = line.y1 = ST.view38.y02 + ovl665_D_80119724[Options.b[player].h[4] - 1];
                AddLineF2Prim((const s16 *)&line, ovl665_D_8011973C, 0, 0);
                line.y0 = line.y1 = ST.view38.y02 - ovl665_D_80119724[Options.b[player].h[4] - 1];
                AddLineF2Prim((const s16 *)&line, ovl665_D_8011973C, 0, 0);
            }
            cursor = SCRATCH->cursor;
            if (Options.b[player].h[4] > 0) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (Options.b[player].h[4] < 3) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            SCRATCH->cursor = cursor;
            break;
        case 3:
            old = Options.b[player].h[5];
            StepMenuValue(player * 4 | 1, 1, &Options.b[player].h[5], 4, 0);
            if (old != Options.b[player].h[5]) {
                CardDialogueStatePtr->b4 = Options.b[player].h[5] + 14;
            }
            value = rsin(ST.phase20) * TwistRangeLimits[Options.b[player].h[5]] / 512;
            ST.offset8C = value;
            ST.offset94 = -value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (PlayerControllers[player].edges08.fields.pressed & 0x840) {
                PlaySfx(1, value);
                CLOSE_ROWS(5);
                FadeCountdown = 64;
                ST.step0C = 2;
            }
            cursor = SCRATCH->cursor;
            if (Options.b[player].h[5] > 0) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (Options.b[player].h[5] < 3) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            SCRATCH->cursor = cursor;
            break;
        }
        if (ovl665_func_8011629C(player) < 0) {
            CLOSE_ROWS(8);
            FadeCountdown = 64;
            ST.step0C = 2;
            ST.next14 = 0;
        }
        LINK_PACKET();
        ovl665_func_8011643C();
        break;
    case 4:
        switch (ST.detail10) {
        case 0:
            ClearScreenOverlayState();
            ScreenFadeState = 2;
            InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
            BadgeRows->h02 = 1;
            SetMenuFrameOptions(1, 0, 0);
            ST.view38 = ovl665_D_80119698[6];
            ST.rotation86 = 0;
            ST.angle1C = 0;
            CardDialogueStatePtr->b4 = 8;
            ST.detail10++;
            break;
        case 1:
            value = (s8)PlayerControllers[player].position41 * 16;
            ST.offset90 = value;
            if (PlayerControllers[player].edges08.fields.pressed & 0x800) {
                PlaySfx(1, value);
                PadSetRumbleFlags(player, 3);
                CardDialogueStatePtr->b4 = Options.c[player].c[0] + 10;
                ST.phase20 = 0;
                ST.detail10++;
                SetMenuFrameOptions(1, 1, 0);
            }
            break;
        case 2:
            old = Options.c[player].c[0];
            StepMenuValue(player * 4 | 1, 1, &Options.c[player].c[0], 4, 0);
            if (old != Options.b[player].h[4]) {
                CardDialogueStatePtr->b4 = Options.c[player].c[0] + 10;
            }
            value = rsin(ST.phase20) * WheelDeadzoneTable[Options.c[player].c[0]] / 128;
            ST.offset90 = value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (PlayerControllers[player].action28 != 0) {
                PlaySfx(1, value);
                CardDialogueStatePtr->b4 = Options.c[player].c[1] + 18;
                ST.phase20 = 0;
                ST.detail10++;
            }
            if (Options.c[player].c[0] > 0) {
                ovl665_func_801169BC(WheelDeadzoneTable[Options.c[player].c[0]] << 5);
                ovl665_func_801169BC(-WheelDeadzoneTable[Options.c[player].c[0]] << 5);
            }
            cursor = SCRATCH->cursor;
            if (Options.c[player].c[0] > 0) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (Options.c[player].c[0] < 3) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            SCRATCH->cursor = cursor;
            break;
        case 3:
            old = Options.c[player].c[1];
            StepMenuValue(player * 4 | 1, 1, &Options.c[player].c[1], 5, 0);
            if (old != Options.c[player].c[1]) {
                CardDialogueStatePtr->b4 = Options.c[player].c[1] + 18;
            }
            value = rsin(ST.phase20) * PadRangeLimits[Options.c[player].c[1]] / 256;
            ST.offset90 = value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (PlayerControllers[player].action28 != 0) {
                PlaySfx(1, value);
                CardDialogueStatePtr->b4 = Options.c[player].c[2] + 23;
                ST.phase20 = 0;
                ST.detail10++;
            }
            cursor = SCRATCH->cursor;
            if (Options.c[player].c[1] > 0) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (Options.c[player].c[1] < 4) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            SCRATCH->cursor = cursor;
            break;
        case 4:
            old = Options.c[player].c[2];
            StepMenuValue(player * 4 | 1, 1, &Options.c[player].c[2], 3, 0);
            if (old != Options.c[player].c[2]) {
                CardDialogueStatePtr->b4 = Options.c[player].c[2] + 23;
            }
            wave = rsin(ST.phase20) * 21;
            value = wave * Options.c[player].c[2] / 256;
            ST.offset90 = value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (PlayerControllers[player].action28 != 0) {
                PlaySfx(1, value);
                CLOSE_ROWS(5);
                FadeCountdown = 64;
                ST.step0C = 2;
            }
            cursor = SCRATCH->cursor;
            if (Options.c[player].c[2] > 0) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (Options.c[player].c[2] < 2) {
                cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            SCRATCH->cursor = cursor;
            break;
        }
        if (ovl665_func_8011629C(player) < 0 || PlayerControllers[player].input00.fields.type != 0xE3) {
            CLOSE_ROWS(8);
            FadeCountdown = 64;
            ST.step0C = 2;
            ST.next14 = 0;
        }
        LINK_PACKET();
        ovl665_func_8011643C();
        break;
    }
    if (ST.step0C < 3) {
        UpdateBadgeRowAnim(0);
        D_800F4CB8 = player + 1;
        arg = 0x10072;
    } else {
        UpdateBadgeRowAnim(0);
        if (BadgeRows->state10 == 3) {
            UpdateDialogue(BadgeRows->x04 + 10, BadgeRows->y06 + 40, CardDialogueStatePtr);
        }
        arg = 0x83;
    }
    UpdateScreenFade(arg);
}
