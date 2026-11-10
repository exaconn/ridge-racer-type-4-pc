/* RODATA 0x800108e8 0x3c */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "scratchpad.h"
#include "rr/ui.h"
#include "game/ui/uitext.h"
#include "game/ui/uidraw.h"
#include "game/ui/uiwin.h"
#include "rr/menu_state_types.h"
#include "rr/bios_devices.h"
#include "rr/audio_cd_controller.h"
#include "rr/sprite_effect.h"
#include "game/ui/badge1.h"
#include "rr/badge_rows.h"
#include "game/ui/badgespr.h"
#include "rr/menu_setup.h"
#include "game/course/trkintp2.h"
#include "rr/trig.h"
#include "game/gfx/sprefct.h"

typedef struct {
    short x, y;
    unsigned short page, clut;
    unsigned char u, v, w, h;
} SpriteDesc;

extern MenuGlyphRow1C *GlyphRows;

int UpdateGlyphRow(int arg0);
void InitUiRow(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...);
void DrawUiWindowRowText(s32 arg0, s32 arg1, s32 arg2);

int UpdateGlyphRow(int arg0) {
    SpriteDesc sp30;
    SpriteDesc sp40;
    MenuGlyphRow1C *base;
    MenuGlyphRow1C *p;
    int s7;
    unsigned int s5;
    int v0;
    void *a1;

    base = GlyphRows;
    p = (MenuGlyphRow1C *)((arg0 * 0x1C) + (int)base);
    sp40.page = sp30.page = ((unsigned short *)p->pad10)[0];
    sp40.clut = sp30.clut = ((unsigned short *)p->pad10)[1];
    sp40.x = sp30.x = p->x08;
    sp40.y = sp30.y = p->y0A;
    sp30.y += p->h0E / 2 - 10;
    sp40.u = sp30.u = p->pad10[4];
    sp40.w = sp30.w = p->w0C;
    sp30.v = p->pad10[5] + p->h0E;
    sp30.h = 0x10;
    sp40.v = p->pad10[5];
    sp40.h = p->h0E;

    s7 = p->control02;
    s5 = p->pad03[0];

    switch (s7) {
    case -2:
        GlyphRows[arg0].pad03[0] = 0;
        GlyphRows[arg0].control02 = -4;
        GlyphRows[arg0].flags00 = 0;
        break;
    case -3:
        break;
    case -4:
        InitRect16Row(GlyphRows[arg0].glyph07, GlyphRows[arg0].x08, GlyphRows[arg0].y0A, GlyphRows[arg0].w0C, GlyphRows[arg0].h0E, 0, 0);
        GlyphRows[arg0].control02 = -5;
        break;
    case -5:
        if (UpdateRect16Row(GlyphRows[arg0].glyph07) != 0) {
            GlyphRows[arg0].control02 = -1;
        }
        break;
    case -6:
        GlyphRows[arg0].pad03[0] = 0;
        GlyphRows[arg0].control02 = -7;
    case -7:
        UpdateRect16Row(GlyphRows[arg0].glyph07);
        if (GlyphRows[arg0].pad03[0] >= 0x14) {
            GlyphRows[arg0].control02 = -1;
            GlyphRows[arg0].pad03[0] = 0;
        } else {
            GlyphRows[arg0].pad03[0]++;
        }
        break;
    case -1:
        a1 = *(void **)SCRATCHPAD;
        v0 = ((unsigned short)s5 * 0x7C) / 10;
        *(void **)SCRATCHPAD = (void *)AddDrawModePrim(
            &CurrentDrawBuf->buffer.ordering70.words[1],
            AddTileSemiTransPrim(&CurrentDrawBuf->buffer.ordering70.words[1], a1, GlyphRows[arg0].x08, GlyphRows[arg0].y0A, GlyphRows[arg0].w0C, GlyphRows[arg0].h0E, v0, v0, v0),
            0x45
        );
        if ((unsigned short)s5 >= 10) {
            GlyphRows[arg0].control02 = 0;
            GlyphRows[arg0].pad03[0] = 0;
        } else {
            GlyphRows[arg0].pad03[0]++;
        }
        UpdateRect16Row(GlyphRows[arg0].glyph07);
        break;
    case 0:
        a1 = *(void **)SCRATCHPAD;
        if ((unsigned short)s5 >= 3) {
            a1 = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], a1, &sp30, 1, 0xFF, 3);
        }
        *(void **)SCRATCHPAD = AddTilePrim(
            &CurrentDrawBuf->buffer.ordering70.words[1],
            a1,
            GlyphRows[arg0].x08,
            GlyphRows[arg0].y0A,
            GlyphRows[arg0].w0C,
            GlyphRows[arg0].h0E,
            8, 8, 8
        );
        if ((unsigned short)s5 >= 0x15) {
            GlyphRows[arg0].control02 = 1;
            GlyphRows[arg0].pad03[0] = 0;
        } else {
            GlyphRows[arg0].pad03[0]++;
        }
        if (s5 == 1) {
            LoadTimImage(GlyphRows[arg0].phase18 + ((GlyphRows[arg0].pad03[3] * (s8)GlyphRows[arg0].flags00) << 11));
        }
        break;
    case 1:
        a1 = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], *(void **)SCRATCHPAD, &sp30, 1, (0x80 - s5) * 2, 2);
        a1 = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], a1, &sp40, 1, s5, 1);
        *(void **)SCRATCHPAD = AddTilePrim(
            &CurrentDrawBuf->buffer.ordering70.words[3],
            a1,
            GlyphRows[arg0].x08,
            GlyphRows[arg0].y0A,
            GlyphRows[arg0].w0C,
            GlyphRows[arg0].h0E,
            8, 8, 8
        );
        if (s5 >= 0x80) {
            GlyphRows[arg0].pad03[0] = 0;
            GlyphRows[arg0].control02 = 2;
        } else {
            GlyphRows[arg0].pad03[0] += 8;
        }
        break;
    case 2:
    default:
        a1 = *(void **)SCRATCHPAD;
        if ((s8)GlyphRows[arg0].flags00 >= 0) {
            a1 = AddSpriteWithDrawMode(
                &CurrentDrawBuf->buffer.ordering70.words[0],
                a1,
                GlyphRows[arg0].x08,
                GlyphRows[arg0].y0A,
                GlyphRows[arg0].w0C,
                GlyphRows[arg0].h0E,
                GlyphRows[arg0].pad10[4],
                GlyphRows[arg0].pad10[5],
                ((unsigned short *)GlyphRows[arg0].pad10)[0],
                ((unsigned short *)GlyphRows[arg0].pad10)[1],
                0,
                0x80
            );
        }
        *(void **)SCRATCHPAD = AddTilePrim(
            &CurrentDrawBuf->buffer.ordering70.words[1],
            a1,
            GlyphRows[arg0].x08,
            GlyphRows[arg0].y0A,
            GlyphRows[arg0].w0C,
            GlyphRows[arg0].h0E,
            8, 8, 8
        );
        if (s5 >= GlyphRows[arg0].pad03[2] && (s8)GlyphRows[arg0].pad01 >= 2) {
            GlyphRows[arg0].pad03[0] = 0;
            GlyphRows[arg0].control02 = 0;
            GlyphRows[arg0].flags00 = (s8)(++GlyphRows[arg0].flags00) % (s8)GlyphRows[arg0].pad01;
        } else {
            GlyphRows[arg0].pad03[0]++;
        }
        break;
    }

    return s7 >= 0;
}

