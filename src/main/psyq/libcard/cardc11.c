#include "rr/commands.h"
#include "psyq/libcard/cardc11.h"

int _cardCmd5CVarTx() {
    switch (CardTxCount) {
    case 2: return 0x5C;
    case 3: return CardCmdSector;
    }
    if (CardTxCount < 5) return 0;
    if (CardCmdParam.value == 0) {
        if (CardTxCount >= CardCmdCount + 5) return 0;
        return CardCmdBuffer[CardTxCount - 5];
    }
    return CardCmdBuffer[CardTxCount - CardCmdCount - 6];
}
