#include "game/system/pad.h"
#include "rr/overlay659.h"
#include "rr/frame_storage_types.h"

void ovl659_exit_race(void) {
    SetupRaceAndSceneId();
    ModeGroup = 1;
    ModeIndex = 0x2F;
    SubmodeState = 0;
}

void ovl659_update_race(void) {
    ModeFrame = ModeFrame + 1U;
    SetupViewport(0);
    if ((RaceState - 1U) < 2U && (PlayerControllers[0].edges08.fields.pressed & 0x800) && FadeCountdown <= 0) {
        FadeCountdown = 15;
        RacePaused = (RacePaused == 0);
        if (RacePaused != 0) {
            SoundPause();
            PauseCursor = 0;
        } else {
            ApplyPauseMenuChoice(GameModeType);
            SoundResume();
        }
    } else if (FadeCountdown > 0) {
        if (RaceState < 4) {
            FadeCountdown--;
        }
    }

    if (RacePaused != 0) {
        UpdatePauseMenu(GameModeType);
        ModeFrame = ModeFrame - 1U;
        DrawRaceOverlaySprites();
        DrawPauseMenu(&CurrentDrawBuf->ordering.ordering70.words[0], PauseCursor);
        DrawLapTimeList(0);
        DrawTimeRemaining(RaceTimeRemaining);
        DrawStartCountdown(&CurrentDrawBuf->ordering.ordering70.words[0], ModeFrame);
        DrawHudRotatingQuad(PlayerDisplayRpm.first, 0);
        DrawViewportBorderFrames(0);
        DrawHudNumber3((PlayerCars[0].input * 160) / 1168, 0);
        DrawHudDigitMod10(PlayerCars[0].control.previewSample84.halves.high, 0);
        UpdateHudRankDisplay(PlayerCars[0].half1EE, 0);
        DrawRaceMinimap(0);
        DrawSteeringIndicator(&CurrentDrawBuf->ordering.ordering70.words[0], 0);
        UpdateFinalLapHudSprite(0);
        SubmitRaceHudPackets(GameModeType);
        BuildCameraMatrices();
        DrawCars(0);
        RR_659_SCRATCH->word78 = RaceQuitTargetMode;
        SetupPlayerView(0);
        DrawTrackObjects(0);
        DrawTrackSceneryForPlayer(0);
        UpdateAndDrawRearViewMirror(ScreenFrame);
        return;
    }

    ScreenFrame = ScreenFrame + 1U;
    if ((RaceState - 2U) < 2U) {
        if ((RaceTimeRemaining = RaceTimeRemaining - 1U) <= 0) {
            RaceState = 5;
            PlayerCars[0].half1EE = 9;
            FadeCountdown = 37;
            RankEntries = (0U - TrackLength);
            SoundSetMasterVolume(0, 33);
        }
    }

    if (RaceIntroCameraState != 0 && RaceState == 0) {
        ScreenFrame = 75;
        ModeFrame = 75;
        SlotsStartForRace();
        RaceState = 1;
        PlayerCars[0].flags1E4 |= 0x100;
        ResetCarSlotBuffers();
        SetModelTexturePage(-1);
    } else if (RaceState == 1) {
        if (ModeFrame >= 176U) {
            UpdateCarStartLineSteering(PlayerCars);
            RaceState = 2;
            CdAudioBeginPlay(1);
        }
    } else if (RaceState >= 4) {
        s32 a2;
        if (FadeCountdown < 25) {
            if (CarTable[0]->half1EE <= ovl659_finish_rank_limits[SelectionState.phase17 >> 1]) {
                a2 = 37;
            } else {
                a2 = 69;
            }
            AddScreenFadeTiles(&CurrentDrawBuf->ordering.ordering70.words[0], (25 - FadeCountdown) * 10, a2);
        }
        if (FadeCountdown <= 0) {
            SelectionState.result19 = PlayerCars[0].half1EE;
            if (PlayerCars[0].half1EE <= ovl659_finish_rank_limits[SelectionState.phase17 / 2] &&
                RaceState == 4) {
                ModeGroup = 1;
                ModeIndex = 21;
                SubmodeState = 0;
            } else {
                FinishRace();
            }
        } else {
            FadeCountdown--;
        }
    }

    UpdateLapTiming(PlayerCars);
    if (RaceState < 4) {
        DrawStartCountdown(&CurrentDrawBuf->ordering.ordering70.words[0], ModeFrame);
        PlayStartCountdownVoice(ModeFrame);
    }
    UpdateRaceRanking();
    CheckFinalLapEvent((char *)PlayerCars);
    if ((RaceState - 1U) < 3U) {
        DrawLapTimeList(0);
        DrawTimeRemaining(RaceTimeRemaining);
        DrawHudRotatingQuad(PlayerDisplayRpm.first, 0);
        DrawViewportBorderFrames(0);
        DrawHudNumber3((PlayerCars[0].input * 160) / 1168, 0);
        DrawHudDigitMod10(PlayerCars[0].control.previewSample84.halves.high, 0);
        UpdateHudRankDisplay(PlayerCars[0].half1EE, 0);
        DrawRaceMinimap(0);
        DrawSteeringIndicator(&CurrentDrawBuf->ordering.ordering70.words[0], 0);
        UpdateFinalLapHudSprite(0);
        SubmitRaceHudPackets(GameModeType);
    }
    StepRaceFrame(GameModeType);
    if (RaceState == 0) {
        DrawLensFlareAndSun(PlayerCars);
    } else if (RaceState == 7) {
        UpdateFinishingPlayerCar(&PlayerCars[1]);
    } else {
        UpdateRaceChaseCamera((struct w3g2p *)(void *)PlayerCars);
    }
    UpdateTrackObjects(0);
    BuildCameraMatrices();
    DrawCars(0);
    if (PlayerCars[0].half1EA != ReverseCourse) {
        DrawRaceStartSignalSprite(0);
    }
    RR_659_SCRATCH->word78 = RaceQuitTargetMode;
    SetupPlayerView(0);
    DrawTrackObjects(0);
    DrawTrackSceneryForPlayer(0);
    UpdateAndDrawRearViewMirror(ScreenFrame);
    if (RaceState == 0) {
        RR_659_SCRATCH->pointers.cursor = AddDrawAreaPrim(
            CurrentDrawBuf->ordering.ordering70.words + 2, RR_659_SCRATCH->pointers.cursor, 0, 0, 320, 256);
        DrawBuffers[1].environment.draw00.background = 1;
        DrawBuffers[0].environment.draw00.background = 1;
        RR_659_SCRATCH->pointers.cursor =
            AddDrawAreaPrim(CurrentDrawBuf->ordering.ordering70.words + 702, RR_659_SCRATCH->pointers.cursor, 0,
                          RaceStartWipeY, 320, (256U - RaceStartWipeY * 2U));
        if ((RaceStartWipeY = (RaceStartWipeY - 16U)) < 0) {
            RaceStartWipeY = 0;
            DrawBuffers[1].environment.draw00.background = 0;
            DrawBuffers[0].environment.draw00.background = 0;
        }
    }
    SlotSetLevelProgram(PlayerDisplayRpm.first, PlayerThrottleLoad.first, 0);
    if (RaceState >= 4) {
        StopRaceRumble();
        if (RaceState == 6) {
            StopRaceAudioAndRumble();
        }
    }
}
