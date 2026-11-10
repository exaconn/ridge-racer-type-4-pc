#ifndef RR_GAME_GFX_TILERECT_H
#define RR_GAME_GFX_TILERECT_H

#include "rr/track_object_render.h"

void DrawTrackObjectBillboard(int modelIndex);
void DrawTrackObjectFlashQuads(int modelIndex);
u8 *EmitModelTrisGT3(const TrackObjectLitWork *work, u32 *head, u8 *packetCursor);
void TileRectSceneModeEnter(void);
void TileRectSceneModeUpdate(void);


#endif /* RR_GAME_GFX_TILERECT_H */
