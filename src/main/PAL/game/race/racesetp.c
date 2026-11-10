#include "rr/race_setup.h"

void SetupRace(void)
{
    u8 *vectors;
    u8 *otherRoot,*byteRoot;
    s32 courseIndex;
    s32 minusOne;
    s32 offset1;

    RaceVehicle *vehicle;
    s32 *rank;
    s32 *selectedCount;
    s32 *result;
    s32 third;
    s32 vehicleIndex;
    u8 *participant;
    u32 course;
    s32 vehicleFlags;
    s32 *vector;
    if (GameModeType == 4)
    {
        func_80114988();
        func_80114AF8(0x7E900);
    }
    DrawSync(0);
    ApplyOptionsPadButtonMaps();
    InitRaceCars(GameModeType);
    if (((GameModeType < 2) || (GameModeType == 6)) || ((GameModeType == 4) && (LinkPlayerCount == 2)))
    {
        TrackEventSfxEnabled = 1;
    }
    else
    {
        TrackEventSfxEnabled = 0;
    }
    if (GameModeType != 5)
    {
        for (vehicleIndex = 0; vehicleIndex < 2; vehicleIndex++)
        {
            if ((s16)PlayerCars[vehicleIndex].flags1E4 != 0)
            {
                PadRumbleStart(vehicleIndex);
                PlayerCars[vehicleIndex].flags1E4 ^= 0x100;
            }
        }
    }
    InitCourseViewState();
    SetupRenderer(5);
    D_8010C13C = 0;
    SplitIndex = 2;
    SplitTimes[1] = 0;
    SplitTimes[0] = 0;
    SplitTimes[2] = -1;
    SplitDistances[2] = TrackLength;
    third = TrackLength / 3;
    SplitDistances[0] = third;
    vectors = (u8 *) (&CourseBestLapTimes);
    SplitDistances[1] = (third * 2);
    vector = (s32 *) (((CourseIndex * 12) + (ReverseCourse * 96)) + vectors);
    BestSplitTimes[0] = vector[0];
    BestSplitTimes[1] = vector[1];
    RaceState = SavedRaceState;
    BestSplitTimes[2] = vector[2];
    if (GameModeType == 0)
    {
        BestLapTime = (&CourseBestLap[CourseIndex])[ReverseCourse * 8];
    }
    else if (GameModeType == 1)
    {
        BestLapTime = CourseBestLapRecords[CourseIndex];
    }
    LapVoicePending = -1;

    if ((GameModeType < 2) || (GameModeType == 6))
    {
        courseIndex = CourseIndex;
        byteRoot = CourseLapCounts;

        if (GameModeType == 1)
        {
            courseIndex += 8;
        }
        participant = courseIndex + byteRoot;
        NumLaps = (*participant);
    }
    vehicleIndex = 0;
    minusOne = -1;

    rank = LapTimes;
    result = LapFrameCounts;
    do
    {
        *result = 0;
        *rank = minusOne;
        rank++;
        vehicleIndex += 1;
        result++;
    }
    while (vehicleIndex < 0xA);
    TotalRaceTime = -1;
    course = (u16) CourseIndex;
    if (((course - 2U) < 3U) || (((s16) course) == 7))
    {
        ForceTailLightTrails = 1;
    }
    else
    {
        ForceTailLightTrails = 0;
    }
    ResetLensFlareState();
    if ((GameModeType == 0) || (GameModeType == 5))
    {
        RearViewMirrorEnabled = 0;
        ChaseViewEnabled = 0;
        DrawSubOtFlag = 0;
    }
    else
    {
        InitRearViewMirrorState();
        RearViewMirrorEnabled = 1;
        ChaseViewEnabled = 0;
        DrawSubOtFlag = 1;
    }
    BuildCameraMatrices();
    InitRaceHudPackets(GameModeType);
    InitHudNumberTargets(GameModeType);
    ScreenFrame = 0;
    ModeFrame = 0;
    FadeCountdown = 0;
    RacePaused = 0;
    PauseCursor = 0;
    RaceStartWipeY = 0x80;
    RaceTimeRemaining = 0x3A98;
    if ((GameModeType != 1) && (GameModeType != 6))
    {
        RaceTimeRemaining = 0xAFC8;
    }
    VSyncThreshold = 0x180;
    ClearTrackObjectStates(0);
    PlayerDisplayRpm.first = 0;
    PlayerDisplayRpm.second = 0;
    PlayerThrottleLoad.first = 0;
    PlayerThrottleLoad.second = 0;
    RaceVoiceFlags = 0;
    StopAllSoundAndReset();
    SelectCourseBgmTrack();
    SetModelTexturePage(0xA);

}
