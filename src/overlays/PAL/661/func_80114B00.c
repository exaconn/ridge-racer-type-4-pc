#include "game/system/pad.h"
#include "rr/overlay661_race_controller.h"
#include "rr/frame_storage_types.h"
void ovl661_func_80114B00(void) {
    s32 var_a2;
    s32 var_a2_2;
    s16 temp_a0;
    s16 temp_a1_3;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 temp_s0_2;
    s32 temp_s0_4;
    s32 playerSoundOffset;
    s32 temp_v0_3;
    s32 var_a1;
    s32 var_s2;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s8 temp_v1;
    RaceVehicle *temp_s0;
    RaceVehicle *temp_s0_3;
    Rr661FrameVector16 *temp_v0;
    Rr661FrameVector16 *temp_v0_2;
    Rr661FrameVector16 *temp_v0_4;
    Rr661FrameVector16 *temp_v0_5;
    Rr661FrameVector16 *temp_v1_2;
    Rr661FrameVector16 *temp_v1_3;
    Rr661FrameVector16 *temp_v1_4;
    Rr661FrameVector16 *temp_v1_5;

    ModeFrame=ModeFrame+1U;
    if (((RaceState - 1U) < 2U) && (PlayerControllers[0].edges08.fields.pressed & 0x800)) {
        if (FadeCountdown <= 0) {
            FadeCountdown = 0xF;
            temp_v1 = RacePaused == 0;
            RacePaused = temp_v1;
            if (temp_v1 != 0) {
                SoundPause();
                PauseCursor = 0;
            } else {
                ApplyPauseMenuChoice(GameModeType);
                SoundResume();
            }
        } else {
            goto block_7;
        }
    } else if (FadeCountdown > 0) {
block_7:
        if (RaceState < 4) {
            FadeCountdown = (u16) FadeCountdown - 1;
        }
    }
    if (RacePaused != 0) {
        UpdatePauseMenu(GameModeType);
        ModeFrame=ModeFrame-1U;
        DrawPauseMenu(CurrentDrawBuf->buffer.secondaryOrderingB70.words, PauseCursor);
        if (RaceState < 4) {
            DrawStartCountdown(CurrentDrawBuf->buffer.secondaryOrderingB70.words, ModeFrame);
        }
        var_s2 = 0;
        if ((RaceState - 1U) < 3U) {
            do {

                temp_s0 = &PlayerCars[var_s2];
                DrawHudNumber3((temp_s0->input * 0xA0) / 1168, var_s2);
                DrawHudNumber5(*(s16 *)((u8 *)&PlayerDisplayRpm+var_s2*2), var_s2);
                DrawHudDigitMod10(temp_s0->control.previewSample84.halves.high, var_s2);
                UpdateHudRankDisplay(temp_s0->half1EE, var_s2);
                {s32 playerArg=var_s2;s32 budget=RaceTimeRemaining;__asm__("" : : "r"(budget));{s32 nextPlayer=playerArg+1;__asm__("" : : "r"(nextPlayer));var_s2=nextPlayer;DrawTimeRemainingForPlayer(budget,playerArg);}}
                DrawLapCounter(var_s2, temp_s0->control.dynamics94.steering.count22);
                DrawRaceMinimap(var_s2);
                DrawSteeringIndicator(CurrentDrawBuf->buffer.secondaryOrderingB70.words, var_s2);
                UpdateFinalLapHudSprite(var_s2);
            } while (var_s2 < 2);
            DrawSplitScreenDivider(0);
            DrawViewportBorderFrames(2);
            SubmitRaceHudPackets(2);
        }
        var_s2 = 0;
        do {
            var_v1 = var_s2 * 0x10;
            temp_v0 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1);
            temp_v1_2 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1);
        RR_661_FRAME_SCRATCH->rotation18 = *temp_v0;
        RR_661_FRAME_SCRATCH->position08 = *temp_v1_2;
            __asm__("" : "+r"(var_s2));
        SetupViewport(var_s2 + 2);
            if (RaceState == 0 && ModeFrame < 75U) {
                UpdateRaceIntroCamera(&PlayerCars[var_s2], ModeFrame);
                var_v1_2 = var_s2 * 0x10;
            } else {
                UpdateRaceChaseCamera(&PlayerCars[var_s2]);
                var_v1_2 = var_s2 * 0x10;
            }
            temp_v0_2 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1_2);
            temp_v1_3 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1_2);
