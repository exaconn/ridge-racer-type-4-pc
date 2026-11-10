#include "rr/overlay662_car_bars.h"
#include "rr/frame_storage_types.h"

#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif

void ovl662_draw_car_slot_bars(s32 selected)
{
    UiUvRect strip;
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
    head = &CurrentDrawBuf->buffer.ordering70.words[0];
    initial = RR_GPU_SCRATCH->cursor;
    cursor = AddSpritePrim(head, initial, 40, y, &strip, 0x7FA4, 1);

    index = 0;
    status = ovl662_car_slot_statuses;
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
            cursor = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor,
                                  x + 1, y, 10, 22, r, g, b);
        }
        x += 12;
        index++;
        status++;
    } while (index < 20);

    result = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 0x39);
    RR_GPU_SCRATCH->cursor = result;
    if (ovl662_car_slot_statuses[(SelectionRows.bytes[2] % 20) & 255] == 2) {
        result = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], result, 142, 256,
                              &ovl662_car_slot_bar_uv, 0x7CBB, 0);
        RR_GPU_SCRATCH->cursor = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], result, 0x38);
    }
}
