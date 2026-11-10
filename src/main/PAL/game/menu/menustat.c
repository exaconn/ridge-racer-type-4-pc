/* RODATA 0x80010538 0x18 */
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "game/data/trkld1.h"
#include "game/menu/menuicon.h"
#include "game/ui/barsel0.h"
#include "game/menu/menustat.h"
#include "game/data/selection.h"
#include "game/data/menuvars.h"
#include "rr/vehicle_table.h"
#include "rr/screen_tiles.h"
#include "game/sound/cdstart.h"
#include "game/ui/badgespr.h"
#include "game/sound/cdplay.h"
#include "game/ui/badge1.h"
#include "game/menu/mnrows.h"
typedef struct { s32 unk00[5]; unsigned char b14; char p15, p16; signed char b17; char p18[2]; short h1A; } S4D8;

extern int CdAudioInit();
extern unsigned int ModeFrame;
extern unsigned short D_800F4D8A;
extern S4D8 SelectionState;
extern RaceVehicle PlayerCars[2];
extern int SavedLoadCursor;
extern unsigned char D_8009F070[];
extern int D_800F3A60;
extern unsigned short D_800AC5D0;
extern short D_800F3A26;
extern unsigned char D_8009F074[];
extern signed char D_8009F108[];

void CarStandingsModeUpdate();

void CarStandingsModeUpdate(void) {
    int sp[3];
    unsigned int x;
    int r;
    S4D8 *p;
    ModeFrame++;
    if (ModeFrame > 0x7FFFF) ModeFrame = 0x1000;
    switch (D_800F4D8A) {
    case 0:
        p = &SelectionState;
        if (p->h1A == 6) {
            if (LoadCarRangeStep(0, 1) == 1) {
                ResetCarState(PlayerCars);
                SavedLoadCursor = LoadCursor;
                r = (p->b17 - 1) / 2;
                x = D_8009F070[p->b14];
                if (r >= 3) { r = 2; x = x * 3; }
                InitCameraState(r, x);
                SetRaceLoadCursor(LoadCursor);
                D_800F4D8A = 3;
            }
        } else if (ModeFrame >= 0x96) {
            if (D_800F3A60 != 0 || ModeFrame >= 0x177) {
                D_800F4D8A = 1;
            }
        }
        break;
    case 1:
        D_800AC5D0 = 0x20;
        D_800F4D8A = 5;
        break;
    case 3:
        if (LoadMenuCarStep(SelectionRows[0], SelectionRows[1])) D_800F4D8A = 4;
        break;
    case 4:
        if (D_800F3A60 != 0 || ModeFrame >= 0x177) {
            CdAudioInit();
            CdAudioSelectTrack(0, 1);
            D_800F4D8A = 1;
        }
        break;
    case 5:
        D_800AC5D0--;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x20 - (short)D_800AC5D0) << 3, 0x45);
        *(void **)SCRATCHPAD = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], *(int *)SCRATCHPAD, 0, 0, 0x140, 0x200);
        if ((short)D_800AC5D0 <= 0) {
            if (D_800AD4F2 == 6) {
                InitMenuArenas();
                ModeGroup = 1;
                D_800F3A26 = 0x21;
                D_800F4D8A = 0;
                LoadCursor = SavedLoadCursor;
                CdAudioBeginPlay(0);
            } else {
                ModeGroup = 1;
                D_800F3A26 = 0x11;
                D_800F4D8A = 0;
            }
            return;
        }
        break;
    default:
        break;
    }
    DrawMenuCarPreviewScene(ModeFrame);
    r = x = ModeFrame;
    if (x < 0x20) {
        sp[0] = (int)(x * 255 + (0x20 - x) * 8) / 32;
        sp[1] = (int)(x * 189 + (0x20 - x) * 8) / 32;
        r = 0x20 - x;
        sp[2] = r / 4;
    } else {
        sp[0] = 0xFF;
        sp[1] = 0xBD;
        sp[2] = 0;
    }
    *(void **)SCRATCHPAD = AddTileGrid2x2(&CurrentDrawBuf->buffer.ordering70.words[703], *(int *)SCRATCHPAD, 0, 0, 0x140, 0x200, sp[0], sp[1], sp[2]);
}

int AddBadgeSpriteStrip(int a, int p, int x, int y, int w, int z) {
    int t0 = x;
    int s1 = y;
    int i = 0;
    do {
        signed char *q = &D_8009F108[i + a * 8];
        int v = *q;
        unsigned char *e;
        int s0, d, c0, c1, c3;
        i++;
        if (v == -1) break;
        e = D_8009F074 + v * 4;
        d = q[4];
        c0 = e[0]; c1 = e[1]; s0 = e[2]; c3 = e[3];
        s1 += d;
        t0 = AddSpriteWithDrawMode(p, t0, s1, w, s0, c3, c0, c1, 0x4A, 0x2E28, 1, z);
        s1 += s0;
    } while (i < 4);
    return t0;
}
