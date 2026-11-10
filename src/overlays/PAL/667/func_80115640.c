#include "rr/vehicle_table.h"
#include "rr/frame_storage_types.h"
#include "types.h"
#include "game/race/racegrid.h"
#include "rr/race_types.h"

/* Versus (serial link) race frame: drives the shared race state machine,
 * exchanges pause/menu input through the serial packet bytes and renders
 * one or two viewports. */

typedef struct {
    s32 words[4];
} Ovl667Vector16;

typedef struct {
    u32 pad00[6];
    u8 enable18;
    u8 pad19[0x22778 - 0x19];
} Ovl667FrameStorage;

extern s16 LinkPlayerCount;
extern s32 RaceStartWipeY;
extern u32 ModeFrame;
extern s32 RaceState;
extern s32 TotalRaceTime;
extern s16 FadeCountdown;
extern s16 NumCars;
extern s32 RaceTimeRemaining;
extern s32 ScreenFrame;
extern s16 LinkHostFlag;
extern u16 Pad0Buttons;
extern s8 RacePaused;
extern s8 PauseCursor;
extern s16 GameModeType;
extern s16 PlayerDisplayRpm[];
extern s16 PlayerThrottleLoad[];
extern u16 LinkPrevCarRanks[];
extern RaceVehicle PlayerCars[];

extern Ovl667FrameStorage DrawBuffers[2];

extern s32 ovl667_link_shutdown_stage;
extern s32 ovl667_pause_packet_slot;
extern u8 ovl667_active_slot_index, ovl667_consumed_slot_index;
extern u8 ovl667_pending_pause_bits, ovl667_remote_pause_bits, ovl667_local_pause_bits;
extern Ovl667Vector16 ovl667_camera_rotations[];
extern Ovl667Vector16 ovl667_camera_positions[];

extern void SetModelTexturePage();
extern char *AddDrawAreaPrim();
extern char *AddTilePrim();
extern void AddScreenFadeTiles();
extern void SubmitRaceHudPackets();
extern void DrawViewportBorderFrames();
extern void DrawHudRotatingQuad();
extern void DrawHudNumber3();
extern void DrawHudNumber5();
extern void DrawHudDigitMod10();
extern void UpdateHudRankDisplay();
extern void UpdateFinalLapHudSprite();
extern void DrawLapCounter();
extern void DrawRaceMinimap();
extern void DrawSteeringIndicator();
extern void DrawSplitScreenDivider();
extern void DrawRaceStartSignalSprite();
extern void UpdateCarStartLineSteering();
extern void DrawTimeRemainingForPlayer();
extern void DrawCars();
extern void UpdateRaceIntroCamera();
extern void UpdateRaceChaseCamera();
extern void PlayStartCountdownVoice();
extern void InitPlayerRumble();
extern void ApplyPauseMenuChoice();
extern void DrawPauseMenu();
extern void StopRaceRumble();
extern void StopRaceAudioAndRumble();
extern void FinishRace();
extern void DrawTrackSceneryForPlayer();
extern void StepRaceFrame();
extern void UpdateRaceRanking();
extern void PlaySfx();
extern void SoundPause();
extern void SoundResume();
extern void SoundSetMasterVolume();
extern void SlotsStartForRace();
extern void SlotSetLevelProgram();
extern void CdAudioBeginPlay();
extern void BuildCameraMatrices();
extern void SetupPlayerView();
extern void SetupViewport();
extern void UpdateAndDrawRearViewMirror();
extern void UpdateTrackObjects();
extern void DrawTrackObjects();
extern void SetDispMask();
extern void _sio_control();
extern void ovl667_close_sio_session(void);
extern void ovl667_receive_peer_inputs(void);
extern void ovl667_transmit_local_inputs(void);
extern void ovl667_abort_on_link_timeout(void);
extern void ovl667_update_checkpoint();

typedef struct {
    char *cursor;
} Ovl667Scratch;

#define SCRATCH ((Ovl667Scratch *)0x1F800000)

