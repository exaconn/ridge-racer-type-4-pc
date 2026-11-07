/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

/* Partial layout: six input slot IDs, per-group limits, six output IDs. */
typedef struct {
    char pad0[4];
    u8 *group_data;
    char pad8[0x20 - 8];
    u8 *slot_groups;
    char pad24[0x46 - 0x24];
    u8 status;
    char pad47[0x5D - 0x47];
    u8 assignments[6];
    char pad63[0xE9 - 0x63];
    u8 group_count;
} RrSlotAssignmentState;

s32 _padActAlignComplete(RrSlotAssignmentState *state)
{
    s32 group, minimum, remaining, i;
    u8 *slots;

    /* Groups with fewer slots than their minimum are marked unassigned. */
    group = 0;
    if (state->group_count != 0) {
        do {
            slots = state->slot_groups;
            remaining = 0;
            for (i = 5; i >= 0; i--) {
                if (*slots++ == group) remaining++;
            }
            minimum = state->group_data[group * 5 + 2];
            slots = state->slot_groups;
            if (minimum == 0) minimum = 1;
            i = 0;
            do {
                if (*slots++ == group) {
                    if (remaining < minimum) {
                        state->assignments[i] = 0xFF;
                        remaining--;
                    } else {
                        state->assignments[i] = group;
                    }
                }
                i++;
            } while (i < 6);
            group++;
        } while (group < state->group_count);
    }
    state->status = 0xFE;
    return 0;
}
