#include "rr/overlay668_frame_submit.h"

s32 ovl668_decode_next_frame(StrDisplayConfig *config) {
    u8 *data;
    s32 remaining = 2000;
    u32 *index;

    while (1) {
        data = ovl668_acquire_stream_frame(config);
        if (data != 0) {
            break;
        }
        if (--remaining == 0) {
            return -1;
        }
    }
    index = &config->word00;
    *index = *index < 1U;
    ovl668_mdec_decode_vlc(data, ActiveStrStreamPtr->commands10[*index]);
    return StFreeRing(data);
}
