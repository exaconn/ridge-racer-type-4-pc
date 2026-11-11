#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/ui_bar.h"
#include "rr/ui_panel.h"
#include "game/ui/uidraw.h"

SpritePacket *DrawBadgeTitleBar(u32 *order,SpritePacket *cursor,s32 x,s32 y,s32 style);

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

SpritePacket *DrawBadgeTitleBar(u32 *order,SpritePacket *cursor,s32 x,s32 y,s32 style) {
    u32 *ordering=order;
    u32 xpos;
    s32 ypos, offset, index;
    UiBarDescriptor *descriptor;
    s32 x_stage=x;
    s32 y_stage=y;
    u8 *descriptor_base;
    s32 selector;
    s32 width,sampled;
    u32 *out_order;
    SpritePacket *out_cursor;
    xpos=x_stage;
    x_stage=(xpos+6U);
    ypos=y_stage;
    y_stage=(ypos+5U);
    offset=4;
    index=0;
    descriptor_base=(u8*)SelectionBarDescriptors;
    selector=style*sizeof(UiBarDescriptor);
    descriptor=(UiBarDescriptor*)(descriptor_base+selector);
    cursor=AddSpritePrim(ordering,cursor,x_stage,y_stage,
                       &SelectionBarGlyphUvs[descriptor->glyph],0x7FFB,0);
    out_order=ordering;
    out_cursor=cursor;
    x_stage=xpos;
    y_stage=ypos;
    width=offset;
    cursor=GameQueueSprite(out_order,out_cursor,x_stage,y_stage,width,24,20,72,0x7E3B);
    if(descriptor->count>0) {
        do {
            cursor=GameQueueSprite(ordering,cursor,(xpos+offset),
                                ypos,8,24,24,72,0x7E3B);
            sampled=descriptor->count;
            index++;
            offset+=8;
        }while(index<sampled);
    }
    cursor=GameQueueSprite(ordering,cursor,(xpos+offset),
                        ypos,8,24,32,72,0x7E3B);
    out_order=ordering;
    out_cursor=cursor;
    x_stage=(xpos+4U);
    y_stage=(ypos+24U);
    width=offset-4;
    return AddTilePrim(out_order,out_cursor,x_stage,y_stage,width,2,255,255,255);
}


void DrawBadgeRow(s32 index) {
    s32 y;
    s32 x;
    s32 width;
    s32 extent;
    s32 height;
    u32 *order;
    s32 white;
    s32 status;
    s32 high_raw;
    void *cursor;

    high_raw = (u32)BadgeRows[index].status00 << 16;
    status = high_raw >> 16;
    if (status != -1) {
        GpuScratchPointers *scratch = (GpuScratchPointers *)SCRATCHPAD;
        white = 255;
        y = BadgeRows[index].y06;
        x = BadgeRows[index].x04;
        width = BadgeRows[index].width0C;
        extent = BadgeRows[index].extent0E;
        height = extent - 22;
        order = &CurrentDrawBuf->buffer.ordering70.words[1];
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
