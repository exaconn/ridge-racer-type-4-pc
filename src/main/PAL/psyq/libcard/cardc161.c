/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libcard/cardc161.h"
#include "rr/commands.h"
int _cardCmd58Tx() {
    if (CardTxCount == 2) return 0x58;
    return 0;
}
