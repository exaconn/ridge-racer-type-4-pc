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
    /* ASM_BARRIER: keep the ordering pointer live across every helper call. */
    register u32 *ordering __asm__("$21")=order;
    u32 xpos;
    s32 ypos, offset, index;
    /* ASM_BARRIER: retain the captured descriptor across live count reloads. */
    register UiBarDescriptor *descriptor __asm__("$18");
    /* ASM_BARRIER: stage each horizontal coordinate in the outgoing argument. */
    register s32 x_stage __asm__("$6")=x;
    /* ASM_BARRIER: stage each vertical coordinate in the outgoing argument. */
    register s32 y_stage __asm__("$7")=y;
    u8 *descriptor_base;
    /* ASM_BARRIER: form the descriptor byte offset in the stack-argument temporary. */
    register s32 selector __asm__("$2");
    s32 width,sampled;
    u32 *out_order;
    SpritePacket *out_cursor;
    /* ASM_BARRIER: capture the saved ordering pointer before position setup. */
    __asm__("" : "+r"(x_stage) : "r"(ordering));
    xpos=x_stage;
    x_stage=(xpos+6U);
    ypos=y_stage;
    y_stage=(ypos+5U);
    offset=4;
    index=0;
    descriptor_base=(u8*)D_8009F968;
    selector=style*sizeof(UiBarDescriptor);
    descriptor=(UiBarDescriptor*)(descriptor_base+selector);
    cursor=AddSpritePrim(ordering,cursor,x_stage,y_stage,
                       &D_8009F94C[descriptor->glyph],0x7FFB,0);
    out_order=ordering;
    out_cursor=cursor;
    x_stage=xpos;
    y_stage=ypos;
    /* ASM_BARRIER: consume the returned cursor before reusing v0 for width. */
    __asm__("" : "+r"(out_order), "+r"(out_cursor), "+r"(x_stage), "+r"(y_stage));
    width=offset;
    cursor=GameQueueSprite(out_order,out_cursor,x_stage,y_stage,width,24,20,72,0x7E3B);
    if(descriptor->count>0) {
        do {
            cursor=GameQueueSprite(ordering,cursor,(xpos+offset),
                                ypos,8,24,24,72,0x7E3B);
            sampled=descriptor->count;
            /* ASM_BARRIER: sample the updated count before incrementing the index. */
            __asm__("" : "+r"(sampled), "+r"(index));
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
    /* ASM_BARRIER: finish the final helper arguments before computing underline width. */
    __asm__("" : "+r"(out_order), "+r"(out_cursor), "+r"(x_stage), "+r"(y_stage) : : "$2");
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
                call_cursor = D_8009F984[(high_raw >> 24) - 1](order, call_cursor, index);
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
