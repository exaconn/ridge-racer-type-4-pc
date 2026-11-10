#ifndef RR_GAME_GFX_SCENE1_H
#define RR_GAME_GFX_SCENE1_H

#include "rr/scene_camera_types.h"
#include "rr/matrix_words.h"

extern VectorWords16 D_800111D4;

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
