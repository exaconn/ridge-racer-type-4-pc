#include "rr/library.h"
LibInterruptCallback _SpuDataCallback(LibInterruptCallback callback) {
    return DMACallback(4,callback);
}
