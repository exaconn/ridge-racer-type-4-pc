#include "rr/overlay666.h"

void ovl666_update_checkpoint(RrRaceCheckpointVehicle *vehicle) {
    RrRaceVehicleControl *control;
    /* Preserve the target through the mode-dependent adjustment. */
    s32 target;
    s32 id;

    control = &vehicle->control;
    if ((s32)((u32)vehicle->second + (u32)vehicle->first) >=
        (s32)((u32)(s32)control->steering94.count22 * (u32)TrackLength)) {
        if (control->steering94.count22 <= NumLaps) {
            control->steering94.count22 = (s16)((u16)control->steering94.count22 + 1U);
            D_8010C134 = 0;
            target = ((u16)(D_800F3914 - 6) < 2) ? (s32)((u32)NumLaps - 1U) : NumLaps;
            if (control->steering94.count22 == target) {
                SaveCarSnapshots(0);
            } else if (control->steering94.count22 > NumLaps) {
                if (RaceState < 4) {
                    D_8010C134 = -1;
                    RaceState = 4;
                    D_800AC5D0 = 75;
                    SoundSetMasterVolume(0, 33);
                    D_800F4C64 = 0;
                    SoundQueueEnqueue(19, 1);
                }
            }
        }
    }
    if (D_8010C134 == 0) {
        id = (s32)(9U - ((u32)NumLaps - (u32)(s32)control->steering94.count22));
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
    func_80036068();
    func_8003CAB4();
    D_800F4C6A = 1;
    D_800F3A26 = 82;
    D_800F4D8A = 0;
    func_800207D4(D_800ACC14->ordering.ordering70, 255, 69);
}

void ovl666_update_race(void) {
    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    SetupViewport(0);
    if (((u32)RaceState - 1) < 2U && (D_800F3A38[0].buttons08 & 0x800) && D_800AC5D0 <= 0) {
        D_800AC5D0 = 15;
        D_800F4C68 = (D_800F4C68 == 0);
        if (D_800F4C68 != 0) {
            SoundPause();
            D_800F2FDC = 0;
        } else {
            func_800348E8(D_800F4D44);
            SoundResume();
        }
    } else if (D_800AC5D0 > 0) {
        if (RaceState < 4) {
            D_800AC5D0--;
        }
    }

    if (D_800F4C68 != 0) {
        func_80034844(D_800F4D44);
        D_800F2DE4 = (u32)D_800F2DE4 - 1;
        func_800349F4(&D_800ACC14->ordering.ordering70[0], D_800F2FDC);
        if (RaceState < 4) {
            func_80033F3C(&D_800ACC14->ordering.ordering70[0], D_800F2DE4);
        }
        if (((u32)RaceState - 1) < 3U) {
            func_80021350(D_800F4C70.first, 0);
            func_80021684((PlayerCars[0].input * 160) / 1168, 0);
            func_800219BC(PlayerCars[0].half27A, 0);
            func_80021A7C(PlayerCars[0].half1EE, 0);
            func_8002BD18(D_800AC5A4, 0);
            func_80021C94(0, PlayerCars[0].steering288.count22);
            func_80021E2C(0);
            func_80022098(&D_800ACC14->ordering.ordering70[0], 0);
            func_80021C00(1);
            func_800211A4(0);
            func_80020B10(4);
        }
        if (RaceState == 0 && D_800F2DE4 < 75U) {
            func_80033474(PlayerCars, D_800F2DE4);
        } else {
            SetModelTexturePage(-1);
            func_80033BE4((struct w3g2p *)(void *)PlayerCars);
        }
        BuildCameraMatrices();
        DrawCars(0);
        func_8006ECE8(0);
        DrawTrackObjects(0);
        func_80036FD4(0);
        func_80070734(ScreenFrame);
        return;
    }

    ScreenFrame = (u32)ScreenFrame + 1;
    if (((u32)RaceState - 2) < 2U) {
        if ((D_800AC5A4 = (u32)D_800AC5A4 - 1) <= 0) {
            RaceState = 5;
            D_800AC5D0 = 75;
            SoundSetMasterVolume(0, 33);
        }
    }

    if (D_800F2DE4 >= 75U && RaceState == 0) {
        RaceState = 1;
        func_8004FED4();
        func_800172DC();
        SetModelTexturePage(-1);
    } else if (RaceState == 1) {
        if (D_800F2DE4 >= 176U) {
            func_80022CD4(PlayerCars);
            RaceState = 2;
            CdAudioBeginPlay(1);
        }
    } else if (RaceState >= 4) {
        s32 a2;
        if (D_800F4C64 == -1 || PlayerCars[0].half1EE != 0) {
            a2 = 69;
        } else {
            a2 = 37;
        }
        if (D_800AC5D0 < 25) {
            func_800207D4(&D_800ACC14->ordering.ordering70[0], (25 - D_800AC5D0) * 10, a2);
            RR_666_SCRATCH->cursor = AddDrawAreaPrim(&D_800ACC14->ordering.ordering70[0],
                                                   RR_666_SCRATCH->cursor, 0, 0, 320, 256);
        }
        if (D_800AC5D0 <= 0) {
            if (RaceState == 4 && (ovl666_result_kind = PlayerCars[0].half1EE) == 0) {
                D_800F4C6A = 1;
                D_800F3A26 = 21;
                D_800F4D8A = 0;
            } else {
                func_8003660C();
            }
        } else {
            D_800AC5D0--;
        }
    } else if (RaceState == 0) {
        if (D_800F2DE4 >= 51U) {
            func_800207D4(&D_800ACC14->ordering.ordering70[0], (D_800F2DE4 - 50) * 10, 37);
            D_800ADAF0[1].environment.rgb24 = 0;
            D_800ADAF0[0].environment.rgb24 = 0;
        } else if (D_800F2DE4 < 25U) {
            func_800207D4(&D_800ACC14->ordering.ordering70[0], (25 - D_800F2DE4) * 10, 69);
            D_800ADAF0[1].environment.rgb24 = 1;
            D_800ADAF0[0].environment.rgb24 = 1;
        }
        RR_666_SCRATCH->cursor = AddDrawAreaPrim(&D_800ACC14->ordering.ordering70[0],
                                               RR_666_SCRATCH->cursor, 0, 0, 320, 256);
    }

    ovl666_update_checkpoint((RrRaceCheckpointVehicle *)(void *)PlayerCars);
    UpdateRaceRanking();
    if (RaceState < 4) {
        func_80033F3C(&D_800ACC14->ordering.ordering70[0], D_800F2DE4);
        func_80034630(D_800F2DE4);
    }
    CheckFinalLapEvent((char *)PlayerCars);
    StepRaceFrame(D_800F4D44);
    if (((u32)RaceState - 1) < 3U) {
        func_80021350(D_800F4C70.first, 0);
        func_80021684((PlayerCars[0].input * 160) / 1168, 0);
        func_800219BC(PlayerCars[0].half27A, 0);
        func_80021A7C(PlayerCars[0].half1EE, 0);
        func_8002BD18(D_800AC5A4, 0);
        func_80021C94(0, PlayerCars[0].steering288.count22);
        func_80021E2C(0);
        func_80022098(&D_800ACC14->ordering.ordering70[0], 0);
        func_80021C00(1);
        func_800211A4(0);
        func_80020B10(4);
    }
    if (RaceState == 0 && D_800F2DE4 < 75U) {
        func_80033474(PlayerCars, D_800F2DE4);
    } else {
        func_80033BE4((struct w3g2p *)(void *)PlayerCars);
    }
    BuildCameraMatrices();
    DrawCars(0);
    if (PlayerCars[0].half1EA != 0) {
        func_800225DC(0);
    }
    func_8006ECE8(0);
    UpdateTrackObjects(0);
    DrawTrackObjects(0);
    func_80036FD4(0);
    func_80070734(ScreenFrame);
    func_8005007C(D_800F4C70.first, D_800F2AB4.first, 0);
    if (RaceState >= 4) {
        func_80036030();
        if (RaceState == 6) {
            func_80036068();
        }
    }
}
