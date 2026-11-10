/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"

LibInterruptCallback InterruptCallback(s32 channel, LibInterruptCallback callback) {
    return IntrDriverTablePtr->install(channel, callback);
}
