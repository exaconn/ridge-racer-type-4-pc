/* RODATA 0x80011198 0x18 */
#include "rr/audio_cd_controller.h"
void func_800534D8(void) {
    s32 ready;
    RrAudioState *phaseState = &D_800F4DB8;
    u8 filter[2];
    RrCdLocation location;
    RrAudioState *var_v0;
    s32 temp_v1_2;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v0;
    register s32 temp_v0_2 __asm__("a2");
    s32 temp_v0_3;
    s32 rate;
    s32 temp_v0_4;
    register s32 temp_v0_5 __asm__("a2");
    s32 temp_v0_6;
    s32 temp_v1;
    s32 temp_v1_3;

    if (D_800F4DB8.sector2664 < 0) {
        switch (D_800F4DB8.action2670) {
        case 1:
            func_80052F2C();
            break;
        case 2:
            func_80053120();
            break;
        case 3:
            func_80053254();
            break;
        case 4:
            func_80053384();
            break;
        case 5:
            func_80053300();
            break;
        case 6:
            func_80052DA8();
            break;
        }
        goto block_34;
    }
    switch (D_800F4DB8.phase2674) {
    case 0:
                if (func_8008C0D8(1, 0) != 0) {
                    D_800F4DB8.state269C = 0;
                    D_800F4DB8.timer26A8 = 0;
                    D_800F4DB8.phase2674 = (s32) ((u32) D_800F4DB8.phase2674 + 1U);
                    goto seek;
                }
                goto timeout;
    case 1:
seek:
        CdIntToPos(D_800F4DB8.sector2664, location.c);
        CdControlF(0x15, location.c);
        D_800F4DB8.phase2674 = (s32) ((u32) D_800F4DB8.phase2674 + 1U);
        goto block_34;
    case 2:
        ready = 2;
            temp_v0_2 = func_8008C0D8(1, 0);
            if (temp_v0_2 == ready) {
                filter[0] = 1;
                filter[1] = (u8) D_800F4DB8.filter266C;
                if (CdControlF(0xD, filter) != 0) {
                    D_800F4DB8.phase2674 = (s32) ((u32) D_800F4DB8.phase2674 + 1U);
                }
            } else if (temp_v0_2 == 5) {
                D_800F4DB8.phase2674 = 1;
                if (func_8008F274(1) == 0x10) {
                    ready = -1;
                    D_800F4DB8.flag267C = 1;
                    goto block_27;
                }
            } else {
timeout:
                temp_v0_3 = (u32) phaseState->timer26A8 + 1U;
                phaseState->timer26A8 = temp_v0_3;
                if (temp_v0_3 >= 0x169) {
                    ready = -1;
block_27:
                    phaseState->flag2680 = ready;
                    func_8008C03C();
                    phaseState->sector2664 = ready;
                    phaseState->phase2674 = 0;
                }
            }
            goto block_34;
    case 3:
        ready = 2;
                temp_v0 = func_8008C0D8(1, 0);
                if (temp_v0 == ready) {
                    temp_v1 = D_800F4DB8.sector2664;
                    D_800F4DB8.sector2664 = -1;
                    D_800F4DB8.phase2674 = 0;
                    D_800F4DB8.state269C = -1;
                    D_800F4DB8.flag267C = 0;
                    D_800F4DB8.flag2680 = 0;
                    D_800F4DB8.sector2668 = temp_v1;
                    D_800F4DB8.position2688 = temp_v1;
                    D_800F4DB8.index2698 = (s32) D_800F4DB8.filter266C;
                    if (D_800F4DB8.state26AC == temp_v0) {
                        D_800F4DB8.state26AC = 0;
                    }
                } else if (temp_v0 == 5) {
                    D_800F4DB8.phase2674 = 1;
                }
                goto block_34;
    }
block_34:
    var_v0 = &D_800F4DB8;
    temp_a0 = var_v0->request26B4;
    if (temp_a0 != 0 && var_v0->action2670 == -1) {
        switch (temp_a0) {
        case 1:func_80052C70();D_800F746C=0;break;
        case 2:func_80052AB8();D_800F746C=0;break;
        default:D_800F746C=0;break;
        }
    }
    if (D_800F4DB8.state269C == 1) {
        temp_v0_4 = D_800F4DB8.toc28C8[D_800F4DB8.bankA8CC][D_800F4DB8.index2698].position;
        D_800F4DB8.previous26DC = (s32) D_800F4DB8.tick26D8;
        D_800F4DB8.base26F0 = temp_v0_4;
        rate = D_800F4DB8.rate26F4;
        __asm__("" : : "r"(rate));
        if (rate != 0) {
            D_800F4DB8.tick26D8 = (s32) ((((u32) temp_v0_4 + 10U) * (u32) rate) / 600000U);
        } else {
            D_800F4DB8.tick26D8 = 0;
        }
        D_800F4DB8.tick26D0 = 0;
        D_800F4DB8.half26D2 = 0;
        temp_v1_2 = (u32) D_800F4DB8.tick26D8 - (u32) D_800F4DB8.previous26DC;
        D_800F4DB8.quarter26D4 = 0;
        if (temp_v1_2 == 1) {
            temp_a1 = (u32) D_800F4DB8.tick26D8 + (u32) D_800F4DB8.offset26E8;
            D_800F4DB8.tick26D0 = temp_v1_2;
            if (!(temp_a1 & 1)) {
                D_800F4DB8.half26D2 = temp_v1_2;
            }
            if (!(temp_a1 & 3)) {
                D_800F4DB8.quarter26D4 = temp_v1_2;
            }
        }
        if ((u32) D_800F4DB8.position2688 > (u32) D_800F4DB8.end2690) {
            if (D_800F4DB8.flag26A0 == 0) {
                D_800F4DB8.action2670 = 4;
                D_800F4DB8.step2678 = 0;
            } else if (D_800F4DB8.action2670 != 5) {
                D_800F4DB8.action2670 = 5;
                D_800F4DB8.timer26A4 = 0;
                func_8001E84C(D_8001118C, &D_800F4DB8);
            }
            func_80052B48(0);
        }
            if (D_800F4DB8.previous268C != D_800F4DB8.position2688) {
            D_800F4DB8.timer26A8 = 0;
        } else {
            D_800F4DB8.timer26A8 = (s32) ((u32) D_800F4DB8.timer26A8 + 1U);
        }
        if (D_800F4DB8.timer26A8 >= 0x1E) {
            D_800F4DB8.action2670 = 4;
            D_800F4DB8.step2678 = 0;
            D_800F4DB8.timer26A8 = 0;
        }
    }
    func_80052E6C();
    D_800F4DB8.previous268C = (s32) D_800F4DB8.position2688;
    if ((u32) ((u32) D_800F4DB8.state269C - 1U) < 2U) {
        temp_v0_5 = func_8008C0D8(1, D_800F4DB8.status265C);
        switch (temp_v0_5) {
        case 0:
        case 1:
            break;
        case 2:
            if (func_8008BFB4() == 0x11) {
                temp_v0_6 = CdPosToInt(&D_800F4DB8.status265C[5]);
                if (temp_v0_6 > 0) {
                    D_800F4DB8.position2688 = temp_v0_6;
                }
            }
            CdControlF(0x11, 0);
            return;
        case 5:
            if (D_800F4DB8.state269C == 2) {
                func_80052ADC(D_800F4DB8.volume26CE);
            }
            temp_v1_3 = D_800F4DB8.state269C;
            D_800F4DB8.state269C = -1;
            D_800F4DB8.action2670 = -1;
            D_800F4DB8.step2678 = 0;
            D_800F4DB8.state26AC = temp_v1_3;
            break;
        }
    }
}