__asm__("" : : "r"(temp_v0_2),"r"(temp_v1_3));
        *temp_v0_2 = RR_661_FRAME_SCRATCH->rotation18;
        *temp_v1_3 = RR_661_FRAME_SCRATCH->position08;
            temp_s0_2 = var_s2 + 1;
            BuildCameraMatrices();
            DrawCars(var_s2 + 2);
            SetupPlayerView(var_s2);
            DrawTrackObjects(0);
            DrawTrackSceneryForPlayer(temp_s0_2);
            var_s2 = temp_s0_2;
        } while (var_s2 < 2);
        return;
    }
    ScreenFrame=ScreenFrame+1U;
    if ((RaceState - 2U) < 2U) {
        temp_v0_3 = RaceTimeRemaining - 1U;
        RaceTimeRemaining = temp_v0_3;
        if (temp_v0_3 <= 0) {
            RaceState = 5;
            FadeCountdown = 0x4B;
            SoundSetMasterVolume(0,0x21);
        }
    }
    if ((ModeFrame >= 0x4BU) && (RaceState == 0)) {
        RaceState = 1;
        SlotsStartForRace();
        SetModelTexturePage(-1);
        ResetCarSlotBuffers();
        DrawBuffers[1].environment.draw00.background = 0;
        DrawBuffers[0].environment.draw00.background = 0;
    } else {
        if (RaceState==1) {
            if (ModeFrame >= 0xB0U) {
                UpdateCarStartLineSteering(&PlayerCars[0]);
                UpdateCarStartLineSteering(&PlayerCars[1]);
                CdAudioBeginPlay(1);
                RaceState = 2;
            }
        } else if (RaceState>=4) {
            var_a2 = 0x25;
            if (TotalRaceTime == -1) {
                var_a2 = 0x45;
            }
            if (FadeCountdown < 0x19) {
                AddScreenFadeTiles(CurrentDrawBuf->buffer.secondaryOrderingB70.words, (0x19 - FadeCountdown) * 0xA, var_a2);
                RR_661_FRAME_SCRATCH->pointers.cursor = AddDrawAreaPrim(CurrentDrawBuf->buffer.secondaryOrderingB70.words, RR_661_FRAME_SCRATCH->pointers.cursor, 0, 0, 0x140, 0x100);
            }
            if (FadeCountdown <= 0) {
                ovl661_D_80118448[0] = (u16) PlayerCars[0].half1EE;
                ovl661_D_80118448[1] = (u16) PlayerCars[1].half1EE;
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
        } else if (RaceState==0) {
            if (ModeFrame >= 51U) {
                AddScreenFadeTiles(CurrentDrawBuf->buffer.secondaryOrderingB70.words,(ModeFrame-50U)*10U,37);
            } else if (ModeFrame < 25U) {
                AddScreenFadeTiles(CurrentDrawBuf->buffer.secondaryOrderingB70.words,(25U-ModeFrame)*10U,69);
            }
            RR_661_FRAME_SCRATCH->pointers.cursor = AddDrawAreaPrim(CurrentDrawBuf->buffer.secondaryOrderingB70.words, RR_661_FRAME_SCRATCH->pointers.cursor, 0, 0, 0x140, 0x100);
        }
    }
    ovl661_func_80114924(&PlayerCars[0]);
    ovl661_func_80114924(&PlayerCars[1]);
    UpdateRaceRanking();
    if ((s16) PlayerCars[0].half1EE < (s16) PlayerCars[1].half1EE) {
        PlayerCars[0].half1EE = 0U;
        PlayerCars[1].half1EE = 1U;
    } else {
        PlayerCars[0].half1EE = 1U;
        PlayerCars[1].half1EE = 0U;
    }
    StepRaceFrame(GameModeType);
    if (RaceState < 4) {
        DrawStartCountdown(CurrentDrawBuf->buffer.secondaryOrderingB70.words, ModeFrame);
        PlayStartCountdownVoice(ModeFrame);
    }
    var_s2 = 0;
    if ((RaceState - 1U) < 3U) {
        do {

                temp_s0_3 = &PlayerCars[var_s2];
            DrawHudNumber3((temp_s0_3->input * 0xA0) / 1168, var_s2);
            DrawHudNumber5(*(s16 *)((u8 *)&PlayerDisplayRpm+var_s2*2), var_s2);
            DrawHudDigitMod10(temp_s0_3->control.previewSample84.halves.high, var_s2);
            UpdateHudRankDisplay(temp_s0_3->half1EE, var_s2);
            {s32 playerArg=var_s2;s32 budget=RaceTimeRemaining;__asm__("" : : "r"(budget));{s32 nextPlayer=playerArg+1;__asm__("" : : "r"(nextPlayer));var_s2=nextPlayer;DrawTimeRemainingForPlayer(budget,playerArg);}}
            DrawLapCounter(var_s2, temp_s0_3->control.dynamics94.steering.count22);
            DrawRaceMinimap(var_s2);
            DrawSteeringIndicator(CurrentDrawBuf->buffer.secondaryOrderingB70.words, var_s2);
            UpdateFinalLapHudSprite(var_s2);
        } while (var_s2 < 2);
        DrawSplitScreenDivider(0);
        DrawViewportBorderFrames(2);
        SubmitRaceHudPackets(2);
    }
    var_s2 = 0;
    do {
        var_v1_3 = var_s2 * 0x10;
        temp_v0_4 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1_3);
        temp_v1_4 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1_3);
        RR_661_FRAME_SCRATCH->rotation18 = *temp_v0_4;
        RR_661_FRAME_SCRATCH->position08 = *temp_v1_4;
        __asm__("" : "+r"(var_s2));
        SetupViewport(var_s2 + 2);
        if (RaceState == 0 && ModeFrame < 75U) {
            UpdateRaceIntroCamera(&PlayerCars[var_s2], ModeFrame);
            var_v1_4 = var_s2 * 0x10;
        } else {
            UpdateRaceChaseCamera(&PlayerCars[var_s2]);
            var_v1_4 = var_s2 * 0x10;
        }
        temp_v0_5 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1_4);
        temp_v1_5 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1_4);
