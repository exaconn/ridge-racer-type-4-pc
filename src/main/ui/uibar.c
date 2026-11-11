#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/ui_bar.h"
#include "rr/ui_panel.h"
#include "game/ui/uidraw.h"


void InitBadgeRow(s32 a0, s32 a1, s16 a2, s32 a3, u16 a4, u16 a5, u16 a6) {
  int s;
  unsigned int z;
  if (a0 < BadgeRowCount)
  {
    BadgeRows[a0].status00 = a6;
    BadgeRows[a0].half02 = 0;
    s = (short) a6;
    if (s < 0)
    {
      a6 = 0;
    }
    else
    {
      z = s == 0;
      a6 = 0x16;
      if (z)
      {
        a4 = 0x80;
      }
    }
    BadgeRows[a0].y06 = a2 - a6;
    BadgeRows[a0].y0A = a2 - a6;
    BadgeRows[a0].x04 = a1;
    BadgeRows[a0].x08 = a1;
    BadgeRows[a0].width0C = a3;
    BadgeRows[a0].extent0E = a4 + a6;
    BadgeRows[a0].state10 = a5;
  }
}

SpritePacket *DrawBadgeTitleBar(GpuDmaTag *ordering,SpritePacket *cursor,s32 x,s32 y,s32 style) {
    UiBarDescriptor *descriptor=&SelectionBarDescriptors[style];
    s32 offset=4;
    s32 index=0;
    cursor=AddSpritePrim(ordering,cursor,x+6,y+5,
                       &SelectionBarGlyphUvs[descriptor->glyph],0x7FFB,0);
    cursor=GameQueueSprite(ordering,cursor,x,y,offset,24,20,72,0x7E3B);
    if(descriptor->count>0) {
        do {
            cursor=GameQueueSprite(ordering,cursor,x+offset,
                                y,8,24,24,72,0x7E3B);
            index++;
            offset+=8;
        }while(index<descriptor->count);
    }
    cursor=GameQueueSprite(ordering,cursor,x+offset,
                        y,8,24,32,72,0x7E3B);
    return AddTilePrim(ordering,cursor,x+4,y+24,offset-4,2,255,255,255);
}


void DrawBadgeRow(s32 index) {
    s32 y;
    s32 x;
    s32 width;
    s32 extent;
    s32 height;
    GpuDmaTag *order;
    s32 white;
    s32 status;
    s32 high_raw;
    void *cursor;

    high_raw = (u32)BadgeRows[index].status00 << 16;
    status = high_raw >> 16;
    if (status != -1) {
        GpuScratchPointers *scratch = RR_GPU_SCRATCH;
        white = 255;
        y = BadgeRows[index].y06;
        x = BadgeRows[index].x04;
        width = BadgeRows[index].width0C;
        extent = BadgeRows[index].extent0E;
        height = extent - 22;
        order = &CurrentDrawBuf->buffer.ordering70.tags[1];
        cursor = AddDrawAreaPrim(order, scratch->cursor, 0, 0, 320, 512);
        cursor = AddDrawModePrim(order, cursor, 37);
        {
            void *call_cursor = cursor;
            if (high_raw >> 24) {
                call_cursor = UiRowDrawHandlers[(high_raw >> 24) - 1](order, call_cursor, index);
            }
            cursor = call_cursor;
        }
        cursor = DrawBadgeTitleBar(order, cursor, x, y, status & 255);
        y += 22;
        cursor = AddTilePrim(order, cursor, x, BadgeRows[index].y0A + height + 20, width, 2, white, white, white);
        cursor = AddTilePrim(order, cursor, x, y, width, 2, white, white, white);
        cursor = AddTilePrim(order, cursor, x, y, 1, height, white, white, white);
        cursor = AddTilePrim(order, cursor, x + width - 1, y, 1, height, white, white, white);
        white = 74;
        cursor = AddTilePrim(order, cursor, x, y, width, height, white, white, white);
        cursor = AddDrawModePrim(order, cursor, 37);
        scratch->cursor = AddDrawAreaPrim(order, cursor, BadgeRows[index].x14, BadgeRows[index].y16, BadgeRows[index].w18, BadgeRows[index].h1A);
    }
}

/* Initial values of the globals this module owns. */
#include "game/ui/uicnt.h"
#include "game/menu/mnlist.h"
#include "rr/menu_overlay_runtime.h"
UiUvRect SelectionBarGlyphUvs[7] = {{.u = 0x58, .v = 0x48, .w = 0x28, .h = 0x10}, {.u = 0x80, .v = 0x48, .w = 0x30, .h = 0x10}, {.u = 0xB4, .v = 0x48, .w = 0x39, .h = 0x10}, {.u = 0x58, .v = 0x58, .w = 0x28, .h = 0x10}, {.u = 0x80, .v = 0x58, .w = 0x24, .h = 0x10}, {.u = 0xA4, .v = 0x58, .w = 0x30, .h = 0x10}, {.u = 0xD4, .v = 0x58, .w = 0x28, .h = 0x10}};
UiBarDescriptor SelectionBarDescriptors[7] = {{.count = 6}, {.glyph = 1, .count = 7}, {.glyph = 2, .count = 8}, {.glyph = 3, .count = 6}, {.glyph = 4, .count = 6}, {.glyph = 5, .count = 6}, {.glyph = 6, .count = 6}};
PanelDraw UiRowDrawHandlers[15] = {(void *(*)(struct GpuDmaTag *, void *, int))DrawCarSpecBadgePanel, (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[28052], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[28564], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[29156], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[29656], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[30012], (void *(*)(struct GpuDmaTag *, void *, int))DrawGpSaveSlotsPanel, (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[30356], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[31096], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[23288], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[22380], (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[24196], (void *(*)(struct GpuDmaTag *, void *, int))DrawControllerConfigPanel, (void *(*)(struct GpuDmaTag *, void *, int))&DrawBuffers[1].buffer.packets1670[31488], (void *(*)(struct GpuDmaTag *, void *, int))DrawBadgeCountdownSprites};
