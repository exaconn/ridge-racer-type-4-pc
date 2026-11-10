#include "game/car/carbound.h"
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/badge_row_types.h"
#include "game/menu/menucalc.h"
#include "game/menu/menustat.h"
#include "game/menu/menuicon.h"
#include "game/gfx/polygt4.h"
#include "game/data/globals.h"
#include "game/data/menuvars.h"
#include "game/ui/badgespr.h"
#include "rr/badge_rows.h"
#include "game/math/rotmat.h"
#include "game/ui/uidraw.h"
extern int InitBadgeRow();
extern int SetGeomOffset();
extern int SetGeomScreen();
extern short MenuIconRowStartX[];
extern short MenuIconRowStartY[];
extern short MenuIconRowWidth[];
extern short MenuIconRowHeight[];
extern BadgeRow1C *BadgeRows;

void DrawMenuCarPreviewScene(int a0) {
    int s0, s1, t0;
    BadgeRow1C *p;
    int one;
    s1 = *(int *)SCRATCHPAD;
    if (a0 < 2 && SubmodeState == 0) {
        s0 = 0;
        do {
            t0 = (s0 == 1 || s0 == 7) ? 8 : 0;
            InitBadgeRow(s0, MenuIconRowStartX[s0], MenuIconRowStartY[s0], MenuIconRowWidth[s0], MenuIconRowHeight[s0], (short)t0, -1);
            s0++;
        } while (s0 < 8);
        one = 1;
        s0 = 7;
        p = &BadgeRows[7];
        do {
            p->half02 = one;
            s0--;
            p--;
        } while (s0 >= 0);
    } else {
        s0 = 0;
        do {
            UpdateBadgeRowAnim(s0);
            s0++;
        } while (s0 < 8);
        s1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], s1, 0, 0, 0x140, 0x200);
        s1 = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], s1, 0x24, 0x60, 0xF8, 2, 8, 8, 8);
        s1 = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[0].x04 + 0x18, BadgeRows[0].y06, 0x1C, 0x20, 0x7C, 0x3C, 0x4A, 0x2E29, 0, 0x80);
        s1 = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[0].x04 + 0x38, BadgeRows[0].y06, 0xA8, 0x20, 0, 0, 0x4A, 0x2E28, 1, 0x80);
        s1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[0].x14, BadgeRows[0].y16, BadgeRows[0].w18, BadgeRows[0].h1A);
        s1 = EmitBadgePatternSprites(s1, 0x12, 2);
        s1 = AddBadgeSpriteStrip(0xE, &CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[1].x04, BadgeRows[1].y06, 0x80);
        s1 = AddBadgeSpriteStrip(0xF, &CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[1].x04 + 0x90, BadgeRows[1].y06, 0x80);
        s1 = AddBadgeSpriteStrip(0x10, &CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[1].x04, BadgeRows[1].y06, 0x80);
        s1 = AddBadgeSpriteStrip(0x11, &CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[1].x04 + 0x90, BadgeRows[1].y06, 0x80);
        s1 = DrawStepProgressBar(&CurrentDrawBuf->buffer.ordering70.words[0], s1);
        s1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], s1, BadgeRows[1].x14, BadgeRows[1].y16, BadgeRows[1].w18, BadgeRows[1].h1A);
        if (MenuSubmodePendingId >= 4) {
            *(int *)SCRATCHPAD = s1;
            BuildCameraMatrices();
            ScaleMatrixRow1(2, (MatrixWords32 *)SCRATCH_ROT_MATRIX);
            SetGeomOffset(0xA0, 0x100);
            SetGeomScreen(0x100);
            DrawMenuGridMesh(0, &CurrentDrawBuf->buffer.ordering70.words[1], 1);
            s1 = *(int *)SCRATCHPAD;
            s1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[1], s1, BadgeRows[7].x14, BadgeRows[7].y16, BadgeRows[7].w18, BadgeRows[7].h1A);
        }
        if (a0 >= 0x3D) {
            s1 = DrawCarStatBars(s1, BadgeRows[0].state10 == 3 ? (a0 - 0x3C) * 2 : 0);
        }
    }
    *(int *)SCRATCHPAD = s1;
}
