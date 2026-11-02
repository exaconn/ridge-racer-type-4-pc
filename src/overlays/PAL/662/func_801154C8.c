#include "rr/overlay662_course_controller.h"
void ovl662_func_801154C8(void) {
    union {RrPreviewRgb3 rgb; u8 channel[3];} color;
    s32 var_a3;
    s32 temp_s1;
    s16 temp_v1_7;
    s16 temp_v1_8;
    s32 temp_a1;
    s16 closePhase;
    s32 temp_s0;
    s32 temp_s5;
    s32 var_a2_2;
    s32 var_s0;
    s8 var_s6;
    s32 temp_v0_2;
    s8 temp_v0_3;
    s32 var_a1;
    u8 temp_s3;
    RrRaceVehicle *temp_v0;
    RrRaceVehicle *vehicleBase;
    s32 playerIndex;
    s32 *tick;
    u8 *temp_v1;
    RrUiRow24 *temp_v1_2;
    RrUiRow24 *temp_v1_3;
    RrUiRow24 *temp_v1_4;
    RrUiRow24 *temp_v1_5;
    RrUiRow24 *temp_v1_6;

    var_s6 = 0;
    tick = &D_800F2DE4;
    playerIndex = D_800AC5D4;
    vehicleBase = D_800ABEF8;
    temp_v0 = &vehicleBase[playerIndex&1];
    temp_a1 = ((u32)temp_v0->rot54 + 8) & 0xFFF;
    *tick=(u32)*tick+1;
    temp_v0->rot54 = temp_a1;
    func_80033A74(1);
    func_8006EF8C(6);
    temp_v0_2 = func_8003AE4C(0, 1);
    if (temp_v0_2 != 1) {
        if ((*(s8*)(D_800F2AB0+1)) == 1) {
            (*(s8*)(D_800F2AB0+1)) = 0;
        }
        goto block_67;
    }
    func_8002E394(vehicleBase);
    if ((*(s8*)(D_800F2AB0+1)) == 7) {
        (*(s8*)(D_800F2AB0+1)) = temp_v0_2;
    }
    var_s6 = 1;
    switch ((u16)D_800F4D8A) {
    case 0:
        func_8002AF8C(1, 1, 1);
        temp_s0 = func_8003708C((s8*)&D_800F4D98);
        func_8002B330(1, 2, (char*)&ovl662_D_801272E4, ovl662_D_801272E0, 0);
        func_80037040((s8*)&D_800F4D98, D_800ABE08[1][ovl662_D_801272E4].glyph);
        if (((u32)temp_s0 << 0x10) != ((u32)func_8003708C((s8*)&D_800F4D98) << 0x10)) {
            func_8003A198();
            D_800ACC18 = D_800F4D70.first;
            func_80037040((s8*)&D_800F4D98, D_800ABE08[1][ovl662_D_801272E4].glyph);
            (*(s8*)(D_800F2AB0+1)) = 6;
            temp_v1 = (u8 *)(void *)&D_800ABE08 + ovl662_D_801272E4 * sizeof(RrCourseChoice4);
            D_800ACBD0[0].first = (s16) (s8) ((RrCourseChoice4 *)(void *)(temp_v1 + sizeof(D_800ABE08[0])))->first;
            D_800ACBD0[0].second = (s16) (s8) ((RrCourseChoice4 *)(void *)(temp_v1 + sizeof(D_800ABE08[0])))->second;
        }
        D_800F4D80[D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char*)&D_800AC64A, 4, 0);
        var_a1 = 2;
        temp_s5 = ((D_800F4D98.pad02[0] % 20) & 0xFF) == 0x13;
        if (temp_s5 != 0) {
            if (ovl662_D_801272E4 == 8) {
                D_800F4D80[0].half20 = 1;
                if (D_800AC64A == 0) {
                    var_a1 = 4;
                }
            } else {
                D_800F4D80[0].half20 = 0;
            }
            D_800F4D80[2].half20 = 1;
            if (D_800AC64A == 2) {
                var_a1 = 4;
            }
        } else {
            D_800F4D80[0].half20 = 0;
            D_800F4D80[2].half20 = 0;
        }
        D_800F4D80[D_800AC64A].cells[0] = var_a1;
        temp_s3 = D_800F4D80[0].byte14;
        if (temp_s3 == 2) {
            temp_s1 = D_800F3AFC->state10;
            if (temp_s1 == 3) {
                if (D_800F3A38[0].a != 0) {
                    func_8002AF8C(0, 1, 1);
                    D_800F4D80[D_800AC64A].cells[0] = 1;
                    if (D_800AC64A == 0) {
                        if (ovl662_D_801272E4 < 8) {
                            func_80014A8C(-1);
                            func_8004F24C(1);
                            var_s0 = 0;

                            D_800F4D8A = (u16) temp_s1;
                            D_800F3AFC->state10 = 4;
                            closePhase = 3;
                            __asm__("" : "+r"(closePhase));
                            (*(s8*)(D_800F2AB0+1)) = closePhase;
                            do {
                                temp_v1_2 = (RrUiRow24 *)((unsigned long)var_s0 * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                                if (temp_v1_2->byte14 == 2) {
                                    temp_v1_2->byte14 = 3U;
                                }
                                var_s0 += 1;

                            } while (var_s0 < 4);
                        } else {
                            goto block_40;
                        }
                    } else if (D_800AC64A == 1) {
                        func_80014A8C(-1);
                        func_8004F24C(1);
                        var_s0 = 0;

                        D_800F3AFC->state10 = 4;
                        (*(s8*)(D_800F2AB0+1)) = (s8) temp_s1;
                        do {
                            temp_v1_3 = (RrUiRow24 *)((unsigned long)var_s0 * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                            if (temp_v1_3->byte14 == 2) {
                                temp_v1_3->byte14 = 3U;
                            }
                            var_s0 += 1;

                        } while (var_s0 < 4);
                        D_800AC5D0 = 0x40;
                        D_800F4D8A = 5;
                    } else if (D_800AC64A == temp_s3) {
                        if (temp_s5 == 0) {
                            func_80014A8C(-1);
                            func_8004F24C(1);
                            var_s0 = 0;

                            D_800F4D8A = 4;
                            D_800F3AFC->state10 = 4;
                            (*(s8*)(D_800F2AB0+1)) = (s8) temp_s1;
                            do {
                                temp_v1_4 = (RrUiRow24 *)((unsigned long)var_s0 * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                                if (temp_v1_4->byte14 == 2) {
                                    temp_v1_4->byte14 = 3U;
                                }
                                var_s0 += 1;

                            } while (var_s0 < 4);
                        } else {
block_40:
                            func_8004F24C(4);
                        }
                    } else if (D_800AC64A == temp_s1) {
                        func_80014A8C(-1);
                        func_8004F24C(1);
                        var_s0 = 0;
                        if (D_800F4BEC > 0) {

                            do {
                                temp_v1_5 = (RrUiRow24 *)((unsigned long)var_s0 * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                                if (temp_v1_5->byte14 == 2) {
                                    temp_v1_5->byte14 = 3U;
                                }
                                var_s0 += 1;

                            } while (var_s0 < D_800F4BEC);
                        }
                        D_800F3AFC->state10 = 4;
                        (*(s8*)(D_800F2AB0+1)) = 3;
                        goto block_54;
                    }
                } else if (D_800F3A38[0].b != 0) {
                    func_8004F24C(2);
                    func_80014A8C(-1);
                    func_8002AF8C(0, 1, 1);
                    var_s0 = 0;
                    if (D_800F4BEC > 0) {

                        do {
                            temp_v1_6 = (RrUiRow24 *)((unsigned long)var_s0 * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                            if (temp_v1_6->byte14 == 2) {
                                temp_v1_6->byte14 = 3U;
                            }
                            D_800F3AFC->state10 = 4;
                            (*(s8*)(D_800F2AB0+1)) = 3;
                            var_s0 += 1;

                        } while (var_s0 < D_800F4BEC);
                    }
block_54:
                    D_800AC5D0 = 0x40;
                    D_800F4D8A = 8;
                }
            }
        }
        goto block_67;
    case 3:
        if (((*(s8*)(D_800F2AB0+1)) == 5) && (D_800F3AFC->state10 == 0xB)) {
            D_800F4C6A = 1;
            D_800F3A26 = 0x41;
            D_800F4D8A = 0;
            ovl662_D_801272BA = (u16) ovl662_D_801272E4;
        }
        goto block_67;
    case 4:
        if (((*(s8*)(D_800F2AB0+1)) == 5) && (D_800F3AFC->state10 == 0xB)) {
            ovl662_func_80118E6C();
        } else {
            goto block_67;
        }
        goto done;
    case 5:
        temp_v1_7 = D_800AC5D0 - 1;
        D_800AC5D0 = (u16) temp_v1_7;
        func_800207D4(((RrFrameStorage*)(void*)D_800ACC14)->ordering.ordering70, (0x40 - temp_v1_7) * 4, 0x45);
        if ((s16) D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 0x1B;
            D_800F4D8A = 0;
        }
        goto block_67;
    case 8:
        temp_v1_8 = D_800AC5D0 - 1;
        D_800AC5D0 = (u16) temp_v1_8;
        func_800207D4(((RrFrameStorage*)(void*)D_800ACC14)->ordering.ordering70, (0x40 - temp_v1_8) * 4, 0x45);
        if ((s16) D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 0x27;
            D_800F4D8A = 0;
        }
        goto block_67;
    default:
        break;
    }
block_67:
        if ((ovl662_D_801272D4 == 0) && ((*(s8*)(D_800F2AB0+1)) == 2)) {
            ovl662_D_801272D4 = 1;
            D_800F3AFC->half02 = 1;
            var_s0 = 0;
        }
        var_s0 = 0;
        do {
            func_8003EC88(var_s0);
            var_s0 += 1;
        } while (var_s0 < 4);
        var_s0 = var_s6;
        if (((u32) ((u8) (*(s8*)(D_800F2AB0+1)) - 1) < 2U) && (var_s0 == 1)) {
            func_80037198(0);
            func_8002E5A0((char *)(void *)D_800ABEF8);
            var_a2_2 = 0x100;
            if ((*(s8*)(D_800F2AB0+1)) == var_s0) {
                temp_v0_3 = ~((D_800F2AB0[2] * 0xFF) / 30);
                color.channel[0] = temp_v0_3;
                color.channel[1] = temp_v0_3;
                color.channel[2] = temp_v0_3;
                var_a3 = 0x102;
                var_a2_2 = (D_800F2AB0[2] << 8) / 30;
            } else {
                var_a3 = 1;
                color.channel[0] = 0x80;
                color.channel[1] = 0x80;
                color.channel[2] = 0x80;
            }
            func_8002CC88(D_800ABEF8, color.rgb, var_a2_2, var_a3);
        }
        func_8003D6F0(0);
        if (func_8003FBA8(0) > 0) {
            func_8002B12C(0xA0, 0x90, 8, 1, (s32) ovl662_D_801272E4, 0, (s32) ovl662_D_801272E0);
        }
done:
    func_8002A344(0x1008E);
}
