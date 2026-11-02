#ifndef RR_TILED_RECTANGLE_H
#define RR_TILED_RECTANGLE_H

#include "rr/animated_sprite_grid.h"

/* Draw-mode command followed by the four SPRITE payload words. */
typedef struct {
    u32 tag, mode, color, position, uv, size;
} RrTiledRectanglePacket;

/* Rectangle fields need halfword alignment; the output cursor and queue heads
 * need word alignment. Intensity clamps to 0..255. Always emits the two E6 mask
 * commands, even for nonpositive dimensions. Positive dimensions tile using
 * min(remaining,256-(X&63)) and min(remaining,256-(Y&255)). Coordinates/counters
 * stay signed32; only packed wire coordinates narrow to 16 bits. Width and X
 * are read again for each tile row. Links clear each queue-head high byte.
 * The final command uses scratch.order[703], requiring an accessible 704-word
 * ordering page. Output must have 16+24*tile_count bytes; at most 16641 tiles can
 * be emitted for signed16 input dimensions. Scratch pointers retain full width. */
void func_80019D5C(RrGpuRectangleFields *rectangle, s32 intensity, u32 *head);

RR_ASSERT_SIZE(RrTiledRectanglePacket, 24);
RR_ASSERT_OFFSET(RrTiledRectanglePacket, mode, 4);
RR_ASSERT_OFFSET(RrTiledRectanglePacket, color, 8);
RR_ASSERT_OFFSET(RrTiledRectanglePacket, position, 12);
RR_ASSERT_OFFSET(RrTiledRectanglePacket, uv, 16);
RR_ASSERT_OFFSET(RrTiledRectanglePacket, size, 20);

#endif
