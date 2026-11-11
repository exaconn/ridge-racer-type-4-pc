#include "rr/replay_types.h"
#include "rr/frame_storage_types.h"
#include "rr/vehicle_table.h"
/* RODATA 0x80010420 0x20 */
#include "scratchpad.h"
#include "game/gfx/scene1.h"
#include "game/sound/sndcmd.h"
#include "game/ui/hudmap2.h"
#include "game/gfx/gfxdraw.h"
#include "game/sound/sndvab.h"
#include "game/system/pad.h"
#include "game/system/pad3.h"
#include "game/race/objclr.h"
#include "game/race/objsim.h"
#include "game/race/objdraw.h"
#include "game/race/raceai2.h"
#include "game/race/racesave.h"
#include "game/data/screenframe.h"
#include "rr/record_banks.h"
#include "rr/screen_tiles.h"
#include "game/ui/badgespr.h"
#include "game/car/carbound.h"
#include "game/race/racemai2.h"
#include "game/sound/cdstart.h"
#include "game/math/rotmat.h"
#include "game/camera/viewcam.h"
#include "rr/trig.h"
#include "game/sound/sndctrl.h"
#include "game/gfx/gtepoly1.h"
#include "game/menu/mnchoice.h"
#include "game/sound/sndslot.h"

typedef struct {
    short h0;
    short h2;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    short h14;
    short h16;
    short h18;
    short h1A;
    short h1C;
    short h1E;
    short h20;
    short h22;
    short h24;
    short h26;
    short h28;
    short h2A;
    short h2C;
    short h2E;
} S30;

typedef struct {
    short h0;
    short s12;
    short s14;
    short reserved06[5];
} S10_sub;

typedef struct {
    int reserved00[4];
    S10_sub sub[2];
} S_AD528;

extern int ReplayState;
extern int SavedRaceState;
extern int RaceState;
extern short NumCars;
extern short TwistRangeLimits[];
extern int ReplayFrame;
extern int LinkRemoteCarInputs[];
extern int LinkLocalCarInputs[];
extern short LinkHostFlag;
extern S_AD528 Options;
extern unsigned char CarInputs[][4];
extern S30 PadButtonMap[];
extern int LinkPendingCarInputs[];
extern short GameModeType;
extern short FadeCountdown;
extern int TrackLength;
extern int NumLaps;
extern int MenuReturnSubmodeIndex;
extern RaceVehicle PlayerCars[2];
extern short ModeIndex;
extern short ModeGroup;
extern short SubmodeState;
extern int ReplayFocusCarIndex;
extern int RaceQuitTargetMode;
extern short PlayerThrottleLoad[];
extern int ModeFrame;
extern short PlayerDisplayRpm[];

void ReadCarInput(RaceVehicle *arg0, s32 arg1);
void ReplayModeEnter();
void ReplayModeUpdate(void);

/* Callers explicitly supply this legacy argument; the body ignores it. */
void SaveCarSnapshots(int unused)
{
  int i;
  if (ReplayState == 0)
  {
    SavedRaceState = RaceState;
    for (i = 0; i < NumCars; i++)
      *CarSnapshots[i] = *CarTable[i];
    ReplayState = 1;
  }
}

