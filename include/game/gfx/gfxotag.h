#ifndef RR_GAME_GFX_GFXOTAG_H
#define RR_GAME_GFX_GFXOTAG_H

#include "types.h"
#include "rr/scene_camera_types.h"

extern short D_8009FE60[];
extern u16 Pad0Buttons;
void ApplyCameraState(const SceneCameraView24 *state);
void RenderSubOtObject(void);
void ResetLensFlareState(void);


#endif /* RR_GAME_GFX_GFXOTAG_H */
