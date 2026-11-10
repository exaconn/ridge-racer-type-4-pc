#include "rr/overlay668_frame_submit.h"

s32 ovl668_func_801150D8(StrDisplayConfig *config) {
    u8 *data;
    s32 remaining = 2000;
    u32 *index;

    while (1) {
        data = ovl668_func_80115164(config);
        if (data != 0) {
            break;
        }
        if (--remaining == 0) {
            return -1;
        }
    }
    index = &config->word00;
    *index = *index < 1U;
    ovl668_func_80116958(data, ActiveStrStreamPtr->commands10[*index]);
    return StFreeRing(data);
}
