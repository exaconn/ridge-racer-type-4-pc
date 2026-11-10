#include "rr/gpu_line.h"
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/sprite_effect.h"
#include "rr/textured_triangle.h"
#include "rr/sprite_emit.h"
#include "psyq/libgpu/primatt2.h"
#include "psyq/libgpu/primatt3.h"
#include "psyq/libgpu/primatt6.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
#include "rr/animated_glyph.h"
#include "psyq/libc/rand.h"

struct P { int tag; unsigned char r, g, b, code; short x, y, w, h; };

/* extern */
/* extern */
/* extern */

u8 *AddJitterSprites(u32 *ordering,u8 *cursor,SpriteEffectSource *descriptor_arg,s32 semi,s32 strength_arg,s32 count_arg);
struct S32 *EmitPolyFT3(u32 *arg0, struct S32 *arg1, void *arg2, void *arg3, void *arg4, void *arg5, u16 arg6, u16 arg7, u16 arg8, u16 arg9);
void *AddTileSemiTransPrim(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
PolyF3Packet *EmitPolyF3(u32 *ordering,PolyF3Packet *packet,s32 xy0,s32 xy1,s32 xy2,const u8 *rgb,s32 flags);

u8 *AddJitterSprites(u32 *ordering,u8 *cursor,SpriteEffectSource *descriptor_arg,s32 semi,s32 strength_arg,s32 count_arg) {
    /* ASM_BARRIER: retain the loop bound in callee-saved S5 across helpers. */
    register s32 count asm("$21")=count_arg;
    /* ASM_BARRIER: retain the full signed intensity in callee-saved S7. */
    register s32 strength asm("$23")=strength_arg;
    register SpritePacket *sprite;
    /* ASM_BARRIER: retain the source in callee-saved S2 across helpers. */
    register SpriteEffectSource *descriptor asm("$18");
    register s32 i;
    /* ASM_BARRIER: use A1 for jitter only until the transparency helper. */
    register s32 jitter asm("$5");
    s32 y;
    s32 level;
    /* ASM_BARRIER: capture the incoming ordering pointer in A0. */
    register u32 *incoming_order asm("$4")=ordering;
    /* ASM_BARRIER: capture the incoming cursor in A1 before the saved copy. */
    register u8 *incoming_cursor asm("$5")=cursor;
    /* ASM_BARRIER: capture the incoming source in A2 before the saved copy. */
u32 *saved_ordering;
    /* ASM_BARRIER: capture stack arguments before copying the pointer arguments. */
    __asm__("" : "+r"(count), "+r"(strength), "+r"(incoming_order), "+r"(incoming_cursor), "+r"(descriptor_arg));
    saved_ordering=incoming_order;
    /* ASM_BARRIER: finish saving the ordering pointer before the other copies. */
    __asm__("" : "+r"(incoming_cursor), "+r"(descriptor_arg) : "r"(saved_ordering));
    sprite=(SpritePacket *)incoming_cursor;
    descriptor=descriptor_arg;
    /* ASM_BARRIER: finish the source copy before initializing the loop index. */
    __asm__("" : "+r"(descriptor));
    i=0;
    while(i<count) {
        SetSprt(sprite);
        jitter=0;
        if(count!=1) {
            u32 random=PsyqRand()&7;
            register s32 three=3;
            jitter=three-random;
            /* ASM_BARRIER: keep the subtraction input distinct from its A1 result. */
            __asm__("" : "+r"(jitter) : "r"(three));
        }
        /* ASM_BARRIER: use V0 for the half-jitter value only during geometry setup. */
        { register s32 temp asm("$2")=jitter>>1;

        if(i==0)y=descriptor->y+temp;
        else if(i==1)y=descriptor->y+jitter*3;
        else if(i==2)y=descriptor->y+jitter; }
        {s16 x=descriptor->x;
        sprite->y=y;
        sprite->x=x;}
        sprite->w=descriptor->uv.w;
        sprite->h=descriptor->uv.h;
        sprite->u=descriptor->uv.u;
        {u8 v=((SpriteEffectSource *)descriptor)->uv.v;
        s32 transparency=*(s32 *)&semi;
        sprite->v=v;
        SetSemiTrans(sprite,transparency);}
        level=strength;
        if(count!=1) {
            if(i==1)level=(s32)((u32)strength*(PsyqRand()&15))/24;
            else level=(s32)(strength*(((u32)PsyqRand()&31)+8))/48;
        }
        /* ASM_BARRIER: move intensity to A1 after division and before the RGB stores. */
        { register s32 color asm("$5")=level;  sprite->r=color;sprite->g=color;sprite->b=color; }
        sprite->clut=descriptor->clut;
        AddPrim(saved_ordering,(u32 *)sprite++);
        {
            u32 page;
            /* ASM_BARRIER: supply the advanced cursor in A1 to the mode helper. */
            register u8 *out_cursor asm("$5");
            if(i>0)page=(u8)descriptor->page|0x60;
            else page=descriptor->page;
            out_cursor=(u8 *)sprite;
            i++;
            sprite=(SpritePacket *)AddDrawModePrim((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]),(GpuDrawModePacket *)out_cursor,page);
        }
    }
    return (u8 *)sprite;
}


