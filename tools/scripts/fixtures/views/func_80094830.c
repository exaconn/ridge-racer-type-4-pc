/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu.h"

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

/* legacy typing of shared globals in this part of the unit */
#define func_80096444 ((int (*)())func_80096444)

#undef func_80096444

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC8 ((s32 *)D_800ABBC8)

#undef D_800ABBC8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC8 ((u32 *)D_800ABBC8)

#undef D_800ABBC8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC4 ((int *)D_800ABBC4)

#define D_800ABBC8 ((int *)D_800ABBC8)

#undef D_800ABBC4

#undef D_800ABBC8

s32 _drs(RrGpuRectangle *arg0,u32 *arg1) {
    register RrGpuRectangle *rect=arg0;
    register u32 *cursor;
    register s32 signed_side;
    /* ASM_BARRIER: retain the raw halfword reload register. */
    register u32 raw_side asm("$3");
    register u32 side;
    register s32 over;
    register u32 limit;
    /* ASM_BARRIER: retain the adjusted word-count arithmetic register. */
    register s32 count,blocks,remainder,adjusted asm("$3");
    /* ASM_BARRIER: retain the preliminary count and captured quotient phases. */
    register s32 preliminary asm("$16");
    register s32 quotient asm("$3");
    register s32 scaled;
    register volatile u32 *status_ptr;
    register u32 status;
    cursor=arg1;
    /* ASM_BARRIER: prepare rectangle and output pointers before starting the deadline. */
    __asm__("" : "+r"(cursor) : "r"(rect));
    set_alarm();
    signed_side=rect->rect.w;raw_side=(u16)rect->rect.w;
    if(signed_side>=0){
        side=raw_side;
        over=D_800ABAC4<signed_side;
        limit=(u16)D_800ABAC4;
        if(over)side=limit;
    }else side=0;
    signed_side=rect->rect.h;raw_side=(u16)rect->rect.h;
    rect->rect.w=side;
    if(signed_side>=0){
        side=raw_side;
        over=D_800ABAC6<signed_side;
        limit=(u16)D_800ABAC6;
        if(over)side=limit;
    }else side=0;
    adjusted=rect->rect.w*(s16)side+1;
    rect->rect.h=side;
    adjusted+=(u32)adjusted>>31;
    count=adjusted>>1;
    preliminary=adjusted>>5;
    if(count<=0)return -1;
    quotient=preliminary;
    /* ASM_BARRIER: capture the block quotient before forming the CPU tail. */
    __asm__("" : "+r"(quotient));
    scaled=quotient*16;
    remainder=count-scaled;
    status_ptr=D_800ABBC8;
    blocks=quotient;
    status=*status_ptr;
    if(!(status&0x04000000U)){
        register u32 mask;
        /* ASM_BARRIER: materialize a fresh mask after the first status test. */
        __asm__("" : : : "$3");
        mask=0x04000000U;
        do {
            if(get_alarm())return -1;
        }while(!(*D_800ABBC8&mask));
    }
    *D_800ABBC8=0x04000000U;
    *D_800ABBC4=0x01000000U;
    *D_800ABBC4=0xC0000000U;
    *D_800ABBC4=rect->words[0];
    *D_800ABBC4=rect->words[1];
    if(!(*D_800ABBC8&0x08000000U)) {
        u32 mask;
        /* ASM_BARRIER: materialize the receive mask after its first status test. */
        __asm__("" : : : "$3");
        mask=0x08000000U;
        do {
            if(get_alarm())return -1;
        }while(!(*D_800ABBC8&mask));
    }
    {
        register s32 empty;
        remainder--;
        empty=-1;
        /* ASM_BARRIER: keep entry and loop sentinels in separate arithmetic phases. */
        __asm__("" : "+r"(empty));
        if(remainder!=empty){
            register s32 sentinel;
            sentinel=-1;
            do {
                u32 word=*D_800ABBC4;
                remainder--;
                *cursor++=word;
            }while(remainder!=sentinel);
        }
    }
    {
        register u32 dma_mode=0x04000000U;
        if(blocks){
            /* ASM_BARRIER: retain DMA register-address and word-count phases. */
            register volatile u32 *port asm("$2")=D_800ABBC8;
            register u32 start;
            register volatile u32 *length_port;
            register u32 length asm("$2");
            dma_mode|=3U;
            *port=dma_mode;
            port=D_800ABBCC;
            start=0x01000000U;
            /* DMA hardware consumes a 32-bit bus address word; keep CPU pointers full. */
            *port=(u32)(unsigned long)cursor;
            length=(u32)blocks<<16;
            length_port=D_800ABBD0;
            length|=16U;
            *length_port=length;
            port=D_800ABBD4;
            start|=0x200U;
            *port=start;
        }
    }
    return 0;
}
