#include "game/system/pad.h"
#include "rr/overlay666.h"
#include "rr/frame_storage_types.h"

void ovl666_update_checkpoint(RaceVehicle *vehicle) {
    RaceVehicleControl *control;
    /* Preserve the target through the mode-dependent adjustment. */
    s32 target;
    s32 id;

    control = &vehicle->control;
    if ((s32)((u32)vehicle->second + vehicle->first) >=
        (s32)((u32)(s32)control->dynamics94.steering.count22 * TrackLength)) {
        if (control->dynamics94.steering.count22 <= NumLaps) {
            control->dynamics94.steering.count22 = (s16)((u16)control->dynamics94.steering.count22 + 1U);
            D_8010C134 = 0;
            target = ((u16)(CourseIndex - 6) < 2) ? (NumLaps - 1U) : NumLaps;
            if (control->dynamics94.steering.count22 == target) {
                SaveCarSnapshots(0);
            } else if (control->dynamics94.steering.count22 > NumLaps) {
                if (RaceState < 4) {
                    D_8010C134 = -1;
                    RaceState = 4;
                    D_800AC5D0 = 75;
                    SoundSetMasterVolume(0, 33);
                    TotalRaceTime = 0;
                    SoundQueueEnqueue(19, 1);
                }
            }
        }
    }
    if (D_8010C134 == 0) {
        id = (9U - ((u32)NumLaps - control->dynamics94.steering.count22));
        if (id == 9) {
            id = 15;
        }
        if (id >= 6) {
            SoundQueueEnqueue(id, 0);
        }
        D_8010C134 = -1;
    }
}

void ovl666_exit_race(void) {
    StopRaceAudioAndRumble();
    SetupRaceAndSceneId();
    ModeGroup = 1;
    D_800F3A26 = 82;
    D_800F4D8A = 0;
    AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, 255, 69);
}

