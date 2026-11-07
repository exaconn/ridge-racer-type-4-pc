/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"

RrLibInterruptCallback InterruptCallback(s32 channel, RrLibInterruptCallback callback) {
    return D_800A6350->install(channel, callback);
}
