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

    RaceVehicle *vehicle;
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
    if (GameModeType == 4)
    {
        ovl667_open_sio_events();
        ovl667_init_sio_session(0x7E900);
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
    InitCourseViewState();
    SetupRenderer(5);
    RaceSetupReadyFlag = 0;
    SplitIndex = 2;
    SplitTimes[1] = 0;
    SplitTimes[0] = 0;
    SplitTimes[2] = -1;
    SplitDistances[2] = (s32) TrackLength;
    third = TrackLength / 3;
    SplitDistances[0] = third;
    vectors = (u8 *) (&CourseBestLapTimes);
    SplitDistances[1] = (s32) (third * 2);
    vector = (s32 *) (((CourseIndex * 12) + (ReverseCourse * 96)) + vectors);
    BestSplitTimes[0] = (s32) vector[0];
    BestSplitTimes[1] = (s32) vector[1];
    RaceState = SavedRaceState;
    BestSplitTimes[2] = (s32) vector[2];
    if (GameModeType == 0)
    {
        countRoot = (u8 *)&CourseBestLap;

        offset0 = CourseIndex * 4;
        offset1 = ReverseCourse * 32;
        offset0 += offset1;
        selectedCount = (s32 *)(offset0+countRoot);
        goto publish_count;
    }
    if (GameModeType == 1)
    {
        otherRoot = (u8 *)&CourseBestLapRecords;
        /* Retain the full table address before forming its byte offset. */
        __asm__("" : : "r"(otherRoot));

        offset0 = CourseIndex * 4;
        selectedCount = (s32 *)(offset0+otherRoot);
    publish_count:
        BestLapTime = *selectedCount;

    }
    LapVoicePending = -1;

    if (GameModeType >= 2)
    {
        resetIndex = 0;
        if (GameModeType == 6)
        {
            goto publish_participants;
        }
    }
    else
    {
    publish_participants:
        courseIndex = CourseIndex;
        byteRoot = CourseLapCounts;

        if (GameModeType == 1)
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
