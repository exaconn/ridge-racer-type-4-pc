#ifndef RR_ANIMATED_SPRITE_GRID_H
#define RR_ANIMATED_SPRITE_GRID_H
#include "rr/gpu.h"

/* PAL scratchpad pointer words; native fixtures retain full pointer widths. */
typedef struct {u8 *cursor;u32 *order;} RrGpuScratchPointers;
typedef struct {u8 pad00[14];u16 tick0E;} RrSpriteGridClock;
typedef struct {u32 tag,command;} RrGpuSingleCommandPacket;

/* Requires aligned pointer slots, a 4-byte-aligned cursor with space for 15
 * sprite packets and one 8-byte command, and accessible heads after each order
 * pointer reload. Adds one modulo 16-bit tick, then advances phase by 5 for each
 * sprite. Raw intensity bits are OR-packed without clamping. Each list insertion
 * clears the ordering-head high byte. Finishes with command E100022A. */
void func_8001E1F0(u32 intensity,RrSpriteGridClock *state);

RR_ASSERT_SIZE(RrGpuScratchPointers, 8);
RR_ASSERT_OFFSET(RrGpuScratchPointers, order, 4);
RR_ASSERT_SIZE(RrSpriteGridClock, 16);
RR_ASSERT_OFFSET(RrSpriteGridClock, tick0E, 14);
RR_ASSERT_SIZE(RrGpuSingleCommandPacket, 8);
#endif
