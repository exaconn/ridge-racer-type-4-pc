/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libcard/cardsts.h"
#include "rr/commands.h"

extern unsigned char D_80114741;

int MemCardSync(int mode, int *command, int *result) {
    if (CardCurrentCommand == 0 || CardCommandStatus == 1) {
        *result = 0;
        *command = 0;
        return -1;
    }
    if (mode == 0) {
        while (CardCommandStatus < 5);
    }
    *command = CardCurrentCommand;
    switch (CardCommandStatus) {
    case 4:
    case 2:
        *result = 0;
        return 0;
    case 17:
        {
            int transmitted = CardTxCount;
            if (transmitted == 1) {
                *result = transmitted;
            } else {
                *result = 2;
            }
        }
        break;
    case 33:
        *result = 2;
        break;
    default:
        if (D_80114741 == 8) {
            *result = 3;
        } else {
            *result = 0;
        }
        break;
    }
    CardCurrentCommand = 0;
    return 1;
}

s32 _cardCmdReset(s32 value) {
    s32 result;
    if (!(CardCommandStatus & 1)) return 0;
    CardReceiveIndex = 0;
    CardRxCount = 0;
    CardBytesRemaining = 0;
    CardTxCount = 0;
    result = 1;
    CardCommandRequest = value;
    return result;
}

