/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
LibInterruptCallback _SpuDataCallback(LibInterruptCallback callback) {
    return DMACallback(4,callback);
}