void ovl666_update_race(void) {
    ModeFrame = ModeFrame + 1U;
    SetupViewport(0);
    if ((RaceState - 1U) < 2U && (PlayerControllers[0].edges08.fields.pressed & 0x800) && D_800AC5D0 <= 0) {
        D_800AC5D0 = 15;
        RacePaused = (RacePaused == 0);
        if (RacePaused != 0) {
            SoundPause();
            PauseCursor = 0;
        } else {
            ApplyPauseMenuChoice(GameModeType);
            SoundResume();
        }
    } else if (D_800AC5D0 > 0) {
        if (RaceState < 4) {
            D_800AC5D0--;
        }
    }

    if (RacePaused != 0) {
        UpdatePauseMenu(GameModeType);
        ModeFrame = ModeFrame - 1U;
        DrawPauseMenu(&CurrentDrawBuf->ordering.ordering70.words[0], PauseCursor);
        if (RaceState < 4) {
            DrawStartCountdown(&CurrentDrawBuf->ordering.ordering70.words[0], ModeFrame);
        }
        if ((RaceState - 1U) < 3U) {
            DrawHudRotatingQuad(D_800F4C70.first, 0);
            DrawHudNumber3((PlayerCars[0].input * 160) / 1168, 0);
            DrawHudDigitMod10(PlayerCars[0].control.previewSample84.halves.high, 0);
            UpdateHudRankDisplay(PlayerCars[0].half1EE, 0);
            DrawTimeRemainingForPlayer(RaceTimeRemaining, 0);
            DrawLapCounter(0, PlayerCars[0].control.dynamics94.steering.count22);
            DrawRaceMinimap(0);
            DrawSteeringIndicator(&CurrentDrawBuf->ordering.ordering70.words[0], 0);
            UpdateFinalLapHudSprite(1);
            DrawViewportBorderFrames(0);
            SubmitRaceHudPackets(4);
        }
        if (RaceState == 0 && ModeFrame < 75U) {
            UpdateRaceIntroCamera(PlayerCars, ModeFrame);
        } else {
            SetModelTexturePage(-1);
            UpdateRaceChaseCamera((struct w3g2p *)(void *)PlayerCars);
        }
        BuildCameraMatrices();
        DrawCars(0);
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
            D_800AC5D0 = 75;
            SoundSetMasterVolume(0, 33);
        }
    }

    if (ModeFrame >= 75U && RaceState == 0) {
        RaceState = 1;
        SlotsStartForRace();
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
        if (TotalRaceTime == -1 || PlayerCars[0].half1EE != 0) {
            a2 = 69;
        } else {
            a2 = 37;
        }
        if (D_800AC5D0 < 25) {
            AddScreenFadeTiles(&CurrentDrawBuf->ordering.ordering70.words[0], (25 - D_800AC5D0) * 10, a2);
            RR_666_SCRATCH->cursor = AddDrawAreaPrim(&CurrentDrawBuf->ordering.ordering70.words[0],
                                                   RR_666_SCRATCH->cursor, 0, 0, 320, 256);
        }
        if (D_800AC5D0 <= 0) {
            if (RaceState == 4 && (ovl666_result_kind = PlayerCars[0].half1EE) == 0) {
                ModeGroup = 1;
                D_800F3A26 = 21;
                D_800F4D8A = 0;
            } else {
                FinishRace();
            }
        } else {
            D_800AC5D0--;
        }
    } else if (RaceState == 0) {
        if (ModeFrame >= 51U) {
            AddScreenFadeTiles(&CurrentDrawBuf->ordering.ordering70.words[0], (ModeFrame - 50U) * 10U, 37);
            D_800ADAF0[1].environment.draw00.background = 0;
            D_800ADAF0[0].environment.draw00.background = 0;
        } else if (ModeFrame < 25U) {
            AddScreenFadeTiles(&CurrentDrawBuf->ordering.ordering70.words[0], (25U - ModeFrame) * 10U, 69);
            D_800ADAF0[1].environment.draw00.background = 1;
            D_800ADAF0[0].environment.draw00.background = 1;
        }
        RR_666_SCRATCH->cursor = AddDrawAreaPrim(&CurrentDrawBuf->ordering.ordering70.words[0],
                                               RR_666_SCRATCH->cursor, 0, 0, 320, 256);
    }

    ovl666_update_checkpoint((RaceVehicle *)(void *)PlayerCars);
    UpdateRaceRanking();
    if (RaceState < 4) {
        DrawStartCountdown(&CurrentDrawBuf->ordering.ordering70.words[0], ModeFrame);
        PlayStartCountdownVoice(ModeFrame);
    }
    CheckFinalLapEvent((char *)PlayerCars);
    StepRaceFrame(GameModeType);
    if ((RaceState - 1U) < 3U) {
        DrawHudRotatingQuad(D_800F4C70.first, 0);
        DrawHudNumber3((PlayerCars[0].input * 160) / 1168, 0);
        DrawHudDigitMod10(PlayerCars[0].control.previewSample84.halves.high, 0);
        UpdateHudRankDisplay(PlayerCars[0].half1EE, 0);
        DrawTimeRemainingForPlayer(RaceTimeRemaining, 0);
        DrawLapCounter(0, PlayerCars[0].control.dynamics94.steering.count22);
        DrawRaceMinimap(0);
        DrawSteeringIndicator(&CurrentDrawBuf->ordering.ordering70.words[0], 0);
        UpdateFinalLapHudSprite(1);
        DrawViewportBorderFrames(0);
        SubmitRaceHudPackets(4);
    }
    if (RaceState == 0 && ModeFrame < 75U) {
        UpdateRaceIntroCamera(PlayerCars, ModeFrame);
    } else {
        UpdateRaceChaseCamera((struct w3g2p *)(void *)PlayerCars);
    }
    BuildCameraMatrices();
    DrawCars(0);
    if (PlayerCars[0].half1EA != 0) {
        DrawRaceStartSignalSprite(0);
    }
    SetupPlayerView(0);
    UpdateTrackObjects(0);
    DrawTrackObjects(0);
    DrawTrackSceneryForPlayer(0);
    UpdateAndDrawRearViewMirror(ScreenFrame);
    SlotSetLevelProgram(D_800F4C70.first, D_800F2AB4.first, 0);
    if (RaceState >= 4) {
        StopRaceRumble();
        if (RaceState == 6) {
            StopRaceAudioAndRumble();
        }
    }
}
