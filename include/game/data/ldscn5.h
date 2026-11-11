#ifndef RR_GAME_DATA_LDSCN5_H
#define RR_GAME_DATA_LDSCN5_H

#include "types.h"

/* Sky command table: a count followed by 12-byte records whose first and
 * third words are signed 32-bit offsets from this base (see sky.c). */
extern u8 *SkyCommandListPtr;
s32 LoadCoursePreviewStep(s32);

#endif /* RR_GAME_DATA_LDSCN5_H */
