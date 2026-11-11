/* RODATA 0x80012178 0x24 */
#include "rr/commands.h"
#include "psyq/libcard/cardc03.h"
#include "psyq/libcard/cardc04.h"
#include "psyq/libcard/cardc07.h"

int _cardCmd5CRangeTx() {
    switch (CardTxCount) {
    case 2: return 0x5C;
    case 3: return 1;
    case 5: return *(u8 *)&CardCmdSector;
    case 6: return D_80114725;
    case 7: return D_80114726;
    case 8: return D_80114727;
    case 9: return CardCmdCount;
    case 4:
    case 10: break;
    default: return CardCmdBuffer[CardTxCount - 0xB];
    }
    return 0;
}

int _cardCmd5CRangeRx() { if (CardRxCount == 9) CardBytesRemaining = CardCmdCount + 1; return 0; }
