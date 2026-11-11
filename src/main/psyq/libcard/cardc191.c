#include "rr/commands.h"
#include "psyq/libcard/cardc192.h"

s32 _cardGetIdBegin(s32 request, u32 *first, u32 *second);

s32 _cardSectorTx(void) {
    s32 result;
    if(CardRetriesLeft) {
        switch(CardTxCount) {
        case 2: return 0x52;
        case 5: return ((u8 *)&CardCmdSector)[1];
        case 6: return *(u8 *)&CardCmdSector;
        default: return 0;
        }
    }
    switch(CardTxCount) {
    case 2: CardChecksum=0; return 0x57;
    case 5: return ((u8 *)&CardCmdSector)[1];
    case 6: return *(u8 *)&CardCmdSector;
    case 133: result=CardCmdParam.value; CardChecksum^=result; return result;
    case 134: return CardChecksum;
    case 135: return ((CardCmdSector>>8)^CardCmdSector)&255;
    default:
        if(CardTxCount<7) return 0;
        if(CardTxCount>=133) return 0;
        result=CardRxTrace[CardTxCount+3];
        CardChecksum^=result;
        return result;
    }
}

s32 _cardSectorRx(s32 input) {
    s32 original = input;
    u8 value;
    value = original;
    if (CardRetriesLeft) {
        switch (CardRxCount) {
        case 2:
            if (value == 8) {
                CardRetriesLeft = 0;
                CardBytesRemaining = 1;
                return 0;
            }
            CardBytesRemaining = 138;
            return 0;
        case 3:
            goto handshake;
        case 8:
            if (value != 0x5D) return -8;
            CardSectorRxChecksum = 0;
            return 0;
        case 9:
            if (value != ((u8 *)&CardCmdSector)[1]) return -9;
            goto fold_original;
        case 10:
            if (value != *(u8 *)&CardCmdSector) return -10;
fold_original:
            CardSectorRxChecksum ^= original;
            goto success;
        case 139:
            if (value == CardSectorRxChecksum) goto success;
            return -139;
        case 140:
            if (value != 0x47) return -140;
            /* fallthrough */
        default:
            if (CardRxCount - 11U < 128) CardSectorRxChecksum ^= value;
            return 0;
        }
    } else {
        switch (CardRxCount) {
        case 2:
            if (value == 8) {
                CardBytesRemaining = 1;
                return 0;
            }
            CardBytesRemaining = 136;
            return 0;
        case 3:
handshake:
            if (value != 0x5A) return -3;
            goto success;
        case 137:
            if (value != 0x5D) return -137;
            goto success;
        default:
            return 0;
        case 138:
            if (value != 0x47) return -138;
            break;
        }
    }
success:
    return 0;
}

s32 _cardGetIdBegin(s32 request, u32 *first, u32 *second) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    CardCurrentCommand = 101;
    CardCommandReader = _cardGetIdTx;
    CardWriteHandler = _cardGetIdRx;
    active = 2;
    CardCmdResultPtrA = first;
    CardCmdResultPtrB = second;
    CardCommandStatus = active;
    return result;
}
