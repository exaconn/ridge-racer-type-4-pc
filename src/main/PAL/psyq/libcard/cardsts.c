/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libcard/cardsts.h"
#include "rr/commands.h"

extern volatile int CardCommandStatus;
extern unsigned char D_80114741;

int MemCardSync(a0, a1, a2) int a0; int *a1; int *a2; {
    if (CardCurrentCommand == 0 || CardCommandStatus == 1) {
        *a2 = 0;
        *a1 = 0;
        return -1;
    }
    if (a0 == 0) {
        while (CardCommandStatus < 5);
    }
    *a1 = CardCurrentCommand;
    switch (CardCommandStatus) {
    case 4:
    case 2:
        *a2 = 0;
        return 0;
    case 17:
        {
            int tx = CardTxCount;
            if (tx == 1) {
                *a2 = tx;
            } else {
    case 33:
                *a2 = 2;
            }
        }
        goto fin;
    default:
        if (D_80114741 == 8) {
            *a2 = 3;
        } else {
            *a2 = 0;
        }
    fin:
        CardCurrentCommand = 0;
        return 1;
    }
}

s32 _cardCmdReset(s32 value) {
    s32 result;
    if (!(CardCommandStatus & 1)) return 0;
    CardReceiveIndex = 0;
    CardRxCount = 0;
    CardBytesRemaining = 0;
    CardTxCount = 0;
    {
        s32 *base;
        result = 1;
        /* PAL RAM mapping: CardCommandRequest is base minus 0x4214. */
        base = (s32 *)0x800B0000U;
        base[-0x4214 / 4] = value;
    }
    return result;
}

