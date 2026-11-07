/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
RrLibInterruptCallback DMACallback(s32 channel, RrLibInterruptCallback callback) {
    return D_800A6350->exchange(channel,callback);
}
