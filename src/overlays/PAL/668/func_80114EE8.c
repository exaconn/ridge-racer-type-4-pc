#include "rr/overlay668_decode.h"

void ovl668_func_80114EE8(LibInterruptCallback callback) {
    ovl668_func_80116298(0);
    ovl668_func_80116534(callback);
    StSetRing(D_800AC708->queue20, 32);
    StSetStream(1, 1, -1, 0, 0);
}
