#include "rr/overlay661_race_controller.h"
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
    register s32 temp_s1 __asm__("$17");
    s32 temp_v0_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_s2;
    register s32 var_v1 __asm__("$3");
    s32 var_v1_2;
    register s32 var_v1_3 __asm__("$3");
    s32 var_v1_4;
    s8 temp_v1;
    RrRaceVehicle *temp_s0;
    RrRaceVehicle *temp_s0_3;
    Rr661FrameVector16 *temp_v0;
    Rr661FrameVector16 *temp_v0_2;
    Rr661FrameVector16 *temp_v0_4;
    Rr661FrameVector16 *temp_v0_5;
    Rr661FrameVector16 *temp_v1_2;
    Rr661FrameVector16 *temp_v1_3;
    Rr661FrameVector16 *temp_v1_4;
    Rr661FrameVector16 *temp_v1_5;

    D_800F2DE4=(u32)D_800F2DE4+1;
    if (((u32) ((u32)D_800FF6B0 - 1) < 2U) && (D_800F3A38[0].buttons08 & 0x800)) {
        if (D_800AC5D0 <= 0) {
            D_800AC5D0 = 0xF;
            temp_v1 = D_800F4C68 == 0;
            D_800F4C68 = temp_v1;
            if (temp_v1 != 0) {
                func_8004FA04();
                D_800F2FDC = 0;
            } else {
                func_800348E8(D_800F4D44);
                func_8004FA50();
            }
        } else {
            goto block_7;
        }
    } else if (D_800AC5D0 > 0) {
block_7:
        if (D_800FF6B0 < 4) {
            D_800AC5D0 = (u16) D_800AC5D0 - 1;
        }
    }
    if (D_800F4C68 != 0) {
        func_80034844(D_800F4D44);
        D_800F2DE4=(u32)D_800F2DE4-1;
        func_800349F4(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, D_800F2FDC);
        if (D_800FF6B0 < 4) {
            func_80033F3C(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, D_800F2DE4);
        }
        var_s2 = 0;
        if ((u32) ((u32)D_800FF6B0 - 1) < 3U) {
            do {
                
                temp_s0 = &D_800ABEF8[var_s2];
                func_80021684((temp_s0->input * 0xA0) / 1168, var_s2);
                func_800217D0(*(s16 *)((u8 *)&D_800F4C70+var_s2*2), var_s2);
                func_800219BC(temp_s0->half27A, var_s2);
                func_80021A7C(temp_s0->half1EE, var_s2);
                {register s32 playerArg __asm__("$5")=var_s2;s32 budget=D_800AC5A4;__asm__("" : : "r"(budget));{register s32 nextPlayer __asm__("$2")=playerArg+1;__asm__("" : : "r"(nextPlayer));var_s2=nextPlayer;func_8002BD18(budget,playerArg);}}
                func_80021C94(var_s2, temp_s0->steering288.count22);
                func_80021E2C(var_s2);
                func_80022098(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, var_s2);
                func_80021C00(var_s2);
            } while (var_s2 < 2);
            func_80022390(0);
            func_800211A4(2);
            func_80020B10(2);
        }
        var_s2 = 0;
        do {
            var_v1 = var_s2 * 0x10;
            __asm__("" :: "r"(var_v1));
            temp_v0 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1);
            temp_v1_2 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1);
__asm__("" : : "r"(temp_v0),"r"(temp_v1_2));
        RR_661_FRAME_SCRATCH->rotation18 = *temp_v0;
        RR_661_FRAME_SCRATCH->position08 = *temp_v1_2;
            __asm__("" : "+r"(var_s2));
        func_8006EF8C(var_s2 + 2);
            if (D_800FF6B0 == 0) {
                var_a0 = var_s2 * 2;
                if ((u32) D_800F2DE4 < 0x4BU) {
                    func_80033474(&D_800ABEF8[var_s2], D_800F2DE4);
                    var_v1_2 = var_s2 * 0x10;
                } else {
                    goto block_22;
                }
            } else {
                var_a0 = var_s2 * 2;
block_22:
                func_80033BE4((struct w3g2p *)((unsigned long)((((var_a0+var_s2)*8)+var_s2)<<5)+(unsigned long)D_800ABEF8));
                var_v1_2 = var_s2 * 0x10;
            }
            temp_v0_2 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1_2);
            temp_v1_3 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1_2);
