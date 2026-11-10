/* RODATA 0x80010884 0x54 */
#include "game/ui/hudmap.h"
#include "rr/frame_storage_types.h"
#include "game/gfx/gfxmtx.h"
#include "game/menu/mnrows.h"
#include "game/ui/uidraw.h"
#include "rr/bios_devices.h"
#include "rr/audio_cd_controller.h"
#include "rr/badge_row_types.h"
#include "rr/menu_state_types.h"
#include "rr/ui_panel.h"
#include "rr/trig.h"
#define EP ((E_2 *)((a0 * 16) + Rect16Rows))

typedef struct { short f0, f2, f4, f6, f8, fA, fC, fE; } E_1;

typedef struct {
    short s0, s2, x, y, w, h, st, cnt;
} E_2;

typedef struct {
    short x00, y02, w04, h06;
    short page08, clut0A;
    char u0C, v0D, f0E, f0F, f10, pad11[3];
} GlyphPreset14;

extern BadgeRow1C *BadgeRows;
extern int Rect16Rows;
extern unsigned char D_8009F9C0[];
extern unsigned char D_8009F9CC[];
extern MenuGlyphRow1C *GlyphRows;
extern GlyphPreset14 D_8009F9D4[];

void UpdateBadgeRowAnim();
void InitRect16Row(int i, short a, short b, short c, short d, short e, short f);
s32 UpdateRect16Row(int a0);
void InitGlyphRow();

