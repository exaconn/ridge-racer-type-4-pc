/* GCC_VERSION: 2.7.2 */

#include "rr/library.h"

RrLibInterruptCallback _SpuDataCallback(RrLibInterruptCallback callback) {
    return DMACallback(4,callback);
}
