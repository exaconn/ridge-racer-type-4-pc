#ifndef RR_GAME_RACE_OBJDRAW_H
#define RR_GAME_RACE_OBJDRAW_H

#include "types.h"
#include "rr/matrix_words.h"
#include "game/race/objrec.h"

void DrawTrackObjects(s32 arg0);
void BuildObjectMatrix(int unused, const s16 *angles, MatrixWords32 *output, int mode);
void SetTrackObjectTransform(int index, MatrixWords32 *matrix, int mode);
/* Serialized TrackObjectFile image, followed by its Node24 keyframe table. */
s32 LoadTrackObjects(void *data, S_ECB4 *states);

#endif /* RR_GAME_RACE_OBJDRAW_H */
