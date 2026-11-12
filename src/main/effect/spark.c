#include "rr/video_timing.h"
#include "game/effect/carfx.h"
#include "game/effect/spark.h"
#include "rr/scene_camera_state.h"
extern short SparkFrameRanges[];
extern const short SparkFrameRangesNtsc[];

void UpdateSparkEffects(void) {
    int a0, a1, lo, l1, l2, a2, a3;
    const short *ranges = RR_PAL_NTSC(SparkFrameRanges, SparkFrameRangesNtsc);
    lo = ranges[0];
    a0 = RaceContext.camera24C->frame104;
    if (a0 >= lo) {
        if (a0 < ranges[1]) {
            UpdateSparkBurstEffectA(a0 - lo, ranges[1] - lo - 0x20);
        }
    }
    l2 = ranges[2];
    a2 = RaceContext.camera24C->frame104;
    if (a2 >= l2) {
        if (a2 < ranges[3]) {
            UpdateSparkBurstEffectB(a2 - l2, ranges[3] - l2 - 0x20);
        }
    }
    l1 = ranges[4];
    a1 = RaceContext.camera24C->frame104;
    if (a1 >= l1) {
        if (a1 < ranges[5]) {
            DrawSparkHudOverlay(0, a1 - l1, ranges[5] - l1 - 0x20);
        }
    }
    l1 = ranges[6];
    a1 = RaceContext.camera24C->frame104;
    if (a1 >= l1) {
        if (a1 < ranges[7]) {
            DrawSparkHudOverlay(1, a1 - l1, ranges[7] - l1 - 0x20);
        }
    }
    l1 = ranges[8];
    a1 = RaceContext.camera24C->frame104;
    if (a1 >= l1) {
        if (a1 < ranges[9]) {
            DrawSparkHudOverlay(2, a1 - l1, ranges[9] - l1 - 0x20);
        }
    }
}

/* Initial values of the globals this module owns. */
short SparkFrameRanges[10] = {0, 130, 150, 300, 320, 470, 480, 640, 650, 800};
/* The NTSC release's showcase frames (rr/video_timing.h). */
const short SparkFrameRangesNtsc[10] = {0, 156, 180, 360, 384, 564, 576, 768, 780, 960};