/* extern */
struct S32 *EmitPolyFT3(u32 *arg0, struct S32 *arg1, void *arg2, void *arg3, void *arg4, void *arg5, u16 arg6, u16 arg7, u16 arg8, u16 arg9) {
    SetPolyFT3(arg1);
    ((u16 *)arg1)[4] = (u16) ((u16 *)arg2)[0];
    ((u16 *)arg1)[5] = (u16) ((u16 *)arg3)[0];
    ((u16 *)arg1)[8] = (u16) ((u16 *)arg2)[2];
    ((u16 *)arg1)[9] = (u16) ((u16 *)arg3)[2];
    ((u16 *)arg1)[12] = (u16) ((u16 *)arg2)[4];
    ((u16 *)arg1)[13] = (u16) ((u16 *)arg3)[4];
    ((u8 *)arg1)[12] = (u8) ((u8 *)arg4)[0];
    ((u8 *)arg1)[13] = (u8) ((u8 *)arg5)[0];
    ((u8 *)arg1)[20] = (u8) ((u8 *)arg4)[4];
    ((u8 *)arg1)[21] = (u8) ((u8 *)arg5)[4];
    ((u8 *)arg1)[28] = (u8) ((u8 *)arg4)[8];
    ((u8 *)arg1)[29] = (u8) ((u8 *)arg5)[8];
    SetSemiTrans(arg1, arg8 & 0xFFFF);
    if ((arg9 & 0xFFFF) != 0x80) {
        SetShadeTex(arg1, 0);
        ((s8 *)arg1)[4] = (s8) arg9;
        ((s8 *)arg1)[5] = (s8) arg9;
        ((s8 *)arg1)[6] = (s8) arg9;
    } else {
        SetShadeTex(arg1, 1);
    }
    ((u16 *)arg1)[11] = arg6;
    ((u16 *)arg1)[7] = arg7;
    AddPrim(arg0, (u32 *)arg1++);
    return arg1;
}


void *AddTileSemiTransPrim(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    ((s8 *)arg1)[3] = 3;
    ((s16 *)arg1)[4] = arg2;
    ((s16 *)arg1)[6] = (s16) arg4;
    ((s8 *)arg1)[7] = 0x62;
    ((s16 *)arg1)[5] = arg3;
    ((s16 *)arg1)[7] = (s16) arg5;
    ((s8 *)arg1)[4] = (s8) arg6;
    ((s8 *)arg1)[5] = (s8) arg7;
    ((s8 *)arg1)[6] = (s8) arg8;
    ((s32 *)arg1)[0] = ((((s32 *)arg1)[0] & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x10;
}

GpuLineF2Packet *GameQueueTileTrans(u32 *ordering, GpuLineF2Packet *line, s32 x0, s32 y0, s32 x1, s32 y1, s32 red, s32 green, s32 blue) {
    SetLineF2(line);
    SetShadeTex(line, 1);
    line->x0 = x0;
    line->y0 = y0;
    line->x1 = x1;
    line->y1 = y1;
    line->r = red;
    line->g = green;
    line->b = blue;
    AddPrim(ordering, (u32 *)line++);
    return line;
}


GpuLineF2Packet *AddLineF2SemiTrans(u32 *ordering, GpuLineF2Packet *line, s32 x0, s32 y0, s32 x1, s32 y1, s32 red, s32 green, s32 blue) {
    SetLineF2(line);
    SetShadeTex(line, 1);
    line->x0 = x0;
    line->y0 = y0;
    line->code |= 2;
    line->x1 = x1;
    line->y1 = y1;
    line->r = red;
    line->g = green;
    line->b = blue;
    AddPrim(ordering, (u32 *)line++);
    return line;
}


PolyF3Packet *EmitPolyF3(u32 *ordering,PolyF3Packet *packet,s32 xy0,s32 xy1,s32 xy2,const u8 *rgb,s32 flags){
 SetPolyF3(packet);
 SetSemiTrans(packet,flags);
 packet->xy0=xy0;
 packet->xy1=xy1;
 packet->xy2=xy2;
 packet->r=rgb[0];
 packet->g=rgb[1];
 packet->b=rgb[2];
 AddPrim(ordering,(u32*)packet++);
 return packet;
}
