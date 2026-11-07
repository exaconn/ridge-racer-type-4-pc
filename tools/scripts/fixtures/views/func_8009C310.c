/* GCC_VERSION: 2.7.2 */

#include "rr/commands.h"

extern s32 _cardCmd59Tx(void), _cardCmd59Rx(s32);

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
