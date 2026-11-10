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



typedef struct {
    short x00, y02, w04, h06;
    short page08, clut0A;
    char u0C, v0D, f0E, f0F, f10, pad11[3];
} GlyphPreset14;

extern BadgeRow1C *BadgeRows;
extern unsigned char SmallDigitSpriteUvs[];
extern unsigned char BigDigitGlyphWidths[];
extern MenuGlyphRow1C *GlyphRows;
extern GlyphPreset14 AnimatedGlyphPresets[];

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

int AllocRect16Rows(void *a0) {
    int n = Rect16RowCount << 4;
    Rect16Rows = a0;
    PsyqBzero(a0, n);
    DebugPrintStub(StrSizeEqLdFmt, n);
    return n;
}

void InitRect16Row(int i, short a, short b, short c, short d, short e, short f) {
    if (i < Rect16RowCount) {
        /* Numeric byte addressing still constrains the PAL allocation.
         * Keep the complete CPU address on native builds. */
        MenuRectRow10 *p = (MenuRectRow10 *)((i * 16)+(unsigned long)Rect16Rows);
        p->parameter00 = f;
        p->restart02 = 0;
        p->x = a;
        p->y = b;
        p->width = c;
        p->height = d;
        p->phase = e;
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
    MenuRectRow10 *e0, *e1, *e2, *e3, *e4, *e5;
    unsigned char *p;
    int *q;
    int *q2;
    int *xs, *ys;
    unsigned char *tb;

    v = a0 < Rect16RowCount;
    if (v != 0) {
        r = 0;
        e0 = (MenuRectRow10 *)((a0 * 16)+(unsigned long)Rect16Rows);
        a1 = e0->phase;
        a3 = e0->tick;
        k = 3;
        q = &lv[3];
        do {
            *q = 0;
            k--;
            q--;
        } while (k >= 0);
        e1 = (MenuRectRow10 *)((a0 * 16)+(unsigned long)Rect16Rows);
        lx[0] = e1->x;
        ly[0] = e1->y;
        lx[1] = e1->x + e1->width - 1;
        ly[1] = e1->y;
        lx[2] = e1->x;
        ly[2] = e1->y + e1->height - 1;
        lx[3] = e1->x + e1->width - 1;
        ly[3] = e1->y + e1->height - 1;
        switch (a1) {
        case 0:
            e2 = (MenuRectRow10 *)((a0 * 16)+(unsigned long)Rect16Rows);
            e2->phase = 1;
            e2->tick = 0;
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
                e3 = (MenuRectRow10 *)((a0 * 16)+(unsigned long)Rect16Rows);
                e3->tick = e3->tick + 1;
            } else {
                e4 = (MenuRectRow10 *)((a0 * 16)+(unsigned long)Rect16Rows);
                e4->tick = 0;
                e4->phase = e4->phase + 1;
            }
            break;
        case 8:
            e5 = (MenuRectRow10 *)((a0 * 16)+(unsigned long)Rect16Rows);
            if (e5->restart02 != 0) {
                e5->phase = 1;
            }
            break;
        default:
            break;
        }
        k = 0;
        xs = lx;
        ys = ly;
        tb = BigDigitGlyphWidths;
        p = SmallDigitSpriteUvs;
    loop:
        AddPartialLine(&CurrentDrawBuf->buffer.ordering70.words[0], *(int *)((char *)xs + (p[0] << 2)), *(int *)((char *)ys + (p[0] << 2)), *(int *)((char *)xs + (p[1] << 2)), *(int *)((char *)ys + (p[1] << 2)), lv[*(u8 *)(k+(unsigned long)tb)], 1, 8, 8, 8, -1);
        p += 2;
        k++;
        if (k < 6) goto loop;
        v = r;
    }
    return v;
}

int AllocGlyphRows(void *a) {
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
    (&GlyphRows[a0])->control02 = a1;
    (&GlyphRows[a0])->glyph07 = a2;
    s = &AnimatedGlyphPresets[a3];
    new_var = (MenuGlyphRow1C *)((a0 * 0x1C) + (unsigned long)GlyphRows);
    q = new_var;
    q->x08 = s->x00;
    q->y0A = s->y02;
    q->w0C = s->w04;
    q->h0E = s->h06;
    q->pad01 = s->f0E;
    (&GlyphRows[a0])->pad03[2] = s->f0F;
    (&GlyphRows[a0])->pad03[3] = s->f10;
    q = (MenuGlyphRow1C *)((a0 * 0x1C) + (unsigned long)GlyphRows);
    ((short *)q->pad10)[0] = s->page08;
    ((short *)q->pad10)[1] = s->clut0A;
    q->pad10[4] = s->u0C;
    (&GlyphRows[a0])->pad10[5] = s->v0D;
  }
}
