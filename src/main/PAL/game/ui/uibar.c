#include "rr/ui_bar.h"
#include "rr/ui_panel.h"

typedef struct 
{
  short a;
  short b;
  short c;
  short d;
  short e;
  short f;
  short g;
  short h;
  short i;
  char pad[10];
} E;

extern int D_800F2ECC;

void func_8003D228();
RrSpritePacket *func_8003D2C4(u32 *order,RrSpritePacket *cursor,s32 x,s32 y,s32 style);
void func_8003D474(s32 index);

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC ((E *)D_800F3AFC)

void func_8003D228(a0, a1, a2, a3, a4, a5, a6)
int a0;
int a1;
short a2;
int a3;
unsigned short a4;
unsigned short a5;
unsigned short a6;
{
  int s;
  unsigned int z;
  if (a0 < D_800F2ECC)
  {
    D_800F3AFC[a0].a = a6;
    D_800F3AFC[a0].b = 0;
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
    D_800F3AFC[a0].d = a2 - a6;
    D_800F3AFC[a0].f = a2 - a6;
    D_800F3AFC[a0].c = a1;
    D_800F3AFC[a0].e = a1;
    D_800F3AFC[a0].g = a3;
    D_800F3AFC[a0].h = a4 + a6;
    D_800F3AFC[a0].i = a5;
  }
}

#undef D_800F3AFC

/* legacy typing of shared globals in this part of the unit */
#define func_8001FF84 (*(RrSpritePacket *(*)(u32*,RrSpritePacket*,s32,s32,RrUiUvRect*,u16,s32))&func_8001FF84)
#define func_8001FEC0 (*(RrSpritePacket *(*)(u32*,RrSpritePacket*,s32,s32,s32,s32,s32,s32,s32))&func_8001FEC0)

RrSpritePacket *func_8003D2C4(u32 *order,RrSpritePacket *cursor,s32 x,s32 y,s32 style) {
    /* ASM_BARRIER: keep the ordering pointer live across every helper call. */
    register u32 *ordering __asm__("$21")=order;
    s32 xpos,ypos,offset,index;
    /* ASM_BARRIER: retain the captured descriptor across live count reloads. */
    register RrUiBarDescriptor *descriptor __asm__("$18");
    /* ASM_BARRIER: stage each horizontal coordinate in the outgoing argument. */
    register s32 x_stage __asm__("$6")=x;
    /* ASM_BARRIER: stage each vertical coordinate in the outgoing argument. */
    register s32 y_stage __asm__("$7")=y;
    u8 *descriptor_base;
    /* ASM_BARRIER: form the descriptor byte offset in the stack-argument temporary. */
    register s32 selector __asm__("$2");
    s32 width,sampled;
    u32 *out_order;
    RrSpritePacket *out_cursor;
    /* ASM_BARRIER: capture the saved ordering pointer before position setup. */
    __asm__("" : "+r"(x_stage) : "r"(ordering));
    xpos=x_stage;
    x_stage=(s32)((u32)xpos+6U);
    ypos=y_stage;
    y_stage=(s32)((u32)ypos+5U);
    offset=4;
    index=0;
    descriptor_base=(u8*)D_8009F968;
    selector=(u32)style*sizeof(RrUiBarDescriptor);
    descriptor=(RrUiBarDescriptor*)(descriptor_base+selector);
    cursor=func_8001FF84(ordering,cursor,x_stage,y_stage,
                       &D_8009F94C[descriptor->glyph],0x7FFB,0);
    out_order=ordering;
    out_cursor=cursor;
    x_stage=xpos;
    y_stage=ypos;
    /* ASM_BARRIER: consume the returned cursor before reusing v0 for width. */
    __asm__("" : "+r"(out_order), "+r"(out_cursor), "+r"(x_stage), "+r"(y_stage));
    width=offset;
    cursor=func_8001FEC0(out_order,out_cursor,x_stage,y_stage,width,24,20,72,0x7E3B);
    if(descriptor->count>0) {
        do {
            cursor=func_8001FEC0(ordering,cursor,(s32)((u32)xpos+(u32)offset),
                                ypos,8,24,24,72,0x7E3B);
            sampled=descriptor->count;
            /* ASM_BARRIER: sample the updated count before incrementing the index. */
            __asm__("" : "+r"(sampled), "+r"(index));
            index++;
            offset+=8;
        }while(index<sampled);
    }
    cursor=func_8001FEC0(ordering,cursor,(s32)((u32)xpos+(u32)offset),
                        ypos,8,24,32,72,0x7E3B);
    out_order=ordering;
    out_cursor=cursor;
    x_stage=(s32)((u32)xpos+4U);
    y_stage=(s32)((u32)ypos+24U);
    /* ASM_BARRIER: finish the final helper arguments before computing underline width. */
    __asm__("" : "+r"(out_order), "+r"(out_cursor), "+r"(x_stage), "+r"(y_stage) : : "$2");
    width=offset-4;
    return func_8001FD20(out_order,out_cursor,x_stage,y_stage,width,2,255,255,255);
}

#undef func_8001FF84
#undef func_8001FEC0

void func_8003D474(s32 index) {
    /* ASM_BARRIER: retain the selected index across helper and callback calls. */
    register s32 which asm("$23")=index;
    s32 high_raw;
    s32 offset=which*sizeof(RrBadgeRow1C);
    RrBadgeRow1C *row=(RrBadgeRow1C *)((unsigned long)offset+(unsigned long)D_800F3AFC);
    u32 raw;
    s32 status;
    /* ASM_BARRIER: retain the byte offset for the two fresh table captures. */
    __asm__("" : "+r"(offset));
    raw=row->status00;

    high_raw=(u32)raw<<16;

    status=high_raw>>16;
    if(status!=-1) {
        s32 y;
        s32 x;
        s32 width;
        u32 *order;
        RrPanelContext *context;
        s32 zero6;
        s32 zero7;
        s32 extent;
        s32 height;
        s32 callback;
        void *cursor;
        s32 white=255;
        RrGpuScratchPointers *scratch=(RrGpuScratchPointers *)0x1F800000U;
        zero6=0;

        zero7=zero6;
        y=row->y06;x=row->x04;width=row->width0C;extent=row->extent0E;
        
        context=(RrPanelContext *)D_800ACC14;
        
        order=&context->head74;
        height=extent-22;
        cursor=func_8001FC28(order,scratch->cursor,zero6,zero7,320,512);
        cursor=func_8001FCC8(order,cursor,37);
        callback=high_raw>>24;
        {
            void *call_cursor=cursor;
            
            if(callback)call_cursor=D_8009F984[callback-1](order,call_cursor,which);
            cursor=call_cursor;
        }
        cursor=func_8003D2C4(order,cursor,x,y,status&255);
        y=y+22;
        row=(RrBadgeRow1C *)((unsigned long)offset+(unsigned long)D_800F3AFC);
        cursor=func_8001FD20(order,cursor,x,row->y0A+height+20,width,2,white,white,white);
        cursor=func_8001FD20(order,cursor,x,y,width,2,white,white,white);
        cursor=func_8001FD20(order,cursor,x,y,1,height,white,white,white);
        cursor=func_8001FD20(order,cursor,x+width-1,y,1,height,white,white,white);
        white=74;
        cursor=func_8001FD20(order,cursor,x,y,width,height,white,white,white);
        cursor=func_8001FCC8(order,cursor,37);
        row=(RrBadgeRow1C *)((unsigned long)offset+(unsigned long)D_800F3AFC);
        scratch->cursor=func_8001FC28(order,cursor,row->x14,row->y16,row->w18,row->h1A);
    }
}
