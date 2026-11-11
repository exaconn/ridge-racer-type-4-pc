#ifndef RR_GAME_DEMO_ATTRACT_H
#define RR_GAME_DEMO_ATTRACT_H

#include "types.h"
#include "game/system/pad.h"
#include "rr/color_conversion.h"

/* Attract-demo work block, RaceContext.attract4BC (PS1 AttractDemoStateBuffer,
 * RaceContext+0x4BC..0x53C). Its words at 0x1C and 0x20 were the PS1 symbols
 * TitleAttractReadyFlag and TitleAttractDelayTimer. */
typedef struct {
    s32 frame00;            /* frames since entry, saturates at 6000 */
    s32 cameraCar04;        /* ComCars index followed by the race camera */
    s32 cameraTimer08;
    s32 mode0C;             /* StartAttractDemo argument: 1 = BGM viewer */
    s32 exitStage10;
    s32 cdFadeTimer14;
    s32 trackDelay18;
    s32 repeat1C;           /* TitleAttractReadyFlag */
    s32 idleTimer20;        /* TitleAttractDelayTimer */
    HsvColor overlayHsv24;
    u8 pad2A[2];
    RgbColor overlayRgb2C;
    u8 pad2F;
    s32 camera30[17];       /* InitDemoComCars parameters */
    s32 freeCamera74;
    s32 viewMode78;
    s32 bgmLocked7C;
} AttractDemoState80;
RR_ASSERT_SIZE(AttractDemoState80, 0x80);
RR_ASSERT_OFFSET(AttractDemoState80, overlayHsv24, 0x24);
RR_ASSERT_OFFSET(AttractDemoState80, camera30, 0x30);
RR_ASSERT_OFFSET(AttractDemoState80, freeCamera74, 0x74);

void StartAttractDemo(int a0);
void AttractDemoRaceEnter(void);


#endif /* RR_GAME_DEMO_ATTRACT_H */
