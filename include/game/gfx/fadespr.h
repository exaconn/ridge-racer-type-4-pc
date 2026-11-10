#ifndef RR_GAME_GFX_FADESPR_H
#define RR_GAME_GFX_FADESPR_H

#include "types.h"
#include "rr/layout.h"

typedef struct {
    s32 unk00[0xB0 / 4];
    s32 unkB0;
    s32 unkB4[(0x3E0 - 0xB4) / 4];
    s32 f3E0;
    s32 f3E4;
    s32 scale3E8;
    s32 f3EC;
    u8 r3F0;
    u8 g3F1;
    u8 b3F2;
    u8 f3F3;
    s32 f3F4;
    s32 f3F8;
    s32 f3FC;
    s32 f400;
    s32 f404;
    s32 f408;
    s32 f40C;
    s16 f410;
    s16 f412;
    s16 f414;
    s16 f416;
    s16 f418;
    s16 f41A;
    s32 f41C;
    s32 f420;
    s32 f424;
    s32 f428;
    s32 f42C;
    s32 f430;
    s32 f434;
    s32 f438;
    s32 f43C;
    s32 f440;
} FadeRaceContext;

RR_ASSERT_SIZE(FadeRaceContext, 0x444);
RR_ASSERT_OFFSET(FadeRaceContext, f3E0, 0x3E0);
RR_ASSERT_OFFSET(FadeRaceContext, f410, 0x410);
RR_ASSERT_OFFSET(FadeRaceContext, f440, 0x440);

void InitOptionsDefaults(void);
void InitDemoFadeContext(int a0);
void UpdateFadeTint(int a0);
void EmitFadeSprite(s32 index, s32 x, s32 y, s32 intensity);


#endif /* RR_GAME_GFX_FADESPR_H */
