#include "rr/overlay662_car_bars.h"

#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((RrGpuScratchPointers *)0x1F800000U)
#endif

void ovl662_func_80116C34(s32 selected)
{
    RrUiUvRect strip;
    void *result;
    u32 *head;
    void *initial;
    s32 x = 40, index, r, g, b;
    s32 y = 288;
    s16 *status;
    void *cursor;

    strip.v = 64;
    strip.w = 240;
    strip.h = 24;
    strip.u = 0;
    head = &D_800ACC14->head70;
    initial = RR_GPU_SCRATCH->cursor;
    cursor = func_8001FF84(head, initial, 40, y, &strip, 0x7FA4, 1);

    index = 0;
    status = ovl662_D_80127300;
    do {
        if (*status != 0) {
            r = 255;
            if (index == selected) {
                r = 240;
                g = 122;
                b = 20;
            } else {
                g = 189;
                b = 0;
            }
            cursor = AddTilePrim(&D_800ACC14->head70, cursor,
                                  x + 1, y, 10, 22, r, g, b);
        }
        x += 12;
        index++;
        status++;
    } while (index < 20);

    result = AddDrawModePrim(&D_800ACC14->head70, cursor, 0x39);
    RR_GPU_SCRATCH->cursor = result;
    if (ovl662_D_80127300[(D_800F4D98.bytes[2] % 20) & 255] == 2) {
        result = func_8001FF84(&D_800ACC14->head70, result, 142, 256,
                              &ovl662_D_80126214, 0x7CBB, 0);
        RR_GPU_SCRATCH->cursor = AddDrawModePrim(&D_800ACC14->head70, result, 0x38);
    }
}
