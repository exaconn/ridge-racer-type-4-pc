/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libcard/cardc192.h"
#include "rr/commands.h"
int _cardGetIdTx() {
    if (CardTxCount == 2) return 0x53;
    return 0;
}