int AllocUiRows(int a) {
    int sz = UiRowCount * 36;
    UiRows = (UiRow24 *)a;
    PsyqBzero(a, sz);
    DebugPrintStub(SizeLdFmt, sz);
    return sz;
}


void InitUiRow(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...) {
 register s32 i;
 u32 offset;
 register UiRow24 *tail;
 /* ASM_BARRIER: Keep the real PAL pointer page in its shared address register. */
 register u8 *page __asm__("$9");
 if(index<UiRowCount) {
  /* Pointer storage is re-read after byte/halfword stores that can alias it. */
  page=(u8*)0x800F0000U;
  /* Preserve PAL offset wrapping while widening the signed offset for host pointers. */
  offset=index*sizeof(UiRow24);
  ((UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80))->byte15=g;
  ((UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80))->byte16=h;
  ((UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80))->byte01=f;
  ((UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80))->byte14=e;
  {
   UiRow24 *row=(UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80);
   row->byte02=count;
   /* Capture this fresh base before the halfword stores, as in the original. */
   tail=((UiRowPointerPage *)page)->rows4D80;

   row->half04=a;row->half06=b;row->half08=c;row->half0A=d;
  }
  ((UiRow24 *)(offset+(unsigned long)tail))->byte17=0;
  i=0;
  ((UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80))->half20=0;
  if(count>0) {
   register u8 *loop_page=page;
   register u32 clear_offset;
   clear_offset=offset;
   do {
    *(u8 *)(clear_offset+(unsigned long)((UiRowPointerPage *)loop_page)->rows4D80+i+RR_FIELD_OFFSET(UiRow24,cells))=0;
    i++;
   }while(i<count);
  }
 }
}

