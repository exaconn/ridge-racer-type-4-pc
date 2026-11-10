#include "rr/overlay668_decode.h"

void ovl668_setup_stream_ring(LibInterruptCallback callback) {
    ovl668_mdec_reset(0);
    ovl668_mdec_out_callback(callback);
    StSetRing(ActiveStrStreamPtr->queue20, 32);
    StSetStream(1, 1, -1, 0, 0);
}