__asm__("" : : "r"(temp_v0_2),"r"(temp_v1_3));
        *temp_v0_2 = RR_661_FRAME_SCRATCH->rotation18;
        *temp_v1_3 = RR_661_FRAME_SCRATCH->position08;
            temp_s0_2 = var_s2 + 1;
            func_8006E1AC();
            func_8002E0D8(var_s2 + 2);
            func_8006ECE8(var_s2);
            func_800747EC(0);
            func_80036FD4(temp_s0_2);
            var_s2 = temp_s0_2;
        } while (var_s2 < 2);
        return;
    }
    ScreenFrame=(u32)ScreenFrame+1;
    if ((u32) ((u32)D_800FF6B0 - 2) < 2U) {
        temp_v0_3 = (s32)((u32)D_800AC5A4 - 1);
        D_800AC5A4 = temp_v0_3;
        if (temp_v0_3 <= 0) {
            D_800FF6B0 = 5;
            D_800AC5D0 = 0x4B;
            func_8004FC18(0,0x21);
        }
    }
    if (((u32) D_800F2DE4 >= 0x4BU) && (D_800FF6B0 == 0)) {
        D_800FF6B0 = 1;
        func_8004FED4();
        func_80014A8C(-1);
        func_800172DC();
        D_800ADAF0[1].environment.rgb24 = 0;
        D_800ADAF0[0].environment.rgb24 = 0;
    } else {
        if (D_800FF6B0==1) {
            if ((u32) D_800F2DE4 >= 0xB0U) {
                func_80022CD4(&D_800ABEF8[0]);
                func_80022CD4(&D_800ABEF8[1]);
                func_80052A30(1);
                D_800FF6B0 = 2;
            }
        } else if (D_800FF6B0>=4) {
            var_a2 = 0x25;
            if (D_800F4C64 == -1) {
                var_a2 = 0x45;
            }
            if (D_800AC5D0 < 0x19) {
                func_800207D4(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, (0x19 - D_800AC5D0) * 0xA, var_a2);
                RR_661_FRAME_SCRATCH->pointers.cursor = func_8001FC28(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, RR_661_FRAME_SCRATCH->pointers.cursor, 0, 0, 0x140, 0x100);
            }
            if (D_800AC5D0 <= 0) {
                ovl661_D_80118448[0] = (u16) D_800ABEF8[0].half1EE;
                ovl661_D_80118448[1] = (u16) D_800ABEF8[1].half1EE;
                if (D_800FF6B0 == 4) {
                    D_800F4C6A = 1;
                    D_800F3A26 = 0x15;
                    D_800F4D8A = 0;
                } else {
                    func_8003660C();
                }
            } else {
                D_800AC5D0 = (u16) D_800AC5D0 - 1;
            }
        } else if (D_800FF6B0==0) {
            if ((u32)D_800F2DE4 >= 51U) {
                func_800207D4(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70,((u32)D_800F2DE4-50U)*10U,37);
            } else if ((u32)D_800F2DE4 < 25U) {
                func_800207D4(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70,(25U-(u32)D_800F2DE4)*10U,69);
            }
            RR_661_FRAME_SCRATCH->pointers.cursor = func_8001FC28(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, RR_661_FRAME_SCRATCH->pointers.cursor, 0, 0, 0x140, 0x100);
        }
    }
    ovl661_func_80114924((RrRaceCheckpointVehicle *)(void *)&D_800ABEF8[0]);
    ovl661_func_80114924((RrRaceCheckpointVehicle *)(void *)&D_800ABEF8[1]);
    func_8003C96C();
    if ((s16) D_800ABEF8[0].half1EE < (s16) D_800ABEF8[1].half1EE) {
        D_800ABEF8[0].half1EE = 0U;
        D_800ABEF8[1].half1EE = 1U;
    } else {
        D_800ABEF8[0].half1EE = 1U;
        D_800ABEF8[1].half1EE = 0U;
    }
    func_80037D88(D_800F4D44);
    if (D_800FF6B0 < 4) {
        func_80033F3C(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, D_800F2DE4);
        func_80034630(D_800F2DE4);
    }
    var_s2 = 0;
    if ((u32) ((u32)D_800FF6B0 - 1) < 3U) {
        do {
            
                temp_s0_3 = &D_800ABEF8[var_s2];
            func_80021684((temp_s0_3->input * 0xA0) / 1168, var_s2);
            func_800217D0(*(s16 *)((u8 *)&D_800F4C70+var_s2*2), var_s2);
            func_800219BC(temp_s0_3->half27A, var_s2);
            func_80021A7C(temp_s0_3->half1EE, var_s2);
            {register s32 playerArg __asm__("$5")=var_s2;s32 budget=D_800AC5A4;__asm__("" : : "r"(budget));{register s32 nextPlayer __asm__("$2")=playerArg+1;__asm__("" : : "r"(nextPlayer));var_s2=nextPlayer;func_8002BD18(budget,playerArg);}}
            func_80021C94(var_s2, temp_s0_3->steering288.count22);
            func_80021E2C(var_s2);
            func_80022098(((RrFrameStorage *)(void *)D_800ACC14)->tables.secondB70, var_s2);
            func_80021C00(var_s2);
        } while (var_s2 < 2);
        func_80022390(0);
        func_800211A4(2);
        func_80020B10(2);
    }
    var_s2 = 0;
    do {
        var_v1_3 = var_s2 * 0x10;
        __asm__("" :: "r"(var_v1_3));
        temp_v0_4 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1_3);
        temp_v1_4 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1_3);
