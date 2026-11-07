/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
RrLibInterruptCallback DMACallback(s32 channel, RrLibInterruptCallback callback) {
    return IntrDriverTablePtr->exchange(channel,callback);
}
