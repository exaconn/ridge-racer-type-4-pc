#ifndef RR_GAME_GFX_SKY_H
#define RR_GAME_GFX_SKY_H

#include "types.h"
#include "rr/gpu.h"

/* Track scenery table loaded from the course file: a count word followed by
 * 12-byte records {vertex offset, unused, command offset}. The offsets are
 * signed 32-bit displacements from the table start and stay unrelocated on
 * the host; DrawTrackScenery resolves them against this pointer. */
extern s32 *TrackSkyTable;
/* translation: three GTE translation words; rotation: a 3x3 s16 matrix;
 * rect: off-screen VRAM rectangle; a3: copy-back intensity. */
void RenderSkyToRect(const void *translation, const void *rotation,
                     const GpuRectangleFields *rect, int a3);
void DrawTrackScenery(int a0);
/* table uses the TrackSkyTable record format (see SkyCommandListPtr). */
void RunSkyCommandList(const u8 *table, int index);
void SetupSkyLightMatrices(char *a0, unsigned short *a1_in);


#endif /* RR_GAME_GFX_SKY_H */
