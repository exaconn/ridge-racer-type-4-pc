#ifndef RR_GAME_EFFECT_SPARK_H
#define RR_GAME_EFFECT_SPARK_H

#include "types.h"

typedef struct {
    s32 unk00[0x104 / 4];
    s32 frame104;
} SparkSub108;

extern SparkSub108 *ActiveSparkEmitterCar;
void UpdateSparkEffects(void);

#endif /* RR_GAME_EFFECT_SPARK_H */
