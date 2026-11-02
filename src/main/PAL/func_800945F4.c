/* CC1_FLAGS: -mno-split-addresses */
#include "rr/gpu.h"
typedef char RrGpuUploadAddressFitsPointer[
    sizeof(unsigned long) >= sizeof(void *) ? 1 : -1];

/* The small CPU tail uses compiler-fenced stores so GCC can place the final
 * write in a branch delay slot. The per-iteration and final memory fences
 * make every port write visible and ordered before subsequent accesses. */
s32 func_800945F4(RrGpuRectangle *arg0,const u32 *arg1) {
    register RrGpuRectangle *rect=arg0;
    register const u32 *cursor;
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
    register s32 mode;
    u32 command;
    cursor=arg1;
    /* ASM_BARRIER: prepare both input pointers before starting the deadline. */
    __asm__("" : "+r"(cursor) : "r"(rect));
    func_80095360();
    signed_side=rect->rect.w;raw_side=(u16)rect->rect.w;
    mode=0;
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
            if(func_80095394())return -1;
        }while(!(*D_800ABBC8&mask));
    }
    command=0xA0000000U;
    *D_800ABBC8=0x04000000U;
    *D_800ABBC4=0x01000000U;
    {
        register volatile u32 *port=D_800ABBC4;
        if(mode)command=0xB0000000U;
        *port=command;
    }
    *D_800ABBC4=rect->words[0];
    *D_800ABBC4=rect->words[1];
    remainder--;
    if(remainder!=-1){
        register s32 sentinel;
        /* ASM_BARRIER: form the loop sentinel independently of the return value. */
        __asm__("" : : : "$2");
        sentinel=-1;
        do {
            register u32 word;
            /* ASM_BARRIER: expose each preceding port write before new input loads. */
            __asm__ volatile("" : : : "memory");
            word=*cursor++;
            remainder--;
            *(u32 *)D_800ABBC4=word;
        }while(remainder!=sentinel);
    }
    /* ASM_BARRIER: expose the final port write before configuring DMA. */
    __asm__ volatile("" : : : "memory");
    {
        register u32 dma_mode=0x04000000U;
        if(blocks){
            /* ASM_BARRIER: retain DMA register-address and word-count phases. */
            register volatile u32 *port asm("$2")=D_800ABBC8;
            register u32 start;
            register volatile u32 *length_port;
            register u32 length asm("$2");
            dma_mode|=2U;
            *port=dma_mode;
            port=D_800ABBCC;
            start=0x01000000U;
            /* DMA hardware consumes a32-bit bus-address word; keep CPU pointers full. */
            *port=(u32)(unsigned long)cursor;
            length=(u32)blocks<<16;
            length_port=D_800ABBD0;
            length|=16U;
            *length_port=length;
            port=D_800ABBD4;
            start|=0x201U;
            *port=start;
        }
    }
    return 0;
}
