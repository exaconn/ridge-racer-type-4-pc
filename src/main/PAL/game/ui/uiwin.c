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



int UpdateGlyphRow(int arg0);
void InitUiRow(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...);
void DrawUiWindowRowText(s32 arg0, s32 arg1, s32 arg2);

int UpdateGlyphRow(int index) {
    SpriteEffectSource captionSprite;
    SpriteEffectSource imageSprite;
    MenuGlyphRow1C *initialRows;
    int initialControl;
    unsigned int initialTick;
    int shade;
    void *cursor;

    initialRows = GlyphRows;
    imageSprite.page = captionSprite.page = initialRows[index].texturePage10;
    imageSprite.clut = captionSprite.clut = initialRows[index].clut12;
    imageSprite.x = captionSprite.x = initialRows[index].x08;
    imageSprite.y = captionSprite.y = initialRows[index].y0A;
    captionSprite.y += initialRows[index].h0E / 2 - 10;
    imageSprite.uv.u = captionSprite.uv.u = initialRows[index].u14;
    imageSprite.uv.w = captionSprite.uv.w = initialRows[index].w0C;
    captionSprite.uv.v = initialRows[index].v15 + initialRows[index].h0E;
    captionSprite.uv.h = 0x10;
    imageSprite.uv.v = initialRows[index].v15;
    imageSprite.uv.h = initialRows[index].h0E;

    initialControl = initialRows[index].control02;
    initialTick = initialRows[index].tick03;

    switch (initialControl) {
    case MENU_GLYPH_RESET:
        GlyphRows[index].tick03 = 0;
        GlyphRows[index].control02 = MENU_GLYPH_INIT_RECTANGLE;
        GlyphRows[index].frameIndex00 = 0;
        break;
    case MENU_GLYPH_IDLE:
        break;
    case MENU_GLYPH_INIT_RECTANGLE:
        InitRect16Row(GlyphRows[index].glyph07, GlyphRows[index].x08, GlyphRows[index].y0A, GlyphRows[index].w0C, GlyphRows[index].h0E, 0, 0);
        GlyphRows[index].control02 = MENU_GLYPH_WAIT_RECTANGLE;
        break;
    case MENU_GLYPH_WAIT_RECTANGLE:
        if (UpdateRect16Row(GlyphRows[index].glyph07) != 0) {
            GlyphRows[index].control02 = MENU_GLYPH_FADE_BACKGROUND;
        }
        break;
    case MENU_GLYPH_START_RECTANGLE_DELAY:
        GlyphRows[index].tick03 = 0;
        GlyphRows[index].control02 = MENU_GLYPH_RECTANGLE_DELAY;
    case MENU_GLYPH_RECTANGLE_DELAY:
        UpdateRect16Row(GlyphRows[index].glyph07);
        if (GlyphRows[index].tick03 >= 0x14) {
            GlyphRows[index].control02 = MENU_GLYPH_FADE_BACKGROUND;
            GlyphRows[index].tick03 = 0;
        } else {
            GlyphRows[index].tick03++;
        }
        break;
    case MENU_GLYPH_FADE_BACKGROUND:
        cursor = ((GpuScratchPointers *)SCRATCHPAD)->cursor;
        shade = ((unsigned short)initialTick * 0x7C) / 10;
        cursor = AddTileSemiTransPrim(&CurrentDrawBuf->buffer.ordering70.words[1],
                                      cursor, GlyphRows[index].x08, GlyphRows[index].y0A,
                                      GlyphRows[index].w0C, GlyphRows[index].h0E,
                                      shade, shade, shade);
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddDrawModePrim(
            &CurrentDrawBuf->buffer.ordering70.words[1], cursor, 0x45);
        if ((unsigned short)initialTick >= 10) {
            GlyphRows[index].control02 = MENU_GLYPH_PREPARE_IMAGE;
            GlyphRows[index].tick03 = 0;
        } else {
            GlyphRows[index].tick03++;
        }
        UpdateRect16Row(GlyphRows[index].glyph07);
        break;
    case MENU_GLYPH_PREPARE_IMAGE:
        cursor = ((GpuScratchPointers *)SCRATCHPAD)->cursor;
        if ((unsigned short)initialTick >= 3) {
            cursor = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, &captionSprite, 1, 0xFF, 3);
        }
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddTilePrim(
            &CurrentDrawBuf->buffer.ordering70.words[1],
            cursor,
            GlyphRows[index].x08,
            GlyphRows[index].y0A,
            GlyphRows[index].w0C,
            GlyphRows[index].h0E,
            8, 8, 8
        );
        if ((unsigned short)initialTick >= 0x15) {
            GlyphRows[index].control02 = MENU_GLYPH_BLEND_IMAGE;
            GlyphRows[index].tick03 = 0;
        } else {
            GlyphRows[index].tick03++;
        }
        if (initialTick == 1) {
            LoadTimImage((int *)(GlyphRows[index].imageBase18 + GlyphRows[index].imageSectors06 * (s8)GlyphRows[index].frameIndex00 * 2048));
        }
        break;
    case MENU_GLYPH_BLEND_IMAGE:
        cursor = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], ((GpuScratchPointers *)SCRATCHPAD)->cursor, &captionSprite, 1, (0x80 - initialTick) * 2, 2);
        cursor = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, &imageSprite, 1, initialTick, 1);
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddTilePrim(
            &CurrentDrawBuf->buffer.ordering70.words[3],
            cursor,
            GlyphRows[index].x08,
            GlyphRows[index].y0A,
            GlyphRows[index].w0C,
            GlyphRows[index].h0E,
            8, 8, 8
        );
        if (initialTick >= 0x80) {
            GlyphRows[index].tick03 = 0;
            GlyphRows[index].control02 = MENU_GLYPH_DISPLAY_IMAGE;
        } else {
            GlyphRows[index].tick03 += 8;
        }
        break;
    case MENU_GLYPH_DISPLAY_IMAGE:
    default:
        cursor = ((GpuScratchPointers *)SCRATCHPAD)->cursor;
        if ((s8)GlyphRows[index].frameIndex00 >= 0) {
            cursor = AddSpriteWithDrawMode(
                &CurrentDrawBuf->buffer.ordering70.words[0],
                cursor,
                GlyphRows[index].x08,
                GlyphRows[index].y0A,
                GlyphRows[index].w0C,
                GlyphRows[index].h0E,
                GlyphRows[index].u14,
                GlyphRows[index].v15,
                GlyphRows[index].texturePage10,
                GlyphRows[index].clut12,
                0,
                0x80
            );
        }
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddTilePrim(
            &CurrentDrawBuf->buffer.ordering70.words[1],
            cursor,
            GlyphRows[index].x08,
            GlyphRows[index].y0A,
            GlyphRows[index].w0C,
            GlyphRows[index].h0E,
            8, 8, 8
        );
        if (initialTick >= GlyphRows[index].frameDuration05 && (s8)GlyphRows[index].frameCount01 >= 2) {
            GlyphRows[index].tick03 = 0;
            GlyphRows[index].control02 = MENU_GLYPH_PREPARE_IMAGE;
            GlyphRows[index].frameIndex00 = (s8)(++GlyphRows[index].frameIndex00) % (s8)GlyphRows[index].frameCount01;
        } else {
            GlyphRows[index].tick03++;
        }
        break;
    }

    return initialControl >= 0;
}

int AllocUiRows(void *a) {
    int sz = UiRowCount * 36;
    UiRows = (UiRow24 *)a;
    PsyqBzero(a, sz);
    DebugPrintStub(SizeLdFmt, sz);
    return sz;
}


void InitUiRow(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...) {
 s32 i;
 u32 offset;
 UiRow24 *tail;
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

   row->half04=a;
   row->half06=b;
   row->half08=c;
   row->half0A=d;
  }
  ((UiRow24 *)(offset+(unsigned long)tail))->byte17=0;
  i=0;
  ((UiRow24 *)(offset+(unsigned long)((UiRowPointerPage *)page)->rows4D80))->half20=0;
  if(count>0) {
   u8 *loop_page=page;
   u32 clear_offset;
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
