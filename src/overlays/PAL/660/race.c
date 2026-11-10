#include "game/system/pad.h"
#include "rr/overlay660.h"
#include "rr/frame_storage_types.h"

void ovl660_check_reverse_run(RaceVehicle *player){
 RaceVehicleControl *control=&player->control;
 if(RaceState-2U<2){
  s32 allow=0;
  if(control->dynamics94.steering.count22<=0)allow=player->half1EA!=ReverseCourse;
  else if(player->first<=(s32)(0U-TrackLength))allow=1;
  if(allow==1){RaceState=5;FadeCountdown=75;SoundSetMasterVolume(0,71);}
 }
}

void ovl660_exit_race(void)
{
    StopRaceAudioAndRumble();
    SetupRaceAndSceneId();
    ModeGroup = 1;
    ModeIndex = 53;
    SubmodeState = 0;
    AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words, 255, 69);
}

void ovl660_update_race(void) {
    s32 fadeFlags;
    s8 paused;

    ModeFrame = (ModeFrame + 1U);
    if (((RaceState - 1U) < 2U) && (PlayerControllers[0].edges08.fields.pressed & 0x800)) {
        if (FadeCountdown <= 0) {
            FadeCountdown = 0xF;
            paused = RacePaused == 0;
            RacePaused = paused;
            if (paused != 0) {
                SoundPause();
                PauseCursor = 0;
            } else {
                ApplyPauseMenuChoice(GameModeType);
                SoundResume();
            }
        } else {
            goto update_pause_timer;
        }
    } else if (FadeCountdown > 0) {
update_pause_timer:
        if (RaceState < 4) {
            FadeCountdown = (u16) FadeCountdown - 1;
        }
    }
    if (RacePaused != 0) {
        UpdatePauseMenu(GameModeType);
        ModeFrame = (ModeFrame - 1U);
        DrawPauseMenu(CurrentDrawBuf->ordering.ordering70.words, PauseCursor);
        DrawRaceOverlaySprites(1);
        DrawLapTimeList(0);
        DrawStartCountdown(CurrentDrawBuf->ordering.ordering70.words, ModeFrame);
        DrawHudRotatingQuad(PlayerDisplayRpm.first, 0);
        DrawViewportBorderFrames(0);
        DrawHudNumber3((PlayerCars[0].input * 0xA0) / 1168, 0);
        DrawHudDigitMod10(PlayerCars[0].control.previewSample84.halves.high, 0);
        DrawRaceMinimap(0);
        DrawSteeringIndicator(CurrentDrawBuf->ordering.ordering70.words, 0);
        SubmitRaceHudPackets(GameModeType);
        BuildCameraMatrices();
        SetupViewport(0);
        DrawCars(0);
        RR_660_FRAME_SCRATCH->word78 = D_800F2A64;
        SetupPlayerView(0);
        DrawTrackObjects(0);
        DrawTrackSceneryForPlayer(0);
        UpdateAndDrawRearViewMirror(ScreenFrame);
        return;
    }
    ScreenFrame = (ScreenFrame + 1U);
    if (ModeFrame >= 75U && RaceState == 0) {
        SlotsStartForRace();
        RaceState=1;
        ResetCarSlotBuffers();
        SetModelTexturePage(-1);
    } else if (RaceState == 1) {
            if (ModeFrame >= 0xB0U) {
                UpdateCarStartLineSteering(PlayerCars);
                RaceState = 2;
                CdAudioBeginPlay(1);
            }
    } else if (RaceState >= 4) {
            if (FadeCountdown < 0x19) {
                fadeFlags = 0x45;
                if (RaceState == 4) {
                    fadeFlags = 0x25;
                }
                AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (0x19 - FadeCountdown) * 0xA, fadeFlags);
            }
            if (FadeCountdown <= 0) {
                if (RaceState == 4) {
                    ModeGroup = 1;
                    ModeIndex = 0x15;
                    SubmodeState = 0;
                } else {
                    FinishRace();
                }
            } else {
                FadeCountdown = (u16) FadeCountdown - 1;
            }
    } else if (RaceState == 0) {
            if (ModeFrame >= 0x33U) {
                AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (ModeFrame - 0x32U) * 0xAU, 0x25);
                DrawBuffers[1].environment.draw00.background = 0;
                DrawBuffers[0].environment.draw00.background = 0;
            } else if (ModeFrame < 0x19U) {
                AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (0x19U - ModeFrame) * 0xAU, 0x45);
                DrawBuffers[1].environment.draw00.background = 1;
                DrawBuffers[0].environment.draw00.background = 1;
            }
            RR_660_FRAME_SCRATCH->pointers.cursor = AddDrawAreaPrim(CurrentDrawBuf->ordering.ordering70.words, RR_660_FRAME_SCRATCH->pointers.cursor, 0, 0, 0x140, 0x100);
    }
    UpdateLapTiming(PlayerCars);
    DrawRaceOverlaySprites(0);
    ovl660_check_reverse_run(PlayerCars);
    UpdateRaceRanking();
    if (RaceState < 4) {
        DrawStartCountdown(CurrentDrawBuf->ordering.ordering70.words, ModeFrame);
        PlayStartCountdownVoice(ModeFrame);
    }
    CheckFinalLapEvent((char *)(void *)PlayerCars);
    if ((RaceState - 1U) < 3U) {
        DrawLapTimeList(0);
        DrawHudRotatingQuad(PlayerDisplayRpm.first, 0);
        DrawViewportBorderFrames(0);
        DrawHudNumber3((PlayerCars[0].input * 0xA0) / 1168, 0);
        DrawHudDigitMod10(PlayerCars[0].control.previewSample84.halves.high, 0);
        DrawRaceMinimap(0);
        DrawSteeringIndicator(CurrentDrawBuf->ordering.ordering70.words, 0);
        SubmitRaceHudPackets(GameModeType);
    }
    StepRaceFrame(GameModeType);
    UpdateTrackObjects(0);
    if (RaceState == 0 && ModeFrame < 0x4BU) {
        UpdateRaceIntroCamera(PlayerCars, ModeFrame);
    } else {
        UpdateRaceChaseCamera((struct w3g2p *)(void *)PlayerCars);
    }
    BuildCameraMatrices();
    DrawCars(0);
    if (PlayerCars[0].half1EA != ReverseCourse) {
        if (RaceState < 4) {
            DrawRaceStartSignalSprite(0);
        }
    }
    RR_660_FRAME_SCRATCH->word78 = D_800F2A64;
    SetupPlayerView(0);
    DrawTrackObjects(0);
    DrawTrackSceneryForPlayer(0);
    SlotSetLevelProgram(PlayerDisplayRpm.first, PlayerThrottleLoad.first, 0);
    if (RaceState >= 4) {
        StopRaceRumble();
        if (RaceState == 6) {
            StopRaceAudioAndRumble();
        }
    }
}
