/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"
#include "psyq/libcard/cardc09.h"

int _cardCmd5BVarTx() {
    switch (CardTxCount) {
    case 2: return 0x5B;
    case 3: return CardCmdSector;
    }
    if (CardTxCount < 5) return 0;
    if (CardCmdParam.value != 0) return 0;
    if (CardTxCount >= CardCmdCount + 5) return 0;
    return CardCmdBuffer[CardTxCount - 5];
}
