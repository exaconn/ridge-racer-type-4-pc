/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"
extern s32 func_8001E84C(const char *,...);
extern char D_80011698[],D_800116B8[],D_800116CC[];
void func_8007ED44(const u16 *source,u32 size) {
    register RrSpuRegisterBank *port=D_800A47D8;
    /* ASM_BARRIER: Retain the transfer-address halfword while capturing device status. */
    register u32 address __asm__("$3")=D_800A47F0;
    register u32 remaining=size;
    register u32 previous;
    s32 chunk;
    /* ASM_BARRIER: Capture initial status in the consumed byte-count argument register. */
    register u32 status __asm__("$5")=port->spu_status;
    register const u16 *input;
    input=source;
    port->transfer_address=address;
    /* ASM_BARRIER: Publish the transfer address before preparing the saved status mask. */
    __asm__("" : "+r"(status) : : "memory");
    previous=status&0x7FF;
    func_8007F59C();
    while(remaining) {
        s32 written;
        if(remaining<65)chunk=remaining;else chunk=64;
        written=0;
        if(chunk>0) {
            RrSpuRegisterBank *output=D_800A47D8;
            do {
                output->transfer_data=*input++;
                written+=2;
            } while(written<chunk);
        }
        {
            register RrSpuRegisterBank *control=D_800A47D8;
            /* ASM_BARRIER: Keep the old control halfword separate from its command bits. */
            /* ASM_BARRIER: Capture final control before restoring the previous status comparison. */
        register u32 old __asm__("$4")=control->spu_control;
            /* ASM_BARRIER: Form the original control mask in the call-delay temporary. */
            /* ASM_BARRIER: Keep the final mode-clear result separate from the next poll counter. */
        register u32 command __asm__("$2");
            /* ASM_BARRIER: Preserve the original control read and mask operands. */
            __asm__("" : "+r"(old));
            command=old&0xFFCF;
            *(u16 *)&control->spu_control=command|0x10;
            func_8007F59C();
        }
        {
            register u32 count=0;
            if(D_800A47D8->spu_status&0x400) {
                u32 busy;
                /* ASM_BARRIER: Retain zero then increment when entering the bounded busy wait. */
                /* ASM_BARRIER: Retain the final wait initial increment after its entry comparison. */
            __asm__("" : "+r"(count));
                count++;
                do {
                    if(count>0xF00) {
                        /* ASM_BARRIER: Prepare the diagnostic format in its original argument register. */
                        /* ASM_BARRIER: Keep the restore-timeout diagnostic format in its argument register. */
                    register const char *format __asm__("$4")=D_80011698;
                        /* ASM_BARRIER: Prepare the mapped diagnostic reason in its original argument register. */
                        /* ASM_BARRIER: Keep the restore-timeout reason in its argument register. */
                    register const char *reason __asm__("$5");
                        /* ASM_BARRIER: Finish the format address before forming the reason-page address. */
                        /* ASM_BARRIER: Finish the second format before forming its mapped reason page. */
                    __asm__("" : : "r"(format) : "$5");
                        reason=(const char *)0x80010000U;
                        /* ASM_BARRIER: Expose the real PAL ROM page for the diagnostic call delay slot. */
                        /* ASM_BARRIER: Expose the restore-timeout ROM reason in the call delay slot. */
                    __asm__("" : "+r"(reason));
                        func_8001E84C(format,reason+0x16B8);break;
                    }
                    busy=D_800A47D8->spu_status&0x400;
                    count++;
                } while(busy);
            }
        }
        remaining-=chunk;
        func_8007F59C();
        func_8007F59C();
    }
    port=D_800A47D8;
    {
        /* ASM_BARRIER: Capture final control before restoring the previous status comparison. */
        register u32 old __asm__("$4")=port->spu_control;
        register u32 expected;
        /* ASM_BARRIER: Keep the final mode-clear result separate from the next poll counter. */
        register u32 command __asm__("$3");
        /* ASM_BARRIER: Retain both final captured values before narrowing the saved status. */
        __asm__("" : "+r"(old), "+r"(previous));
        expected=(u16)previous;
        /* ASM_BARRIER: Prepare the saved-status halfword before the control restore store. */
        __asm__("" : "+r"(expected));
        command=old&0xFFCF;
        port->spu_control=command;
        {
        /* ASM_BARRIER: Retain the final bounded-poll counter in its original temporary. */
        register u32 count __asm__("$3")=0;
        if((port->spu_status&0x7FF)!=expected) {
            u32 busy;
            /* ASM_BARRIER: Retain the final wait initial increment after its entry comparison. */
            __asm__("" : "+r"(count));
            count++;
            do {
                if(count>0xF00) {
                    /* ASM_BARRIER: Keep the restore-timeout diagnostic format in its argument register. */
                    register const char *format __asm__("$4")=D_80011698;
                    /* ASM_BARRIER: Keep the restore-timeout reason in its argument register. */
                    register const char *reason __asm__("$5");
                    /* ASM_BARRIER: Finish the second format before forming its mapped reason page. */
                    __asm__("" : : "r"(format) : "$5");
                    reason=(const char *)0x80010000U;
                    /* ASM_BARRIER: Expose the restore-timeout ROM reason in the call delay slot. */
                    __asm__("" : "+r"(reason));
                    func_8001E84C(format,reason+0x16CC);break;
                }
                busy=(D_800A47D8->spu_status&0x7FF)!=expected;
                count++;
            } while(busy);
        }
    }
}
}
