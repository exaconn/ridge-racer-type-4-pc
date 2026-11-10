#include "game/sound/sndque.h"
#include "rr/overlay667_checkpoint.h"

extern s32 TrackLength;
extern s32 NumLaps;
extern s32 RaceState;
extern s32 TotalRaceTime;
extern s16 FadeCountdown;
extern void SaveCarSnapshots(s32);
extern void SoundSetMasterVolume(s16, s16);

void ovl667_func_80115508(RaceVehicle *arg0) {
    RaceVehicleControl *sub = &arg0->control;
    /* MIPS sums and products retain their low 32 bits. */
    if ((s32)((u32)arg0->second + arg0->first) >= (s32)((u32)(s32)sub->dynamics94.steering.count22 * TrackLength)) {
        if (sub->dynamics94.steering.count22 <= NumLaps) {
            sub->dynamics94.steering.count22=(s16)((u16)sub->dynamics94.steering.count22+1U);
            if (sub->dynamics94.steering.count22 == NumLaps) {
                SaveCarSnapshots(0);
            } else if (sub->dynamics94.steering.count22 > NumLaps) {
                if (RaceState < 4) {
                    TotalRaceTime = 1;
                    RaceState = 4;
                    FadeCountdown = 75;
                    SoundSetMasterVolume(0, 71);
                    SoundQueueEnqueue(19, 1);
                }
            }
        }
    }
}
