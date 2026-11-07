#include "rr/race_setup.h"

void SetupRace(void)
{
    u8 *vectors;
    register u8 *countRoot __asm__("$4");
    u8 *otherRoot,*byteRoot;
    s32 courseIndex;
    s32 minusOne;
    register s32 offset0 __asm__("$2");
    s32 offset1;

    RrRaceVehicle *vehicle;
    s32 *rank;
    s32 *selectedCount;
    s32 *result;
    s32 third;
    s32 vehicleIndex;
    register s32 resetIndex __asm__("$17");
    u8 *participant;
    u32 course;
    s32 vehicleFlags;
    s32 *vector;
    if (D_800F4D44 == 4)
    {
        func_80114988();
        func_80114AF8(0x7E900);
    }
    DrawSync(0);
    ApplyOptionsPadButtonMaps();
    InitRaceCars(D_800F4D44);
    if (((D_800F4D44 < 2) || (D_800F4D44 == 6)) || ((D_800F4D44 == 4) && (D_80107178 == 2)))
    {
        TrackEventSfxEnabled = 1;
    }
    else
    {
        TrackEventSfxEnabled = 0;
    }
    if (D_800F4D44 != 5)
    {
        vehicleIndex = 0;
        vehicle = PlayerCars;
        do
        {
            /* Keep the complete owner address through this traversal. */
            __asm__("" : "+r"(vehicle));
            vehicleFlags = (s16) vehicle->flags1E4;

            if (vehicleFlags != 0)
            {
                PadRumbleStart(vehicleIndex);
                vehicle->flags1E4 = (s16) (((u16) vehicle->flags1E4) ^ 0x100);
            }
            vehicleIndex += 1;
            vehicle++;
        }
        while (vehicleIndex < 2);
    }
    func_8006E794();
    SetupRenderer(5);
    D_8010C13C = 0;
    D_8010C12C = 2;
    D_8010C120[1] = 0;
    D_8010C120[0] = 0;
    D_8010C120[2] = -1;
    D_8010C140[2] = (s32) TrackLength;
    third = TrackLength / 3;
    D_8010C140[0] = third;
    vectors = (u8 *) (&CourseBestLapTimes);
    D_8010C140[1] = (s32) (third * 2);
    vector = (s32 *) (((CourseIndex * 12) + (ReverseCourse * 96)) + vectors);
    D_8010C0F8[0] = (s32) vector[0];
    D_8010C0F8[1] = (s32) vector[1];
    RaceState = SavedRaceState;
    D_8010C0F8[2] = (s32) vector[2];
    if (D_800F4D44 == 0)
    {
        countRoot = (u8 *)&CourseBestLap;

        offset0 = CourseIndex * 4;
        offset1 = ReverseCourse * 32;
        offset0 += offset1;
        selectedCount = (s32 *)(offset0+countRoot);
        goto publish_count;
    }
    if (D_800F4D44 == 1)
    {
        otherRoot = (u8 *)&D_800F3B00;
        /* Retain the full table address before forming its byte offset. */
        __asm__("" : : "r"(otherRoot));

        offset0 = CourseIndex * 4;
        selectedCount = (s32 *)(offset0+otherRoot);
    publish_count:
        BestLapTime = *selectedCount;

    }
    D_8010C134 = -1;

    if (D_800F4D44 >= 2)
    {
        resetIndex = 0;
        if (D_800F4D44 == 6)
        {
            goto publish_participants;
        }
    }
    else
    {
    publish_participants:
        courseIndex = CourseIndex;
        byteRoot = CourseLapCounts;

        if (D_800F4D44 == 1)
        {
            courseIndex += 8;
        }
        participant = courseIndex + byteRoot;
        NumLaps = (s32) (*participant);
        /* Initialized-value metadata retains the original store order. */
        __asm__("" : : "m"(NumLaps));
        resetIndex = 0;
    }
    minusOne = -1;

    rank = LapTimes;
    result = LapFrameCounts;
    do
    {
        *result = 0;
        *rank = minusOne;
        rank++;
        resetIndex += 1;
        result++;
    }
    while (resetIndex < 0xA);
    TotalRaceTime = -1;
    course = (u16) CourseIndex;
    if (((course - 2U) < 3U) || (((s16) course) == 7))
    {
        D_800ABEF0 = 1;
    }
    else
    {
        D_800ABEF0 = 0;
    }
    func_80045830();
    if ((D_800F4D44 == 0) || (D_800F4D44 == 5))
    {
        D_800AD50C = 0;
        D_800AC538 = 0;
        D_800ACBF8 = 0;
    }
    else
    {
        func_80070440();
        D_800AD50C = 1;
        D_800AC538 = 0;
        D_800ACBF8 = 1;
    }
    BuildCameraMatrices();
    InitRaceHudPackets(D_800F4D44);
    InitHudNumberTargets(D_800F4D44);
    ScreenFrame = 0;
    ModeFrame = 0;
    D_800AC5D0 = 0;
    RacePaused = 0;
    PauseCursor = 0;
    D_80107190 = 0x80;
    RaceTimeRemaining = 0x3A98;
    if ((D_800F4D44 != 1) && (D_800F4D44 != 6))
    {
        RaceTimeRemaining = 0xAFC8;
    }
    VSyncThreshold = 0x180;
    ClearTrackObjectStates(0);
    D_800F4C70.first = 0;
    D_800F4C70.second = 0;
    D_800F2AB4.first = 0;
    D_800F2AB4.second = 0;
    RaceVoiceFlags = 0;
    StopAllSoundAndReset();
    SelectCourseBgmTrack();
    SetModelTexturePage(0xA);

}
