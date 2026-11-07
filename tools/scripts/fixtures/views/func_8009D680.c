/* GCC_VERSION: 970318 */

#include "rr/commands.h"

s32 _cardCmd58Rx(s32 value) {
    s32 result;
    switch(CardRxCount) {
    case 3: D_80114730=value & 255; break;
    case 4: *(u32 *)D_8011471C=value & 255; break;
    case 5: *D_80114720=value & 255; break;
    }
    result=0;
    return result;
}