void ReadCarInput(RaceVehicle *arg0, s32 arg1) {
    RaceVehicleControl *control;
    short t3;
    short t5;
    short t1;
    short a1;
    int idx;
    int t2;    int b;
    int f;
    int mode;

    control = &arg0->control;
    if (RaceState != 0) {
        if (ReplayState == 2) {
            if (ReplayFrame < ReplayLength) {
                *(ReplayFrame4 *)CarInputs[arg1] = ReplayInputs[arg1][ReplayFrame];
                if (GameModeType == 4) {
                    if (arg1 == 3) {
                        ReplayFrame += 1;
                    }
                } else if (arg1 == 1 || GameModeType < 2 || GameModeType == 6) {
                    ReplayFrame += 1;
                }
            }
        } else {
            t3 = 0;
            t2 = idx = arg1 & 1;
            if (RaceState < 4) {
                mode = PlayerControllers[idx].protocol10;
                if (mode == 0) {
                    t1 = 0;
                    if (PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h4) {
                        t1 = 0xFF;
                    }
                    b = (PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h6) != 0;
                    {
                        int f1, tand;
                        tand = PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h0;
                        f1 = tand != 0;
                        if ((PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h2) > 0) {
                            f1 |= 2;
                        }
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h8) > 0) {
                            f1 |= 4;
                        }
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].hA) > 0) {
                            f1 |= 8;
                        }
                        f = f1;
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].hE) > 0) {
                            f |= 0x10;
                        }
                    }
                    if (PlayerControllers[idx].input00.fields.type == 0x73) {
                        t5 = (PlayerControllers[idx].steering3C * 0x7F) / 128;
                        t3 = f | 0x80;
                    } else {
                        t5 = 0;
                        t3 = f | 0x20;
                    }
                    control->dynamics94.player.digitalThrottle1C += b * 4;
                    if (b == 0) {
                        control->dynamics94.player.digitalThrottle1C = 0;
                    }
                    if (control->dynamics94.player.digitalThrottle1C >= 0x74) {
                        control->dynamics94.player.digitalThrottle1C = 0x73;
                    }
                    a1 = control->dynamics94.player.digitalThrottle1C + (b * 140);
                } else if (mode == 1) {
                    t5 = (PlayerControllers[idx].steering3C * 0x7F) / TwistRangeLimits[Options.sub[idx].s12];
                    t1 = 0;
                    if (PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h14) {
                        t1 = 0xFF;
                    }
                    a1 = 0;
                    if (PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h16) {
                        a1 = 0xFF;
                    }
                    {
                        int fm1;
                        fm1 = ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h18) != 0) * 4;
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h1A) > 0) {
                            fm1 |= 8;
                        }
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h1E) > 0) {
                            fm1 |= 0x10;
                        }
                        t3 = fm1 | 0x40;
                    }
                    switch (Options.sub[idx].s14) {
                    case 0:
                    case 5: {
                        int i0;
                        t1 = (PlayerControllers[({ i0 = t2;i0; })].pressure22 * 0xFF) / 106;
                        a1 = (PlayerControllers[i0].pressure24 * 0xFF) / 106;
                        break;
                    }
                    case 1:
                    case 6: {
                        int i1;
                        t1 = (PlayerControllers[({ i1 = t2;i1; })].pressure24 * 0xFF) / 106;
                        a1 = (PlayerControllers[i1].pressure22 * 0xFF) / 106;
                        break;
                    }
                    case 2: {
                        int i2;
                        a1 = (PlayerControllers[({ i2 = t2;i2; })].pressure26 * 0xFF) / 106;
                        break;
                    }
                    case 3: {
                        int i3;
                        t1 = (PlayerControllers[({ i3 = t2;i3; })].pressure24 * 0xFF) / 106;
                        a1 = (PlayerControllers[i3].pressure26 * 0xFF) / 106;
                        break;
                    }
                    case 4:
                    case 7:
                        break;
                    }
                } else if (mode == 2) {
                    t5 = (PlayerControllers[idx].steering3C * 0x7F) / 128;
                    t1 = 0;
                    if (PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h24) {
                        t1 = 0xFF;
                    }
                    b = (PlayerControllers[idx].input00.fields.buttons & PadButtonMap[idx].h26) != 0;
                    control->dynamics94.player.digitalThrottle1C += b * 4;
                    if (b == 0) {
                        control->dynamics94.player.digitalThrottle1C = 0;
                    }
                    if (control->dynamics94.player.digitalThrottle1C >= 0x74) {
                        control->dynamics94.player.digitalThrottle1C = 0x73;
                    }
                    a1 = control->dynamics94.player.digitalThrottle1C + (b * 140);
                    {
                        unsigned short u2;                        int tand;                        int fb, fc;
                        u2 = PlayerControllers[idx].input00.fields.buttons;
                        tand = u2 & PadButtonMap[idx].h20;
                        f = tand != 0;
                        if ((u2 & PadButtonMap[idx].h22) > 0) {
                            f |= 2;
                        }
                        fb = f;
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h28) > 0) {
                            fb |= 4;
                        }
                        fc = fb;
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h2A) > 0) {
                            fc |= 8;
                        }
                        if ((PlayerControllers[idx].edges08.fields.pressed & PadButtonMap[idx].h2E) > 0) {
                            fc |= 0x10;
                        }
                        t3 = fc | 0x60;
                    }
                } else {
                    a1 = 0;
                    t1 = 0;
                }
            } else {
                t1 = 0;
                a1 = 0;
            }
            CarInputs[arg1][0] = t5;
            CarInputs[arg1][1] = t1;
            CarInputs[arg1][2] = a1;
            CarInputs[arg1][3] = t3;
            if (GameModeType == 4) {
                if (LinkHostFlag != 0) {
                    if (arg1 >= 2) {
                        *(int *)CarInputs[arg1] = LinkRemoteCarInputs[t2];
                    } else {
                        LinkPendingCarInputs[t2] = *(int *)CarInputs[arg1];
                        *(int *)CarInputs[arg1] = LinkLocalCarInputs[t2];
                    }
                } else {
                    if (arg1 < 2) {
                        *(int *)CarInputs[arg1] = LinkRemoteCarInputs[t2];
                    } else {
                        LinkPendingCarInputs[t2] = *(int *)CarInputs[arg1];
                        *(int *)CarInputs[arg1] = LinkLocalCarInputs[t2];
                    }
                };                if (ReplayState == 1) {
                    if (ReplayFrame < ReplayLength) {
                        ReplayInputs[arg1][ReplayFrame] = *(ReplayFrame4 *)CarInputs[arg1];
                        if (arg1 == 3) {
                            ReplayFrame += 1;
                        }
                    }
                }
            } else if (ReplayState == 1) {
                if (ReplayFrame < ReplayLength) {
                    ReplayInputs[arg1][ReplayFrame] = *(ReplayFrame4 *)CarInputs[arg1];
                    if (arg1 == 1 || GameModeType < 2 || GameModeType == 6) {
                        ReplayFrame += 1;
                    }
                }
            }
        }
    }
}

