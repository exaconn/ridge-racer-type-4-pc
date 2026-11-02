#include "rr/overlay659.h"

u32 ovl659_module_id __attribute__((section(".ovl659.header"))) = 10;

u8 ovl659_finish_rank_limits[4]
    __attribute__((section(".ovl659.finish_rank_limits"))) = {2, 1, 0, 0};

s32 ovl659_result_labels[34] __attribute__((section(".ovl659.result_labels"))) = {
    7701, 0,   4378, 3,    4,    -1, 269,  525, 781,  20, 1038, 7701, 0,  4378, 18, -1,   269,
    525,  781, 20,   1038, 7701, 5,  7701, 17,  7701, -1, 6,    24,   20, 4107, -1, 7701, -1};

s32 ovl659_preview_timer __attribute__((section(".ovl659.preview_timer"))) = 0;
