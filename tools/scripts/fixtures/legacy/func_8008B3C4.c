/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
LibInterruptCallback DMACallback(s32 channel, LibInterruptCallback callback) {
    return IntrDriverTablePtr->exchange(channel,callback);
}