void ovl667_update_race(void) {
    s32 split;
    char *frame;
    s32 fade;
    s32 i;
    s32 n;
    s32 state;
    u16 buttons;

    split = (LinkPlayerCount != 2) << 1;
    if (split == 0) {
        frame = &CurrentDrawBuf->buffer.ordering70.words[0];
    } else {
        *(s16 *)0x1F80005E = 1;
        frame = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
    }
    if (ModeFrame < 2) {
        SetDispMask(0);
    } else {
        SetDispMask(1);
    }
    SetModelTexturePage(-1);
    if (ovl667_link_shutdown_stage == 2) {
        ovl667_close_sio_session();
        ovl667_link_shutdown_stage = 3;
    } else if (ovl667_link_shutdown_stage == 3) {
        _sio_control(1, 1, 3);
    }
    ovl667_receive_peer_inputs();

    if (ModeFrame >= 75 && RaceState == 0) {
        RaceState = 1;
        SlotsStartForRace();
    } else if (RaceState == 1) {
        if (ModeFrame >= 176) {
            for (i = 0; i < 4; i++) {
                UpdateCarStartLineSteering(CarTable[i]);
            }
            RaceState = 2;
            CdAudioBeginPlay(1);
            FadeCountdown = 30;
        }
    } else if (RaceState >= 4) {
        fade = 37;
        if (TotalRaceTime == -1) {
            fade = 69;
        }
        if (FadeCountdown < 25) {
            AddScreenFadeTiles(frame, (25 - FadeCountdown) * 10, fade);
            SCRATCH->cursor = AddDrawAreaPrim(frame, SCRATCH->cursor, 0, 0, 320, 256);
        }
        if (FadeCountdown <= 0) {
            if (ovl667_link_shutdown_stage != 3) {
                ovl667_close_sio_session();
            }
            for (i = 0; i < NumCars; i++) {
                LinkPrevCarRanks[i] = (u16)CarTable[i]->half1EE;
            }
            FinishRace();
            _sio_control(1, 1, 3);
        } else {
            FadeCountdown--;
        }
    } else if (ModeFrame >= 51 && RaceState == 0) {
        AddScreenFadeTiles(frame, (ModeFrame - 50) * 10, 37);
        SCRATCH->cursor = AddDrawAreaPrim(frame, SCRATCH->cursor, 0, 0, 320, 256);
    }

    for (i = 0; i < NumCars; i++) {
        ovl667_update_checkpoint(CarTable[i]);
    }
    UpdateRaceRanking();

    if (ovl667_active_slot_index != ovl667_consumed_slot_index) {
        buttons = Pad0Buttons;
        ovl667_pending_pause_bits = (buttons & 0x800) && FadeCountdown == 0;
        ovl667_pending_pause_bits |= (buttons & 0x1000) ? 2 : 0;
        ovl667_pending_pause_bits |= (buttons & 0x4000) ? 4 : 0;
        state = RaceState;
        if ((state - 1) < 3U) {
            if ((ovl667_local_pause_bits | ovl667_remote_pause_bits) & 1) {
                RacePaused ^= 1;
                ovl667_pause_packet_slot = ovl667_active_slot_index;
                if (RacePaused == 0) {
                    FadeCountdown = 10;
                    SoundResume();
                    ApplyPauseMenuChoice(GameModeType);
                    if (PauseCursor != 0) {
                        ovl667_close_sio_session();
                        StopRaceAudioAndRumble();
                        return;
                    }
                } else {
                    SoundPause();
                }
                PauseCursor = 0;
            } else if (FadeCountdown > 0 && state < 4) {
                FadeCountdown--;
            }
        }
        if (RacePaused == 0) {
            ModeFrame++;
            ScreenFrame++;
            if ((RaceState - 2) < 2U) {
                if (--RaceTimeRemaining <= 0) {
                    RaceState = 5;
                    FadeCountdown = 75;
                    SoundSetMasterVolume(0, 33);
                }
            }
            StepRaceFrame(4);
        } else {
            if ((ovl667_local_pause_bits | ovl667_remote_pause_bits) & 2) {
                if (PauseCursor > 0) {
                    PlaySfx(0);
                    PauseCursor--;
                }
            }
            if ((ovl667_local_pause_bits | ovl667_remote_pause_bits) & 4) {
                if (PauseCursor < 2) {
                    PlaySfx(0);
                    PauseCursor++;
                }
            }
            DrawPauseMenu(frame, PauseCursor);
            InitPlayerRumble();
        }
        ovl667_consumed_slot_index = ovl667_active_slot_index;
    } else {
        ovl667_pending_pause_bits = 0;
    }

    n = 1;
    ovl667_transmit_local_inputs();
    if (split != 0) {
        n = 2;
    }
    if (RaceState < 4) {
        DrawStartCountdown(frame, ModeFrame);
        if (RacePaused == 0) {
            PlayStartCountdownVoice(ModeFrame);
        }
    }
    if ((RaceState - 1) < 3U) {
        for (i = 0; i < n; i++) {
            if (i == 1 && PlayerCars[i].half1E8 == -1) {
                SCRATCH->cursor = AddTilePrim(frame, SCRATCH->cursor, 0, 128, 320, 128, 8, 8, 8);
            }
            if (n == 1) {
                if (PlayerCars[i].half1E8 == -1) {
                    continue;
                }
                DrawHudRotatingQuad(PlayerDisplayRpm[i], i);
                DrawRaceMinimap(0);
                DrawLapCounter(0, PlayerCars[i].control.dynamics94.steering.count22);
                DrawSteeringIndicator(frame, 0);
            } else {
                DrawHudNumber5(PlayerDisplayRpm[i], i);
                DrawRaceMinimap(i + 1);
                DrawLapCounter(i + 1, PlayerCars[i].control.dynamics94.steering.count22);
                DrawSteeringIndicator(frame, i + 1);
            }
            DrawHudNumber3((PlayerCars[i].input * 160) / 1168, i);
            DrawHudDigitMod10(PlayerCars[i].control.previewSample84.halves.high, i);
            UpdateHudRankDisplay((s16)PlayerCars[i].half1EE, i);
            DrawTimeRemainingForPlayer(RaceTimeRemaining, i);
            UpdateFinalLapHudSprite(i + 1);
            if (i == 1) {
                DrawSplitScreenDivider(0);
            }
        }
        DrawViewportBorderFrames(split);
        SubmitRaceHudPackets(3);
    }

    for (i = 0; i < n; i++) {
        if (PlayerCars[i].half1E8 != -1) {
            *(Ovl667Vector16 *)0x1F800018 = ovl667_camera_rotations[i];
            *(Ovl667Vector16 *)0x1F800008 = ovl667_camera_positions[i];
            SetupViewport(split + i);
            if (RaceState == 0 && ModeFrame < 75) {
                UpdateRaceIntroCamera(&PlayerCars[i], ModeFrame);
            } else {
                UpdateRaceChaseCamera(&PlayerCars[i]);
            }
            ovl667_camera_rotations[i] = *(Ovl667Vector16 *)0x1F800018;
            ovl667_camera_positions[i] = *(Ovl667Vector16 *)0x1F800008;
            BuildCameraMatrices();
            DrawCars(i + ((s16)(LinkHostFlag ^ 1) + 1) * 2);
            if (PlayerCars[i].half1EA != 0 && RacePaused == 0) {
                DrawRaceStartSignalSprite(n == 1 ? 0 : i + 1);
            }
            SetupPlayerView(i);
            UpdateTrackObjects(0);
            DrawTrackObjects(0);
            DrawTrackSceneryForPlayer(i + 1);
            if (n == 1) {
                UpdateAndDrawRearViewMirror(ScreenFrame);
            }
            if (RaceState == 0 && n == 1) {
                DrawBuffers[1].enable18 = n;
                DrawBuffers[0].enable18 = n;
                SCRATCH->cursor = AddDrawAreaPrim(frame + 0xAF8, SCRATCH->cursor, 0, RaceStartWipeY, 320,
                                               256 - (RaceStartWipeY << n));
                if (i == 0) {
                    if ((RaceStartWipeY -= 16) < 0) {
                        RaceStartWipeY = 0;
                        DrawBuffers[1].enable18 = 0;
                        DrawBuffers[0].enable18 = 0;
                    }
                }
            }
            SlotSetLevelProgram(PlayerDisplayRpm[i], PlayerThrottleLoad[i], i);
            if (i == 0) {
                ovl667_transmit_local_inputs();
            }
        }
    }

    if (RaceState >= 4) {
        StopRaceRumble();
        if (RaceState == 6) {
            StopRaceAudioAndRumble();
        }
    }
    if (ovl667_link_shutdown_stage == 0) {
        ovl667_abort_on_link_timeout();
    }
}
