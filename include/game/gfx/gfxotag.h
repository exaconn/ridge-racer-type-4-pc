#ifndef RR_GAME_GFX_GFXOTAG_H
#define RR_GAME_GFX_GFXOTAG_H

#include "types.h"
#include "rr/scene_camera_types.h"

extern short CourseSkyboxTypeTable[];
extern u16 Pad0Buttons;
void ApplyCameraState(const SceneCameraView24 *state);
void RenderSubOtObject(void);
void ResetLensFlareState(void);


#endif /* RR_GAME_GFX_GFXOTAG_H */
