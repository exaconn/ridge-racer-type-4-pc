#include "game/system/pad.h"
/* RODATA 0x8001134c 0x14 */
#include "types.h"
#include "scratchpad.h"
#include "game/race/objrec.h"
#include "game/gfx/polytri.h"
#include "game/gfx/poly1.h"
#include "game/race/objclr.h"
#include "game/race/objsim.h"
#include "rr/overlay662_distance.h"
#include "rr/overlay661_init.h"
#include "rr/overlay668_camera.h"
#include "rr/audio_loop.h"

extern int D_800A4090;
extern int ModeFrame;
extern short CourseIndex;
extern int RaceState;
extern int D_8010E7A8;
extern int D_8010E7AC;
extern int D_8010E7B0;
extern int D_8010E7B4;
extern int D_8010E7B8;
extern int D_8010E7C8[];

s32 InterpolateObjectKeyframe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 *sp = (s32 *)(SCRATCHPAD + 0x7C);
    s32 d1, w, d2;

    sp[14] = arg2;
    sp[10] = arg4;
    sp[11] = arg5;
    sp[12] = arg6;
    sp[13] = arg7;

    if ((TrackObjectKeyframes[arg1].f08 % 10) == 8 &&
        (arg1 + 1) < (TrackObjectDefs[arg0].f28 + TrackObjectDefs[arg0].f24) &&
        (TrackObjectStates[arg0].f0 + 10) > (TrackObjectKeyframes[arg1].f00 * 6)) {
        d1 = (TrackObjectKeyframes[arg1 + 1].f00 - TrackObjectKeyframes[arg1].f00) * 6;
        w = TrackObjectKeyframes[arg1].f00 * 6 - 10;
        w = TrackObjectStates[arg0].f0 - w;
        sp[0] = (TrackObjectKeyframes[arg1].f0C - TrackObjectKeyframes[arg1 - 1].f0C) / sp[11];
        sp[1] = (TrackObjectKeyframes[arg1].f10 - TrackObjectKeyframes[arg1 - 1].f10) / sp[11];
        sp[2] = (TrackObjectKeyframes[arg1].f14 - TrackObjectKeyframes[arg1 - 1].f14) / sp[11];
        sp[3] = (TrackObjectKeyframes[arg1 + 1].f0C - TrackObjectKeyframes[arg1].f0C) / d1;
        sp[4] = (TrackObjectKeyframes[arg1 + 1].f10 - TrackObjectKeyframes[arg1].f10) / d1;
        sp[5] = (TrackObjectKeyframes[arg1 + 1].f14 - TrackObjectKeyframes[arg1].f14) / d1;
        sp[6] = ((sp[0] * (20 - w)) >> 1) + ((sp[3] * w) >> 1);
        sp[7] = ((sp[1] * (20 - w)) >> 1) + ((sp[4] * w) >> 1);
        sp[8] = ((sp[2] * (20 - w)) >> 1) + ((sp[5] * w) >> 1);
        TrackObjectStates[arg0].fC += sp[6] / 10;
        TrackObjectStates[arg0].f10 += sp[7] / 10;
        TrackObjectStates[arg0].f14 += sp[8] / 10;
    } else if ((arg1 - 1) >= TrackObjectDefs[arg0].f24 &&
               (TrackObjectKeyframes[arg1 - 1].f08 % 10) == 8 &&
               (TrackObjectStates[arg0].f0 - 10) <= (TrackObjectKeyframes[arg1 - 1].f00 * 6)) {
        d2 = (TrackObjectKeyframes[arg1 - 1].f00 - TrackObjectKeyframes[arg1 - 2].f00) * 6;
        sp[0] = (TrackObjectKeyframes[arg1].f0C - TrackObjectKeyframes[arg1 - 1].f0C) / sp[11];
        sp[1] = (TrackObjectKeyframes[arg1].f10 - TrackObjectKeyframes[arg1 - 1].f10) / sp[11];
        sp[2] = (TrackObjectKeyframes[arg1].f14 - TrackObjectKeyframes[arg1 - 1].f14) / sp[11];
        sp[3] = (TrackObjectKeyframes[arg1 - 1].f0C - TrackObjectKeyframes[arg1 - 2].f0C) / d2;
        sp[4] = (TrackObjectKeyframes[arg1 - 1].f10 - TrackObjectKeyframes[arg1 - 2].f10) / d2;
        sp[5] = (TrackObjectKeyframes[arg1 - 1].f14 - TrackObjectKeyframes[arg1 - 2].f14) / d2;
        sp[6] = ((sp[0] * (sp[10] + 9)) >> 1) + ((sp[3] * (11 - sp[10])) >> 1);
        sp[7] = ((sp[1] * (sp[10] + 9)) >> 1) + ((sp[4] * (11 - sp[10])) >> 1);
        sp[8] = ((sp[2] * (sp[10] + 9)) >> 1) + ((sp[5] * (11 - sp[10])) >> 1);
        TrackObjectStates[arg0].fC += sp[6] / 10;
        TrackObjectStates[arg0].f10 += sp[7] / 10;
        TrackObjectStates[arg0].f14 += sp[8] / 10;
    } else {
        TrackObjectStates[arg0].fC = ((((((TrackObjectKeyframes[arg1].f0C - TrackObjectKeyframes[arg1 - 1].f0C) / sp[14]) * (100 - TrackObjectKeyframes[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13] + TrackObjectKeyframes[arg1 - arg3].f0C;
        TrackObjectStates[arg0].f10 = ((((((TrackObjectKeyframes[arg1].f10 - TrackObjectKeyframes[arg1 - 1].f10) / sp[14]) * (100 - TrackObjectKeyframes[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13] + TrackObjectKeyframes[arg1 - arg3].f10;
        TrackObjectStates[arg0].f14 = ((((((TrackObjectKeyframes[arg1].f14 - TrackObjectKeyframes[arg1 - 1].f14) / sp[14]) * (100 - TrackObjectKeyframes[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13] + TrackObjectKeyframes[arg1 - arg3].f14;
    }

    TrackObjectStates[arg0].f1C = TrackObjectKeyframes[arg1 - arg3].f1C + ((((((TrackObjectKeyframes[arg1].f1C - TrackObjectKeyframes[arg1 - 1].f1C) / sp[14]) * (100 - TrackObjectKeyframes[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13];
    TrackObjectStates[arg0].f1E = TrackObjectKeyframes[arg1 - arg3].f1E + ((((((TrackObjectKeyframes[arg1].f1E - TrackObjectKeyframes[arg1 - 1].f1E) / sp[14]) * (100 - TrackObjectKeyframes[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13];
    TrackObjectStates[arg0].f20 = TrackObjectKeyframes[arg1 - arg3].f20 + ((((((TrackObjectKeyframes[arg1].f20 - TrackObjectKeyframes[arg1 - 1].f20) / sp[14]) * (100 - TrackObjectKeyframes[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13];

    if (TrackObjectKeyframes[arg1].f04 != 0) {
        if ((TrackObjectKeyframes[arg1].f08 % 10) == 4 || (TrackObjectKeyframes[arg1].f08 % 10) == 5) {
            TrackObjectStates[arg0].fC += ((((TrackObjectKeyframes[arg1].f0C - TrackObjectKeyframes[arg1 - 1].f0C) / sp[14]) * sp[10]) / sp[11]) * TrackObjectKeyframes[arg1].f04 / 100;
            TrackObjectStates[arg0].f10 += ((((TrackObjectKeyframes[arg1].f10 - TrackObjectKeyframes[arg1 - 1].f10) / sp[14]) * sp[10]) / sp[11]) * TrackObjectKeyframes[arg1].f04 / 100;
            TrackObjectStates[arg0].f14 += ((((TrackObjectKeyframes[arg1].f14 - TrackObjectKeyframes[arg1 - 1].f14) / sp[14]) * sp[10]) / sp[11]) * TrackObjectKeyframes[arg1].f04 / 100;
            TrackObjectStates[arg0].f1C += ((((TrackObjectKeyframes[arg1].f1C - TrackObjectKeyframes[arg1 - 1].f1C) / sp[14]) * sp[10]) / sp[11]) * TrackObjectKeyframes[arg1].f04 / 100;
            TrackObjectStates[arg0].f1E += ((((TrackObjectKeyframes[arg1].f1E - TrackObjectKeyframes[arg1 - 1].f1E) / sp[14]) * sp[10]) / sp[11]) * TrackObjectKeyframes[arg1].f04 / 100;
            TrackObjectStates[arg0].f20 += ((((TrackObjectKeyframes[arg1].f20 - TrackObjectKeyframes[arg1 - 1].f20) / sp[14]) * sp[10]) / sp[11]) * TrackObjectKeyframes[arg1].f04 / 100;
        }
    }
}

void UpdateTrackObjects(void) {
    int i;
    int s0;
    int *sp = (int *)(SCRATCHPAD + 0x7C);
    int v1, a0, a2;
    int t0, t1;
    int r1, r2, r3, r5, r6;
    register int r4 asm("$2"); /* ASM_BARRIER: force modulo result into v0 before copying to a3 */
    int *hw;

    UpdateTrackObjectActivation();
    for (i = 0; i < TrackObjectCount; i++) {
            if (TrackObjectStates[i].f24 == 1) {
                if (TrackObjectDefs[i].f8 == 0xA) {
                    if (TrackObjectStates[i].f8 == 0) {
                        if (GameModeType != 1) {
                            TrackObjectStates[i].f8 = 0xB;
                        } else if (CourseIndex == 7) {
                            if (D_800AC1A2 != 0) {
                                TrackObjectStates[i].f8 = D_800AC1A2 + 0xA;
                            } else {
                                TrackObjectStates[i].f8 = 0xB;
                            }
                        } else if (D_800AC1A2 <= 0 || (CourseIndex == 6 && D_800AC1A2 < 2)) {
                            TrackObjectStates[i].f8 = 0xB;
                        } else {
                            switch (D_800AC0E6 + 1) {
                            case 1:
                                TrackObjectStates[i].f8 = 0xC;
                                break;
                            case 2:
                            case 3:
                                TrackObjectStates[i].f8 = 0xB;
                                break;
                            default:
                                TrackObjectStates[i].f8 = 0xD;
                                break;
                            }
                        }
                    }
                }
                if (TrackObjectDefs[i].f4 == 5) {
                    if (TrackObjectStates[i].f8 == 0 || TrackObjectStates[i].f0 >= 0xD1) {
                        if (TrackObjectDefs[i].f0 > 0 && (PsyqRand() % (TrackObjectDefs[i].f0 * 8 + 0x14)) != 0) {
                            TrackObjectStates[i].f8 = 0;
                            goto next_iter;
                        }
                        r1 = PsyqRand();
                        TrackObjectStates[i].fC = TrackObjectKeyframes[TrackObjectDefs[i].f24].f0C + (r1 % TrackObjectKeyframes[TrackObjectDefs[i].f24 + 1].f0C);
                        r2 = PsyqRand();
                        TrackObjectStates[i].f10 = TrackObjectKeyframes[TrackObjectDefs[i].f24].f10 + (r2 % TrackObjectKeyframes[TrackObjectDefs[i].f24 + 1].f10);
                        r3 = PsyqRand();
                        TrackObjectStates[i].f14 = TrackObjectKeyframes[TrackObjectDefs[i].f24].f14 + (r3 % TrackObjectKeyframes[TrackObjectDefs[i].f24 + 1].f14);
                        r4 = PsyqRand() % 6;
                        TrackObjectStates[i].f8 = r4 + 1;
                        SetAnimatedTrackObjectState(TrackObjectDefs[i].f0, 7, &TrackObjectStates[i].fC, r4);
                        TrackObjectStates[i].f0 = 0;
                    } else if (TrackObjectStates[i].f0 == 0x3C) {
                        hw = (int *)SCRATCHPAD;
                        sp[0] = (TrackObjectStates[i].fC - hw[2]) >> 10;
                        sp[1] = (TrackObjectStates[i].f14 - hw[4]) >> 10;
                        sp[2] = CordicSqrt((sp[0] * sp[0] + sp[1] * sp[1]) >> 8) >> 4;
                        TrackObjectStates[i].f4 = sp[2] - 0x46;
                    }
                    TrackObjectStates[i].f0++;
                } else {
                    s0 = TrackObjectDefs[i].f24 + TrackObjectStates[i].f28;
                    if (TrackObjectStates[i].f0 == TrackObjectKeyframes[s0].f00 * 6) {
                        if (s0 < TrackObjectDefs[i].f28 + TrackObjectDefs[i].f24) {
                            TrackObjectStates[i].f28++;
                            s0++;
                            if (TrackObjectStates[i].f0 == TrackObjectDefs[i].f10 * 6) {
                                TrackObjectStates[i].f0 = 0;
                                TrackObjectStates[i].f28 = 1;
                                s0 = TrackObjectDefs[i].f24 + 1;
                            }
                            v1 = TrackObjectKeyframes[s0].f08 / 10;
                            if (v1 == 1) {
                                TrackObjectStates[i].f4 = (TrackObjectStates[i].f4 % 100) + 300;
                            } else if (v1 == 0) {
                                TrackObjectStates[i].f4 = TrackObjectStates[i].f4 % 100;
                            } else if (v1 == 4) {
                                TrackObjectStates[i].f4 = (TrackObjectStates[i].f4 % 100) + 100;
                            } else if (v1 == 5) {
                                TrackObjectStates[i].f4 = (TrackObjectStates[i].f4 % 100) + 200;
                            }
                        }
                    }
                    if (TrackObjectDefs[i].f10 == 1) {
                        TrackObjectStates[i].fC = TrackObjectKeyframes[s0].f0C;
                        TrackObjectStates[i].f10 = TrackObjectKeyframes[s0].f10;
                        TrackObjectStates[i].f14 = TrackObjectKeyframes[s0].f14;
                        TrackObjectStates[i].f1C = TrackObjectKeyframes[s0].f1C;
                        TrackObjectStates[i].f1E = TrackObjectKeyframes[s0].f1E;
                        TrackObjectStates[i].f20 = TrackObjectKeyframes[s0].f20;
                    } else {
                        v1 = TrackObjectStates[i].f4 / 100;
                        if (v1 == 1) {
                            TrackObjectStates[i].f4 = (TrackObjectStates[i].f4 == 0x86) ? 0x73 : TrackObjectStates[i].f4 + 1;
                        } else if (v1 == 2) {
                            if ((r5 = TrackObjectStates[i].f4 - 0xC8) != 0) {
                                TrackObjectStates[i].f4 = (TrackObjectStates[i].f4 == 0xF4) ? 0xC8 : TrackObjectStates[i].f4 + 1;
                            }
                        }
                        if ((unsigned short)GameModeType - 2 < 3U) {
                            a2 = TrackObjectStates[i].f0;
                            TrackObjectStates[i].f0 = a2 + 1;
                            if (D_8010E7C8[i] == ModeFrame) {
                                TrackObjectStates[i].f0 = a2;
                            }
                            D_8010E7C8[i] = ModeFrame;
                        } else {
                            TrackObjectStates[i].f0++;
                        }
                        v1 = TrackObjectKeyframes[s0 - 1].f00;
                        t0 = (TrackObjectKeyframes[s0].f00 - v1) * 6;
                        t1 = TrackObjectStates[i].f0 - v1 * 6;
                        if (t0 == 0) {
                            TrackObjectStates[i].f24 = 0;
                        } else {
                            switch (TrackObjectKeyframes[s0].f08 % 10) {
                            case 1:
                            case 4:
                                InterpolateObjectKeyframe(i, s0, 1, 1, t1, t0, t1, t0);
                                break;
                            case 2:
                            case 5:
                                InterpolateObjectKeyframe(i, s0, -1, 0, t0 - t1, t0, t0 - t1, t0);
                                break;
                            case 3:
                                t0 /= 2;
                                if (t1 < t0) {
                                    InterpolateObjectKeyframe(i, s0, 2, 1, t1, t0, t1, t0);
                                } else {
                                    InterpolateObjectKeyframe(i, s0, -2, 0, t0 * 2 - t1, t0, t0 * 2 - t1, t0);
                                }
                                break;
                            default:
                                InterpolateObjectKeyframe(i, s0, 1, 1, t1, t0, 1, 1);
                                break;
                            }
                            if (TrackObjectKeyframes[s0].f08 / 10 == 1) {
                                switch (TrackObjectDefs[i].fC % 10) {
                                case 2:
                                case 5:
                                    sp[0] = TrackObjectDefs[i].fC / 10;
                                    sp[1] = (sp[0] << 4) + (sp[0] << 2);
                                    if (TrackObjectStates[i].f0 == 1 || TrackObjectStates[i].f4 == 300) {
                                        for (a0 = 0; a0 < 20; a0++) {
                                            D_8010E8E8[sp[1] + a0].vx = TrackObjectStates[i].fC >> 3;
                                            D_8010E8E8[sp[1] + a0].vy = TrackObjectStates[i].f10 >> 3;
                                            D_8010E8E8[sp[1] + a0].vz = TrackObjectStates[i].f14 >> 3;
                                        }
                                    } else if (TrackObjectStates[i].f0 % 2 == 1) {
                                        for (a0 = 19; a0 > 0; a0--) {
                                            D_8010E8E8[sp[1] + a0] = D_8010E8E8[sp[1] + a0 - 1];
                                        }
                                        D_8010E8E8[sp[1]].vx = TrackObjectStates[i].fC >> 3;
                                        D_8010E8E8[sp[1]].vy = TrackObjectStates[i].f10 >> 3;
                                        D_8010E8E8[sp[1]].vz = TrackObjectStates[i].f14 >> 3;
                                    }
                                    break;
                                case 3:
                                    if (TrackObjectStates[i].f0 == 1) {
                                        for (a0 = 0; a0 < 40; a0++) {
                                            D_8010E8E8[a0].vx = TrackObjectStates[i].fC >> 3;
                                            D_8010E8E8[a0].vy = TrackObjectStates[i].f10 >> 3;
                                            D_8010E8E8[a0].vz = TrackObjectStates[i].f14 >> 3;
                                        }
                                    } else if (TrackObjectStates[i].f0 % 12 == 1) {
                                        for (a0 = 39; a0 > 0; a0--) {
                                            D_8010E8E8[a0] = D_8010E8E8[a0 - 1];
                                        }
                                        D_8010E8E8[0].vx = TrackObjectStates[i].fC >> 3;
                                        D_8010E8E8[0].vy = TrackObjectStates[i].f10 >> 3;
                                        D_8010E8E8[0].vz = TrackObjectStates[i].f14 >> 3;
                                    }
                                    break;
                                }
                            }
                            if (TrackObjectDefs[i].f8 == 0xD) {
                                if (RaceState == 1) {
                                    switch (TrackObjectStates[i].f8) {
                                    case 0:
                                    case 4:
                                        if (PlayerControllers[0].edges08.fields.pressed & 0x8000) {
                                            TrackObjectStates[i].f8++;
                                        } else if (PlayerControllers[0].edges08.word & 0x7000) {
                                            TrackObjectStates[i].f8 = 5;
                                        }
                                        break;
                                    case 1:
                                    case 2:
                                    case 3:
                                        if (PlayerControllers[0].edges08.fields.pressed & 0x2000) {
                                            TrackObjectStates[i].f8++;
                                        } else if (PlayerControllers[0].edges08.word & 0xD000) {
                                            TrackObjectStates[i].f8 = 5;
                                        }
                                        break;
                                    }
                                    D_8010E7A8 = 0x37;
                                    D_8010E7AC = 0x36;
                                    D_8010E7B0 = 0x35;
                                    D_8010E7B4 = 0x20;
                                    if (ModeFrame == 0xAF) {
                                        if (TrackObjectStates[i].f8 != 4) {
                                            TrackObjectStates[i].f24 = -1;
                                        }
                                    }
                                } else {
                                    D_800A4090 += D_8010E7B4;
                                    if (D_800A4090 >= 0x80) {
                                        D_800A4090 -= 0x80;
                                        a0 = D_8010E7B0 - 1;
                                        if (D_8010E7B0 <= 0) {
                                            a0 += TrackNodeCount;
                                        }
                                        D_8010E7B0 = a0;
                                        v1 = D_8010E7AC - 1;
                                        if (D_8010E7AC <= 0) {
                                            v1 += TrackNodeCount;
                                        }
                                        D_8010E7AC = v1;
                                        v1 = D_8010E7A8 - 1;
                                        if (D_8010E7A8 <= 0) {
                                            v1 += TrackNodeCount;
                                        }
                                        D_8010E7A8 = v1;
                                    }
                                    sp[2] = (((TrackNodes[D_8010E7AC].x08 - TrackNodes[D_8010E7A8].x08) * D_800A4090) >> 5) + TrackNodes[D_8010E7A8].x08 * 4;
                                    sp[3] = (((TrackNodes[D_8010E7AC].y10 - TrackNodes[D_8010E7A8].y10) * D_800A4090) >> 7) + TrackNodes[D_8010E7A8].y10;
                                    sp[4] = (((TrackNodes[D_8010E7AC].z0C - TrackNodes[D_8010E7A8].z0C) * D_800A4090) >> 5) + TrackNodes[D_8010E7A8].z0C * 4;
                                    sp[5] = ((((TrackNodes[D_8010E7B0].x08 - TrackNodes[D_8010E7AC].x08) * D_800A4090) >> 5) + TrackNodes[D_8010E7AC].x08 * 4) - sp[2];
                                    sp[6] = ((((TrackNodes[D_8010E7B0].y10 - TrackNodes[D_8010E7AC].y10) * D_800A4090) >> 7) + TrackNodes[D_8010E7AC].y10) - sp[3];
                                    sp[7] = ((((TrackNodes[D_8010E7B0].z0C - TrackNodes[D_8010E7AC].z0C) * D_800A4090) >> 5) + TrackNodes[D_8010E7AC].z0C * 4) - sp[4];
                                    TrackObjectStates[i].fC += sp[2];
                                    TrackObjectStates[i].f10 += sp[3] - 0xFA;
                                    TrackObjectStates[i].f14 += sp[4];
                                    sp[8] = 0x400 - Atan2Table(sp[5], sp[7]);
                                    if (sp[8] == D_8010E7B8) {
                                        D_8010E7B4 += (D_8010E7B4 < 0x2C);
                                    } else if ((sp[8] >> 4) == (D_8010E7B8 >> 4)) {
                                        if (D_8010E7B4 < 0x26) {
                                            D_8010E7B4++;
                                        } else if (D_8010E7B4 >= 0x27) {
                                            D_8010E7B4--;
                                        }
                                    } else {
                                        D_8010E7B4 -= (D_8010E7B4 >= 0x22);
                                    }
                                    r6 = sp[8];
                                    TrackObjectStates[i].f1E = r6;
                                    D_8010E7B8 = r6;
                                    TrackObjectStates[i].f1C = 0x400 - Atan2Table(sp[6], CordicSqrt(sp[5] * sp[5] + sp[7] * sp[7]) >> 6);
                                    TrackObjectStates[i].f20 = 0;
                                }
                            }
                            if (TrackObjectDefs[i].f14 > 0) {
                                if (TrackObjectStates[i].f0 == TrackObjectDefs[i].f14 * 6) {
                                    TrackObjectStates[i].f24 = -1;
                                }
                            }
                        }
                    }
                }
            }
next_iter:
        ;
    }
}