__asm__("" : : "r"(temp_v0_5),"r"(temp_v1_5));
        *temp_v0_5 = RR_661_FRAME_SCRATCH->rotation18;
        *temp_v1_5 = RR_661_FRAME_SCRATCH->position08;
        playerSoundOffset = var_s2 * 2;
        BuildCameraMatrices();
        DrawCars(var_s2 + 2);
        if (PlayerCars[var_s2].half1EA != 0) {
            DrawRaceStartSignalSprite(var_s2 + 1);
        }
        RR_661_FRAME_SCRATCH->mode5E = 1;
        SetupPlayerView(var_s2);
        UpdateTrackObjects(0);
        DrawTrackObjects(0);
        temp_s0_4 = var_s2 + 1;
        DrawTrackSceneryForPlayer(temp_s0_4);
        {register s32 playerArg2 __asm__("$6")=var_s2;
        temp_a0 = *(s16 *)((u8 *)&PlayerDisplayRpm+playerSoundOffset);
        temp_a1_3 = *(s16 *)((u8 *)&PlayerThrottleLoad+playerSoundOffset);
        var_s2 = temp_s0_4;
        SlotSetLevelProgram(temp_a0,temp_a1_3,playerArg2);}
        __asm__("" : : "r"(playerSoundOffset));
    } while (var_s2 < 2);
    if (RaceState >= 4) {
        StopRaceRumble();
        if (RaceState == 6) {
            StopRaceAudioAndRumble();
        }
    }
}
