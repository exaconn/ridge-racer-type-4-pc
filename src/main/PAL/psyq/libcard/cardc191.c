/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/commands.h"
#include "psyq/libcard/cardc192.h"

s32 _cardGetIdBegin(s32 request, u32 *first, u32 *second);

s32 _cardSectorTx(void) {
    register s32 result;
    /* ASM_BARRIER: Keep the folding accumulator separate from the returned byte. */
    register s32 checksum __asm__("$3");
    if(CardRetriesLeft) {
        switch(CardTxCount) {
        case 2: result=0x52;goto done;
        case 5: goto high;
        case 6: goto low;
        default: result=0;goto done;
        }
    }
    switch(CardTxCount) {
    case 2: CardChecksum=0;result=0x57;goto done;
    case 5:
high: result=((u8 *)&CardCmdSector)[1];goto done;
    case 6:
low: result=*(u8 *)&CardCmdSector;goto done;
    case 133: result=CardCmdParam.value;checksum=CardChecksum^result;goto fold;
    case 134: result=CardChecksum;goto done;
    case 135: result=((CardCmdSector>>8)^CardCmdSector)&255;goto done;
    default:
        if(CardTxCount<7) {result=0;goto done;}
        if(CardTxCount>=133) {result=0;goto done;}
        {
            s32 phase=CardTxCount;
            checksum=CardChecksum;
            result=CardRxTrace[phase+3];
            checksum^=result;
        }
fold:
        CardChecksum=checksum;
    }
done:
    return result;
}

s32 _cardSectorRx(s32 input) {
    register s32 original=input;
    /* ASM_BARRIER: Retain the original byte argument and final-marker mask register. */
    register s32 value __asm__("$4");
    /* ASM_BARRIER: Keep error values and shared checksum folding in the ABI result register. */
    register s32 result __asm__("$2");
    /* ASM_BARRIER: Capture the full input before loading the retry flag. */
    __asm__("" : "+r"(original) : : "$2");
    value=original;
    if(CardRetriesLeft) {
        switch(CardRxCount) {
        case 2: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte==8) {
                CardRetriesLeft=0;
                byte=1;
                CardBytesRemaining=byte;
                result=0;goto done;
            }
            CardBytesRemaining=138;
            result=0;goto done;
        }
        case 3:goto handshake;
        case 8: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte!=0x5D){result=-8;goto done;}
            CardSectorRxChecksum=0;result=0;goto done;
        }
        case 9:
            if((result=value&255)!=((u8 *)&CardCmdSector)[1]){result=-9;goto done;}
            goto fold_original;
        case 10:
            if((result=value&255)!=*(u8 *)&CardCmdSector){result=-10;goto done;}
fold_original:
            result=CardSectorRxChecksum;
            result^=original;
            CardSectorRxChecksum=result;result=0;goto done;
        case 139:
            if((result=value&255)==CardSectorRxChecksum)goto success;
            result=-139;goto done;
        case 140:
            if((value&255)!=0x47){result=-140;goto done;}
        default:
            if(CardRxCount-11U<128)CardSectorRxChecksum^=value;
            result=0;goto done;
        }
    } else {
        switch(CardRxCount) {
        case 2: {
            /* ASM_BARRIER: Preserve the original byte comparison temporary and branch delay. */
            register u32 byte __asm__("$3")=value&255;
            if(byte==8) {
                CardBytesRemaining=1;
                result=0;goto done;
            }
            CardBytesRemaining=136;
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
    return result;
}

s32 _cardGetIdBegin(s32 request, u32 *first, u32 *second) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    CardCurrentCommand = 101;
    CardCommandReader = _cardGetIdTx;
    CardWriteHandler = _cardGetIdRx;
    active = 2;
    CardCmdResultPtrA = first;
    CardCmdResultPtrB = second;
    {
        s32 *base;
        /* PAL RAM mapping: base[-0x421C / 4] is CardCommandStatus. */
        base = (s32 *)0x800B0000U;
        base[-0x421C / 4] = active;
    }
    return result;
}