void CheckRaceFinish(RaceVehicle *vehicle) {
    RaceVehicleControl *control = &vehicle->control;
    int a1 = control->dynamics94.steering.count22;
    if (vehicle->second + vehicle->first >= a1 * TrackLength) {
        unsigned short u = (u16)control->dynamics94.steering.count22;
        if (NumLaps >= a1) {
            control->dynamics94.steering.count22 = u + 1;
            if (NumLaps < (short)(u + 1)) {
                if (RaceState < 4) {
                    if (FadeCountdown == -1) {
                        RaceState = 4;
                        FadeCountdown = 0x4B;
                        SoundSetMasterVolume(0, 0x47);
                    }
                }
            }
        }
    }
    if (ReplayFrame >= ReplayLength - 0x4B && FadeCountdown == -1) {
        FadeCountdown = 0x25;
        SoundSetMasterVolume(0, 0x21);
    } else if (MenuReturnSubmodeIndex != 0) {
        if (FadeCountdown == -1) {
            FadeCountdown = 0x25;
            SoundSetMasterVolume(0, 0x21);
        }
    }
}

void ReplayModeEnter(void) {
    SoundInitForRace();
    SelectReplayBgmTrack();
    AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], 0xFF, 0x25);
    SetupRaceMemoryArena(0, 2);
    RaceState = SavedRaceState;
    ClearTrackObjectStates(1);
    SetModelTexturePage(10);
    ReplayFocusCarIndex = 0;
    if (((unsigned short)GameModeType - 2) < 2U) {
        if (PlayerCars[0].first < PlayerCars[1].first) ReplayFocusCarIndex = 1;
    }
    FadeCountdown = -1;
    ModeGroup = 1;
    ModeIndex = 0x16;
    SubmodeState = 0;
}

void ReplayModeUpdate(void) {
    int i;
    short t;
    ModeFrame += 1;
    if (ReplayFrame < 0x1A) {
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x19 - ReplayFrame) * 10, 0x25);
        if (ReplayFrame == 10) {
            CdAudioBeginPlay(0);
        }
    } else {
        UpdateAndDrawSceneOverlay();
    }
    ScreenFrame += 1;
    if (FadeCountdown >= 0) {
        if (FadeCountdown == 0) {
            SetReverbDepthBoth(0);
            FinishRace();
        }
        t = (unsigned short)FadeCountdown - 1;
        FadeCountdown = t;
        if (t < 0x19) {
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x19 - t) * 10, 0x45);
        }
    }
    if (rsin((ReplayFrame << 7) & 0xF80) > 0) {
        { int tt = *(int *)SCRATCHPAD; *(int *)SCRATCHPAD = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.words[0], tt, 0x10, 0x10, 0x4C, 0xC, 0x64, 0xC0, 5, 0x7FF8, 0, 0x80); }
    }
    for (i = 0; i < NumCars; i++) {
        if ((s16)CarTable[i]->flags1E4 >= 0x10) {
            CheckRaceFinish(CarTable[i]);
        }
    }
    StepRaceFrame(GameModeType);
    if (((unsigned short)GameModeType - 2) < 2U) {
        if ((unsigned char)ReplayFrame == 0) {
            if (PlayerCars[0].first < PlayerCars[1].first) {
                ReplayFocusCarIndex = 1;
            } else {
                ReplayFocusCarIndex = 0;
            }
        }
    }
    SetupViewport(0);
    UpdateRaceCamera(2, &PlayerCars[ReplayFocusCarIndex]);
    UpdateTrackObjects(0);
    BuildCameraMatrices();
    DrawCars(6);
    ((int *)SCRATCHPAD)[0x78 / 4] = RaceQuitTargetMode;
    SetupPlayerView(0);
    DrawTrackObjects(0);
    DrawTrackSceneryForPlayer(0);
    if (ReplayFrame >= 0x19) {
        QueueDrawStp();
    }
    SlotSetLevelProgram(PlayerDisplayRpm[0], PlayerThrottleLoad[0], 0);
    SlotSetLevelProgram(PlayerDisplayRpm[1], PlayerThrottleLoad[1], 1);
    PadClearPulse(0);
    PadClearPulse(1);
    PadRumbleStop(0);
    PadRumbleStop(1);
}