void DrawUiWindowRowText(s32 arg0, s32 arg1, s32 arg2) {
    s32 pad[8];
    s32 s0;
    s32 s1;
    s32 s3;
    s32 s5;
    s32 s6;
    s32 a0;
    s32 v;
    s32 t;
    s32 b;
    s32 d;
    s32 c;
    UiRow24 *p;

    if (UiRows[arg0].byte02 == 0) {
        *(s32 *)SCRATCHPAD = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], *(s32 *)SCRATCHPAD, arg1, arg2, 1, UiRows[arg0].half0A, 0xFF, 0xFF, 0xFF);
    } else {
        s1 = 0;
        s3 = *(s32 *)SCRATCHPAD;
        s5 = ((UiRows[arg0].byte17 - 0x80) << 4) & 0xFFF;
        if ((b = UiRows[arg0].byte02) != 0) {
            do {
                switch ((s8)UiRows[arg0].cells[s1]) {
                default:
                case 0:
                    a0 = 0xC00;
                    s0 = 0;
                    break;
                case 2:
                    a0 = s5;
                    s0 = 0;
                    break;
                case 1:
                    a0 = 0xC00;
                    s0 = 3;
                    break;
                case 3:
                case 4:
                    a0 = s5;
                    s0 = 3;
                    break;
                }
                s6 = arg1 + s1 * 10 + 4;
                c = 0x1E;
                v = rsin(a0);
                t = s0 + 1;
                s0 = ((t + v / 0x801) << 3) + 0xD0;
                s3 = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.words[0], s3, s6, arg2 + 8, 8, 0x10, s0, 0x10, c, 0x7F3A, 0, 0x80);
                s1++;
            } while (s1 < UiRows[arg0].byte02);
        }
        UiRows[arg0].byte17 += 8;
        p = UiRows;
        /* ASM_BARRIER: Prevent sched1/sched2 from hoisting reloads above b17 += 8 store */
        __asm__ ("");
        d = arg1 - 12;
        b = p[arg0].byte17;
        /* ASM_BARRIER: Keep unused b17 load live and reload UiRows without a branch basic-block boundary */
        __asm__("" : "=m"(UiRows) : "r"(b), "r"(d));
        *(s32 *)SCRATCHPAD = s3;
        DrawUiBox(arg1, arg2, s6 - d, UiRows[arg0].half0A, 0xCE, 0xCE, 0xCE);
    }
}
