#include "rr/ui_bar.h"

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
extern E *D_800F3AFC;
/* Caller-facing O32 arguments retain full register words; the existing helpers
 * narrow coordinates and packet fields when writing the GPU representation. */
extern RrSpritePacket *func_8001FF84(u32*,RrSpritePacket*,s32,s32,RrUiUvRect*,u16,s32);
extern RrSpritePacket *func_8001FEC0(u32*,RrSpritePacket*,s32,s32,s32,s32,s32,s32,s32);
extern void *func_8001FD20(u32*,void*,s32,s32,s32,s32,s32,s32,s32);

void func_8003D228();
RrSpritePacket *func_8003D2C4(u32 *order,RrSpritePacket *cursor,s32 x,s32 y,s32 style);

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
