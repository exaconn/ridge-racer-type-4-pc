#ifndef RR_GAME_GFX_SPREFCT_H
#define RR_GAME_GFX_SPREFCT_H

#include "rr/gpu.h"

/* Coordinates, dimensions and colors arrive as full words. Packet stores
 * retain low16 coordinates/dimensions and low8 colors. */
void *AddTileSemiTransPrim(GpuDmaTag *ordering, void *cursor, s32 x, s32 y,
    s32 width, s32 height, s32 red, s32 green, s32 blue);

/* Three four-byte entries per coordinate/UV array. Material arguments arrive
 * as full words and are narrowed to their low 16 bits by the emitter. The
 * final reserved packet halfword is preserved; return advances one packet. */
GpuPolyFT3Packet *EmitPolyFT3(GpuDmaTag *ordering, GpuPolyFT3Packet *packet,
    const s32 *x, const s32 *y, const s32 *u, const s32 *v,
    u32 pageWord, u32 clutWord, u32 modeWord, u32 shadeWord);
PolyF3Packet *EmitPolyF3(GpuDmaTag *ordering, PolyF3Packet *packet,
    s32 xy0, s32 xy1, s32 xy2, const u8 *rgb, s32 flags);

#endif /* RR_GAME_GFX_SPREFCT_H */
