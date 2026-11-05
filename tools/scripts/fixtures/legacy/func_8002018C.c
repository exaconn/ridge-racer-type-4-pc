#include "rr/sprite_effect.h"
extern void func_80095B94(void *),func_80095AF4(void *,s32),AddPrim(u32 *,u32 *);
extern s32 func_80096464(void);
extern u8 *func_8001FCC8(u32 *,u8 *,u16);
u8 *func_8002018C(u32 *ordering,u8 *cursor,RrSpriteEffectSource *descriptor_arg,s32 semi,s32 strength_arg,s32 count_arg) {
    /* ASM_BARRIER: retain the loop bound in callee-saved S5 across helpers. */
    register s32 count asm("$21")=count_arg;
    /* ASM_BARRIER: retain the full signed intensity in callee-saved S7. */
    register s32 strength asm("$23")=strength_arg;
    register RrSpritePacket *sprite;
    /* ASM_BARRIER: retain the source in callee-saved S2 across helpers. */
    register RrSpriteEffectSource *descriptor asm("$18");
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
    register RrSpriteEffectSource *incoming_descriptor asm("$6")=descriptor_arg;
    u32 *saved_ordering;
    /* ASM_BARRIER: capture stack arguments before copying the pointer arguments. */
    __asm__("" : "+r"(count), "+r"(strength), "+r"(incoming_order), "+r"(incoming_cursor), "+r"(incoming_descriptor));
    saved_ordering=incoming_order;
    /* ASM_BARRIER: finish saving the ordering pointer before the other copies. */
    __asm__("" : "+r"(incoming_cursor), "+r"(incoming_descriptor) : "r"(saved_ordering));
    sprite=(RrSpritePacket *)incoming_cursor;
    descriptor=incoming_descriptor;
    /* ASM_BARRIER: finish the source copy before initializing the loop index. */
    __asm__("" : "+r"(descriptor));
    i=0;
    while(i<count) {
        func_80095B94(sprite);
        jitter=0;
        if(count!=1) {
            u32 random=(u32)func_80096464()&7;
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
        {u8 v=((volatile RrSpriteEffectSource *)descriptor)->uv.v;
        s32 transparency=*(volatile s32 *)&semi;
        sprite->v=v;
        func_80095AF4(sprite,transparency);}
        level=strength;
        if(count!=1) {
            if(i==1)level=(s32)((u32)strength*((u32)func_80096464()&15))/24;
            else level=(s32)((u32)strength*(((u32)func_80096464()&31)+8))/48;
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
            sprite=(RrSpritePacket *)func_8001FCC8((u32 *)(D_800ACC14+0x70),out_cursor,page);
        }
    }
    return (u8 *)sprite;
}
