#include "rr/sprite_emit.h"
#include "rr/gpu_line.h"
#include "rr/gpu_scratch_types.h"
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "game/race/objsim.h"
#include "game/ui/hudmap.h"
#include "game/course/trktext.h"
#include "game/ui/uipart1.h"
#include "rr/race_types.h"
#include "rr/track_lateral.h"
#include "rr/badge_rows.h"

extern char StrTotalTimeLabel[];
extern char LapTimeLabel[];
extern char StrPressStart[];
extern short HudMapZoomMode;
extern int NumLaps;
extern int TotalRaceTime;
extern int LapTimes[];
extern char HudSignalLightUv[];
extern int RaceState;



static inline u32 ScaleLineDeltaQ8(u32 delta, u32 scale) {
    s32 product = delta * scale;
    return product >> 8;
}

void DrawRaceTimesScreen(void);
void DrawRaceStartSignalSprite(int a0);
void AddPartialLine(u32 *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);

void DrawRaceTimesScreen(void) {
    char sp18[8];
    int i;
    int y;
    int *q;
    char *b;
    int col, v;
    int sel;
    *(int *)SCRATCHPAD = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], *(int *)SCRATCHPAD, 0, 0, 0x140, 0x100);
    i = 0;
    sel = HudMapZoomMode;
    if (NumLaps > 0) {
        y = 0x86;
        q = LapTimes;
    L1:
        v = *q;
        col = 0x7EF8;
        if (i == sel) {
            col = 0x7EF8;
        }
        DrawRaceTime(0x78, y, v, col, 1);
        y += 0xA;
        q += 1;
        if (++i < NumLaps) goto L1;
    }
    DrawRaceTime(0x78, 0x5E, TotalRaceTime, 0x7EF8, 1);
    DrawFontStringCentered(0x78, 0x50, StrTotalTimeLabel, 0x80, 0);
    DrawFontStringCentered(0x78, 0x78, LapTimeLabel, 0x80, 0);
    i = 7;
    b = sp18 + 7;
    do {
        *b = 0;
        i -= 1;
        b -= 1;
    } while (i >= 0);
    FormatDecimalDigits(sp18, HudDisplayedLapIndex + 1, 1);
    DrawFontStringCentered(0x78, 0x3C, sp18, 0x80, 0);
    DrawFontStringCentered(0x78, 0xC8, StrPressStart, 0x80, 0);
    AddSetTexpagePrim(0);
}

void DrawRaceStartSignalSprite(int a0) {
    int a3;
    char *s0;
    void *p;
    if ((RaceState - 1) < 3U) {
        p = *(void **)SCRATCHPAD;
        if (a0 > 0) {
            a3 = ((a0 - 1) << 7) + 0x38;
            if (a0 == 1) {
                s0 = &CurrentDrawBuf->buffer.ordering70.words[0];
            } else {
                s0 = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
            }
        } else {
            a3 = 0x78;
            s0 = &CurrentDrawBuf->buffer.ordering70.words[0];
        }
        *(int *)SCRATCHPAD = AddDrawModePrim(s0, AddSpritePrim(s0, p, 0x6A, a3, HudSignalLightUv, 0x7EF8, 0), 0x3E);
    }
}

void AddPartialLine(u32 *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    int d1, d2, x, y;
    void *r;
    if (a5 > 0) {
        d1 = a3 - (u32)a1;
        d2 = a4 - (u32)a2;
        if (a5 >= 0x100) a5 = 0x100;
        r = ((GpuScratchPointers *)SCRATCHPAD)->cursor;
        x = a1 + ScaleLineDeltaQ8(d1, a5);
        y = a2 + ScaleLineDeltaQ8(d2, a5);
        if (a10 >= 0) {
            r = AddLineF2SemiTrans(a0, r, a1, a2, x, y, a7, a8, a9);
            if (a6 != 0) {
                r = AddLineF2SemiTrans(a0, r, a1, a2 + 1U, x, y + 1U, a7, a8, a9);
            }
            r = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], r, ((a10 & 3) << 5) | 5);
        } else {
            r = GameQueueTileTrans(a0, r, a1, a2, x, y, a7, a8, a9);
            if (a6 != 0) {
                r = GameQueueTileTrans(a0, r, a1, a2 + 1U, x, y + 1U, a7, a8, a9);
            }
        }
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = r;
    }
}


int IsCarFacingReverse(RaceVehicle *vehicle) {
    int reverseHeading = 0x800 - TrackNodes[vehicle->trackNodeF0].road.heading;
    return (unsigned)(((vehicle->heading1D4 - reverseHeading) & 0xFFF) - 0x401) < 0x7FF;
}
