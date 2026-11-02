/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
s32 func_8009DB18(s32 input) {
    register s32 original=input;
    /* ASM_BARRIER: Retain the original byte argument and final-marker mask register. */
    register s32 value __asm__("$4");
    /* ASM_BARRIER: Keep error values and shared checksum folding in the ABI result register. */
    register s32 result __asm__("$2");
    /* ASM_BARRIER: Capture the full input before loading the retry flag. */
    __asm__("" : "+r"(original) : : "$2");
    value=original;
    if(D_800ABDF0) {
        switch(D_800ABDF8) {
        case 2: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte==8) {
                D_800ABDF0=0;
                byte=1;
                D_80114730=byte;
                result=0;goto done;
            }
            D_80114730=138;
            result=0;goto done;
        }
        case 3:goto handshake;
        case 8: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte!=0x5D){result=-8;goto done;}
            D_80114714=0;result=0;goto done;
        }
        case 9:
            if((result=value&255)!=((u8 *)&D_80114724)[1]){result=-9;goto done;}
            goto fold_original;
        case 10:
            if((result=value&255)!=*(u8 *)&D_80114724){result=-10;goto done;}
fold_original:
            result=D_80114714;
            result^=original;
            D_80114714=result;result=0;goto done;
        case 139:
            if((result=value&255)==D_80114714)goto success;
            result=-139;goto done;
        case 140:
            if((value&255)!=0x47){result=-140;goto done;}
        default:
            if((u32)D_800ABDF8-11<128)D_80114714^=value;
            result=0;goto done;
        }
    } else {
        switch(D_800ABDF8) {
        case 2: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte==8) {
                D_80114730=1;
                result=0;goto done;
            }
            D_80114730=136;
            result=0;goto done;
        }
        case 3:
handshake:
            {
                /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
                if(byte!=0x5A) {result=-3;goto done;}
                goto success;
            }
        case 137: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte!=0x5D) {result=-137;goto done;}
            goto success;
        }
        case 138: {
            /* ASM_BARRIER: Keep the final marker separate from the masked argument. */
            register s32 magic __asm__("$3");
            value&=255;
            magic=0x47;
            if(value!=magic) {result=-138;goto done;}
            goto success;
        }
        default:result=0;goto done;
        }
    }
success:
    result=0;
done:
    /* ASM_BARRIER: Retain the shared result entry and original validation branch directions. */
    __asm__("" : "+r"(result));
    return result;
}
