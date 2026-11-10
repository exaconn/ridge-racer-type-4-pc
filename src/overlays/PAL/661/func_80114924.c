#include "rr/overlay661_checkpoint.h"

extern s32 TrackLength;
extern s32 NumLaps;
extern s16 LapVoicePending;
extern s32 RaceState;
extern s16 FadeCountdown;
extern s32 TotalRaceTime;
extern s32 RaceVoiceFlags;

extern void SaveCarSnapshots(s32);
extern void SoundSetMasterVolume(s16, s16);

void ovl661_func_80114924(RaceVehicle *vehicle) {
    RaceVehicleControl *control = &vehicle->control;
    s16 counter;
    s32 target;
    s32 message;

    counter = control->dynamics94.steering.count22;
    if (((s32)((u32)vehicle->second + vehicle->first)) >= ((s32)((u32)(s32)counter * TrackLength))) {
        target = NumLaps;
        if (target >= counter) {
            control->dynamics94.steering.count22=(s16)((u16)control->dynamics94.steering.count22+1U);
            LapVoicePending = 0;
            if (control->dynamics94.steering.count22 == target) {
                SaveCarSnapshots(0);
            } else if (target < control->dynamics94.steering.count22) {
                if (RaceState < 4) {
                    LapVoicePending = -1;
                    RaceState = 4;
                    FadeCountdown = 75;
                    SoundSetMasterVolume(0, 33);
                    TotalRaceTime = 0;
                    SoundQueueEnqueue(19, 1);
                }
            }
        }
    }
    if (LapVoicePending == 0) {
        message = (9U - ((u32)NumLaps - control->dynamics94.steering.count22));
        if (message >= 6) {
            if (message == 9) {
                message = 15;
            }
            if (!(RaceVoiceFlags & (0x200U << (message - 6U)))) {
                SoundQueueEnqueue(message, 0);
                RaceVoiceFlags |= 0x200U << (message - 6U);
            }
        }
        LapVoicePending = -1;
    }
}