__asm__("" : : "r"(temp_v0_4),"r"(temp_v1_4));
        RR_661_FRAME_SCRATCH->rotation18 = *temp_v0_4;
        RR_661_FRAME_SCRATCH->position08 = *temp_v1_4;
        __asm__("" : "+r"(var_s2));
        func_8006EF8C(var_s2 + 2);
        if (D_800FF6B0 == 0) {
            var_a0_2 = var_s2 * 2;
            if ((u32) D_800F2DE4 < 0x4BU) {
                func_80033474(&D_800ABEF8[var_s2], D_800F2DE4);
                var_v1_4 = var_s2 * 0x10;
            } else {
                goto block_66;
            }
        } else {
            var_a0_2 = var_s2 * 2;
block_66:
            func_80033BE4((struct w3g2p *)((unsigned long)((((var_a0_2+var_s2)*8)+var_s2)<<5)+(unsigned long)D_800ABEF8));
            var_v1_4 = var_s2 * 0x10;
        }
        temp_v0_5 = (Rr661FrameVector16 *)((u8 *)D_801183D0+var_v1_4);
        temp_v1_5 = (Rr661FrameVector16 *)((u8 *)D_801183F0+var_v1_4);
__asm__("" : : "r"(temp_v0_5),"r"(temp_v1_5));
        *temp_v0_5 = RR_661_FRAME_SCRATCH->rotation18;
        *temp_v1_5 = RR_661_FRAME_SCRATCH->position08;
        temp_s1 = var_s2 * 2;
        func_8006E1AC();
        __asm__("" : : "r"(temp_s1));
        func_8002E0D8(var_s2 + 2);
        if (((RrRaceVehicle *)((unsigned long)((((temp_s1+var_s2)*8)+var_s2)<<5)+(unsigned long)D_800ABEF8))->half1EA != 0) {
            func_800225DC(var_s2 + 1);
        }
        RR_661_FRAME_SCRATCH->mode5E = 1;
        func_8006ECE8(var_s2);
        func_800735D8(0);
        func_800747EC(0);
        temp_s0_4 = var_s2 + 1;
        func_80036FD4(temp_s0_4);
        {register s32 playerArg2 __asm__("$6")=var_s2;
        temp_a0 = *(s16 *)((u8 *)&D_800F4C70+temp_s1);
        temp_a1_3 = *(s16 *)((u8 *)&D_800F2AB4+temp_s1);
        var_s2 = temp_s0_4;
        func_8005007C(temp_a0,temp_a1_3,playerArg2);}
        __asm__("" : : "r"(temp_s1));
    } while (var_s2 < 2);
    if (D_800FF6B0 >= 4) {
        func_80036030();
        if (D_800FF6B0 == 6) {
            func_80036068();
        }
    }
}
