#ifndef RR_GAME_GFX_GTEPOLY1_H
#define RR_GAME_GFX_GTEPOLY1_H

#include "types.h"
#include "rr/geometry.h"
#include "rr/matrix_words.h"

int DrawRroModel(const Vector *position, MatrixWords32 *rotation,
                 s32 object, s32 flags);
void DrawWheelTrailEffects(void *arg0, s32 *arg1, void *arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);
/* Full texture-page word consumed by the model renderer. */
extern s32 ModelTexturePage;
void SetModelTexturePage(s32 page);


#endif /* RR_GAME_GFX_GTEPOLY1_H */
