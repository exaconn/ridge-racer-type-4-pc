/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"
#include "psyq/libcard/cardc02.h"

extern s32 _cardCmd59Tx(void);

s32 _cardCmd59Begin(s32 a0, u32 *a1);
int _cardCmd59Tx();

s32 _cardCmd59Begin(s32 a0, u32 *a1) {
    int r;
    if (!(D_800ABDE4 & 1)) {
        r = 0;
    } else {
        CardReceiveIndex = 0; CardRxCount = 0; D_80114730 = 0; D_800ABDE8 = 0; CardCommandRequest = a0;
        r = 1;
    }
    if (!r) return 0;
    CardCmdSector = -2;
    CardCurrentCommand = 1;
    CardCommandReader = _cardCmd59Tx;
    CardWriteHandler = _cardCmd59Rx;
    D_8011471C = a1;
    D_800ABDE4 = 2;
    return 1;
}

int _cardCmd59Tx() {
    int a; 
    register int r __asm__("$2");
    int t; 
    register int c; 
    a = D_800ABDE8;
    t = 2;
    if (a == t) {
        r = 0x59;
        goto out;
    }
    r = 0;
    if (a < 2) goto out;
    if (a >= 6) goto out;
    c = a < 4;
    if (c) goto out;
    /* ASM_BARRIER: stops 2.7.2 if-converting the last compare into xori/negu/andi */
    __asm__("" : "=r"(r) : "0"(0xFF));
out:
    return r;
}
