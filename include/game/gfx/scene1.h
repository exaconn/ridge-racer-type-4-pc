#ifndef RR_GAME_GFX_SCENE1_H
#define RR_GAME_GFX_SCENE1_H

#include "rr/scene_camera_types.h"
#include "rr/matrix_words.h"

extern VectorWords16 UnitScaleVector;

/* State setters accept complete PAL argument words. Color stores retain the
 * low byte of each channel. */
void InitSceneOverlayState(void);
void SetSceneOverlayColor(s32 mode, s32 red, s32 green, s32 blue);
void SetSceneOverlayEnabled(s32 enabled);
void QueueDrawStp(void);
void DrawSceneOverlayIfActive(void);
void UpdateAndDrawSceneOverlay(void);
void InitCameraState(int mode, int flags);
void SetupObjectLightMatrices(const SceneCameraPose18 *pose,
                              const PreviewLighting40 *lighting,
                              MatrixWords32 *light, MatrixWords32 *rotation);
/* Callers retain the fifth workspace slot; this implementation does not read
 * or write it. The three word components are narrowed to signed GTE inputs. */
void TransformPoint(const MatrixWords32 *rotation, const Vec3i *origin,
                    const Vec3i *point, Vec3i *output,
                    MatrixWords32 *rotationWorkspace);

#endif /* RR_GAME_GFX_SCENE1_H */
