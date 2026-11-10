/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libcard/cardc192.h"
#include "rr/commands.h"
int _cardGetIdTx() {
 register int r asm("$2") = 0x53; int d; d = CardTxCount; if (d != 2) r = 0; return r;
}
