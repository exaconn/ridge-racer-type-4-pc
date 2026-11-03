/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/commands.h"

extern s32 func_8009DE30(void);

s32 func_8009D978(void);
s32 func_8009DB18(s32 input);
s32 func_8009DD88(s32 request, u32 *first, u32 *second);

s32 func_8009D978(void) {
    register s32 result;
    /* ASM_BARRIER: Keep the folding accumulator separate from the returned byte. */
    register s32 checksum __asm__("$3");
    if(D_800ABDF0) {
        /* ASM_BARRIER: Keep the retry-phase comparison after its independent tick read. */
        __asm__("" : : : "$2");
        switch(D_800ABDE8) {
        case 2: result=0x52;goto done;
        case 5: goto high;
        case 6: goto low;
        default: result=0;goto done;
        }
    }
    /* ASM_BARRIER: Keep the normal-phase comparison after its independent tick read. */
    __asm__("" : : : "$2");
    switch(D_800ABDE8) {
    case 2: D_80114710=0;result=0x57;goto done;
    case 5:
high: result=((u8 *)&D_80114724)[1];goto done;
    case 6:
low: result=*(u8 *)&D_80114724;goto done;
    case 133: result=D_8011472C.value;checksum=D_80114710^result;goto fold;
    case 134: result=D_80114710;goto done;
    case 135: result=((D_80114724>>8)^D_80114724)&255;goto done;
    default:
        if(D_800ABDE8<7) {result=0;goto done;}
        /* ASM_BARRIER: Preserve the separate fresh tick read for the upper bound. */
        __asm__("" : : : "memory");
        if(D_800ABDE8>=133) {result=0;goto done;}
        /* ASM_BARRIER: Reload the selected tick after both bounds have passed. */
        __asm__("" : : : "memory");
        {
            s32 phase=D_800ABDE8;
            checksum=D_80114710;
            result=D_80114740[phase+3];
            checksum^=result;
        }
fold:
        D_80114710=checksum;
    }
done:
    /* ASM_BARRIER: Retain the original shared return entry for all protocol phases. */
    __asm__("" : "+r"(result));
    return result;
}

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

s32 func_8009DD88(s32 request, u32 *first, u32 *second) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 101;
    D_80114734 = func_8009DE30;
    D_80114738 = func_8009DE54;
    active = 2;
    D_8011471C = first;
    D_80114720 = second;
    {
        /* ASM_BARRIER: preserve the original assembler temporary for the mapped store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: finish parameter publication before materializing the state address. */
        __asm__ volatile("" : : : "$1", "memory");
        /* PAL RAM mapping: base[-0x421C / 4] is D_800ABDE4. */
        base = (s32 *)0x800B0000U;
        /* ASM_BARRIER: expose the active-state store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[-0x421C / 4] = active;
    }
    return result;
}
