#include "rr/track_interpolation.h"

void func_80043AB0(RrRaceVehicle *vehicle, s32 *output, s32 distance) {
    s16 current_y;
    s32 fraction;
    s32 index, length;
    /* ASM_BARRIER: use v1 for the PAL count page before loading vehicle fields. */
    register u8 *page __asm__("$3");
    s32 neighbour;
    RrTrackSample3C *next;
    RrTrackSample3C *current;

    if (D_800AD522 == 0) {
        index = vehicle->trackNodeF0 - ((s32) (distance - *(s16 *)&vehicle->pad120[0x50]) >> 0xA);
        neighbour = index - 1;
        if (index < 0) {
            index += D_800ABECC;
            neighbour = index - 1;
        }
        if (neighbour < 0) {
            neighbour += D_800ABECC;
        }
    } else {
        page=(u8*)0x800B0000U;
        /* ASM_BARRIER: preserve the page base for the signed count displacement. */
        __asm__("" : "+r"(page));
        length=*(s32*)(page-0x4134);
        index = vehicle->trackNodeF0 + ((s32) (distance - *(s16 *)&vehicle->pad120[0x50]) >> 0xA);
        neighbour = index + 1;
        if (index >= length) {
            index -= length;
            neighbour = index + 1;
        }
        /* This signed test is present in the original, including negative indices. */
        if (neighbour >= 0) {
            neighbour -= length;
        }
    }
    current = D_800ABEA4 + index;
    next = D_800ABEA4 + neighbour;
    fraction = (distance - *(s16 *)&vehicle->pad120[0x50]) & 0x3FF;
    output[0] = (s32) ((((s32) ((next->x08 - current->x08) * fraction) / 1024) + current->x08) * 4);
    current_y = current->y10;
    output[1] = (s32) (((s32) ((next->y10 - current_y) * fraction) / 1024) + current_y);
    output[2] = (s32) ((((s32) ((next->z0C - current->z0C) * fraction) / 1024) + current->z0C) * 4);
}