void UpdateBadgeRowAnim(int a0) {
    int s2;
    if (a0 < BadgeRowCount) {
        s2 = *(s16 *)BadgeRows[a0].pad12;
        switch (BadgeRows[a0].state10) {
        case 0:
            BadgeRows[a0].h1A = 2;
            BadgeRows[a0].state10 = 0xA;
            BadgeRows[a0].w18 = 0;
            *(s16 *)BadgeRows[a0].pad12 = 0;
            BadgeRows[a0].x04 = BadgeRows[a0].x08;
            BadgeRows[a0].y06 = (BadgeRows[a0].y0A + BadgeRows[a0].extent0E) - 2;
            BadgeRows[a0].x14 = BadgeRows[a0].x08;
            BadgeRows[a0].y16 = (BadgeRows[a0].y0A + BadgeRows[a0].extent0E) - 2;
            break;
        case 1:
            BadgeRows[a0].w18 = ((BadgeRows[a0].width0C + 2) * rsin((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                (*(s16 *)BadgeRows[a0].pad12)++;
            } else {
                BadgeRows[a0].state10 = 2;
                *(s16 *)BadgeRows[a0].pad12 = 0;
            }
            break;
        case 2:
            BadgeRows[a0].h1A = (BadgeRows[a0].extent0E * rsin((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].y16 = (BadgeRows[a0].y0A + BadgeRows[a0].extent0E) - BadgeRows[a0].h1A;
            BadgeRows[a0].y06 = BadgeRows[a0].y16;
            if (s2 < 0xF) {
                (*(s16 *)BadgeRows[a0].pad12)++;
            } else {
                BadgeRows[a0].state10 = 3;
                *(s16 *)BadgeRows[a0].pad12 = 0;
            }
            break;
        case 3:
            BadgeRows[a0].x04 = BadgeRows[a0].x08;
            BadgeRows[a0].y06 = BadgeRows[a0].y0A;
            BadgeRows[a0].x14 = BadgeRows[a0].x08;
            BadgeRows[a0].y16 = BadgeRows[a0].y0A;
            BadgeRows[a0].w18 = BadgeRows[a0].width0C + 2;
            BadgeRows[a0].h1A = BadgeRows[a0].extent0E;
            break;
        case 4:
            BadgeRows[a0].h1A = (BadgeRows[a0].extent0E * rcos((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].y16 = BadgeRows[a0].y0A + (BadgeRows[a0].extent0E - BadgeRows[a0].h1A);
            BadgeRows[a0].y06 = BadgeRows[a0].y16;
            if (s2 < 0xF) {
                (*(s16 *)BadgeRows[a0].pad12)++;
            } else {
                BadgeRows[a0].state10 = 5;
                *(s16 *)BadgeRows[a0].pad12 = 0;
            }
            break;
        case 5:
            BadgeRows[a0].w18 = ((BadgeRows[a0].width0C + 2) * rcos((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                (*(s16 *)BadgeRows[a0].pad12)++;
            } else {
                BadgeRows[a0].state10 = 0xB;
                *(s16 *)BadgeRows[a0].pad12 = 0;
            }
            break;
        case 10:
            if (BadgeRows[a0].half02 != 0) {
                BadgeRows[a0].state10 = 1;
            }
            break;
        case 7:
            BadgeRows[a0].w18 = ((BadgeRows[a0].width0C + 2) * rsin((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].h1A = BadgeRows[a0].extent0E;
            BadgeRows[a0].x14 = BadgeRows[a0].x08;
            BadgeRows[a0].y16 = BadgeRows[a0].y0A;
            if (s2 < 0xF) {
                (*(s16 *)BadgeRows[a0].pad12)++;
            } else {
                BadgeRows[a0].state10 = 3;
                *(s16 *)BadgeRows[a0].pad12 = 0;
            }
            break;
        case 6:
        case 9:
        case 11:
            break;
        case 8:
            BadgeRows[a0].w18 = BadgeRows[a0].width0C + 2;
            BadgeRows[a0].h1A = (BadgeRows[a0].extent0E * rsin((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].x14 = BadgeRows[a0].x08;
            BadgeRows[a0].y16 = BadgeRows[a0].y0A + ((BadgeRows[a0].extent0E / 2) - (BadgeRows[a0].h1A / 2));
            if (s2 < 0xF) {
                (*(s16 *)BadgeRows[a0].pad12)++;
            } else {
                BadgeRows[a0].state10 = 3;
                *(s16 *)BadgeRows[a0].pad12 = 0;
            }
            break;
        }
        DrawBadgeRow(a0);
    }
}

int AllocRect16Rows(int a0) {
    int n = Rect16RowCount << 4;
    Rect16Rows = a0;
    PsyqBzero(a0, n);
    DebugPrintStub(StrSizeEqLdFmt, n);
    return n;
}

void InitRect16Row(int i, short a, short b, short c, short d, short e, short f) {
    if (i < Rect16RowCount) {
        E_1 *p = (E_1 *)((i << 4) + Rect16Rows);
        p->f0 = f;
        p->f2 = 0;
        p->f4 = a;
        p->f6 = b;
        p->f8 = c;
        p->fA = d;
        p->fC = e;
    }
}

int UpdateRect16Row(int a0) {
    int lx[4];
    int ly[4];
    int lv[4];
    int r;
    int hh;
    int v;
    int k;
    int a1, a3;
    E_2 *e0, *e1, *e2, *e3, *e4, *e5;
    unsigned char *p;
    int *q;
    int *q2;
    int *xs, *ys;
    unsigned char *tb;

    v = a0 < Rect16RowCount;
    if (v != 0) {
        r = 0;
        e0 = EP;
        a1 = e0->st;
        a3 = e0->cnt;
        k = 3;
        q = &lv[3];
        do {
            *q = 0;
            k--;
            q--;
        } while (k >= 0);
        e1 = EP;
        lx[0] = e1->x;
        ly[0] = e1->y;
        lx[1] = e1->x + e1->w - 1;
        ly[1] = e1->y;
        lx[2] = e1->x;
        ly[2] = e1->y + e1->h - 1;
        lx[3] = e1->x + e1->w - 1;
        ly[3] = e1->y + e1->h - 1;
        switch (a1) {
        case 0:
            e2 = EP;
            e2->st = 1;
            e2->cnt = 0;
            break;
        case 5:
            r = 1;
            hh = 0x100;
            k = 3;
            q2 = &lv[3];
            do {
                *q2 = hh;
                k--;
                q2--;
            } while (k >= 0);
            break;
        case 4:
            lv[2] = 0x100;
        case 3:
            lv[1] = 0x100;
        case 2:
            lv[0] = 0x100;
        case 1:
            lv[a1 - 1] = (a3 << 8) / 6;
            if (a3 < 6) {
                e3 = EP;
                e3->cnt = e3->cnt + 1;
            } else {
                e4 = EP;
                e4->cnt = 0;
                e4->st = e4->st + 1;
            }
            break;
        case 8:
            e5 = EP;
            if (e5->s2 != 0) {
                e5->st = 1;
            }
            break;
        default:
            break;
        }
        k = 0;
        xs = lx;
        ys = ly;
        tb = D_8009F9CC;
        p = D_8009F9C0;
    loop:
        AddPartialLine(&CurrentDrawBuf->buffer.ordering70.words[0], *(int *)((char *)xs + (p[0] << 2)), *(int *)((char *)ys + (p[0] << 2)), *(int *)((char *)xs + (p[1] << 2)), *(int *)((char *)ys + (p[1] << 2)), lv[*(unsigned char *)(k + (int)tb)], 1, 8, 8, 8, -1);
        p += 2;
        k++;
        if (k < 6) goto loop;
        v = r;
    }
    return v;
}

int AllocGlyphRows(int a) {
    int sz = GlyphRowCount * 28;
    GlyphRows = (MenuGlyphRow1C *)a;
    PsyqBzero(a, sz);
    DebugPrintStub(SizeLdFmt, sz);
    return sz;
}

void InitGlyphRow(int a0, int a1, int a2, int a3) {
  GlyphPreset14 *s;
  MenuGlyphRow1C *new_var;
  MenuGlyphRow1C *q;
  if (a0 < GlyphRowCount)
  {
    ((MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows))->control02 = a1;
    ((MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows))->glyph07 = a2;
    s = &D_8009F9D4[a3];
    new_var = (MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows);
    q = new_var;
    q->x08 = s->x00;
    q->y0A = s->y02;
    q->w0C = s->w04;
    q->h0E = s->h06;
    q->pad01 = s->f0E;
    ((MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows))->pad03[2] = s->f0F;
    ((MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows))->pad03[3] = s->f10;
    q = (MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows);
    ((short *)q->pad10)[0] = s->page08;
    ((short *)q->pad10)[1] = s->clut0A;
    q->pad10[4] = s->u0C;
    ((MenuGlyphRow1C *)((a0 * 0x1C) + (int)GlyphRows))->pad10[5] = s->v0D;
  }
}
