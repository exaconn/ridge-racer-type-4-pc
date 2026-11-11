#ifndef RR_TILED_RECTANGLE_H
#define RR_TILED_RECTANGLE_H

#include "rr/animated_sprite_grid.h"
#include "rr/gpu.h"

/* Draw-mode command followed by the four SPRITE payload words. The PsyZ
 * merged-primitive NOP after the E1 command marks the payload as packed
 * 32-bit GP0 words (length 6 on the host; PS1 length 5). */
typedef struct {
    GpuDmaTag tag;
    u32 mode, nop, color, position, uv, size;
} TiledRectanglePacket;

/* Rectangle fields need halfword alignment; the output cursor and queue heads
 * need word alignment. Intensity clamps to 0..255. Always emits the two E6 mask
 * commands, even for nonpositive dimensions. Positive dimensions tile using
 * min(remaining,256-(X&63)) and min(remaining,256-(Y&255)). Coordinates/counters
 * stay signed32; only packed wire coordinates narrow to 16 bits. Width and X
 * are read again for each tile row. Links clear each queue-head high byte.
 * The final command uses scratch.order[703], requiring an accessible 704-word
 * ordering page. Output must have 16+24*tile_count bytes; at most 16641 tiles can
 * be emitted for signed16 input dimensions. Scratch pointers retain full width. */
void EmitTiledRect(GpuRectangleFields *rectangle, s32 intensity, GpuDmaTag *head);


#endif
