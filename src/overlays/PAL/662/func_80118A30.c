#include "rr/overlay662_name_controller.h"

void ovl662_func_80118A30(void) {
    s16 fadeStep;
    s32 cancel;
    s32 phase;
    u8 normalizedRow;

    phase = (u16)D_800F4D8A;
    D_800F2DE4=(u32)D_800F2DE4+1;
    cancel = 0;
    ScreenFrame=(u32)ScreenFrame+1;
    if (phase == 0) {
        func_8002AF8C(1, 1, 1);
        if (D_800F3A38[0].b != 0) {cancel=1;goto normalizeColumn;}
        if (D_800F3A38[0].buttons0A & 0x1000) {
            func_8004F24C(0);ovl662_D_801273A3--;goto normalizeRow;
        }else if (D_800F3A38[0].buttons0A & 0x4000) {
            func_8004F24C(0);ovl662_D_801273A3++;goto normalizeRow;
        }else if (D_800F3A38[0].buttons0A & 0x8000) {
            func_8004F24C(0);ovl662_D_801273A2--;goto normalizeColumn;
        }else if (D_800F3A38[0].buttons0A & 0x2000) {
            func_8004F24C(0);ovl662_D_801273A2++;goto normalizeColumn;
        }else goto normalizeColumn;
normalizeRow:
        normalizedRow=((s8)ovl662_D_801273A3+5)%5;
        ovl662_D_801273A3=normalizedRow;
        if((s8)ovl662_D_801273A2>=3){if((s8)normalizedRow==4)ovl662_D_801273A2=2;}
normalizeColumn:
        if((s8)ovl662_D_801273A3>=4)ovl662_D_801273A2=((s8)ovl662_D_801273A2+3)%3;
        else ovl662_D_801273A2=((s8)ovl662_D_801273A2+10)%10;
        ovl662_D_801273B4 = (s8) ovl662_D_801273A2 + ((s8) ovl662_D_801273A3 * 0xA);
        if (D_800F3A60 != 0) {
            func_8004F24C(1);
            if(ovl662_D_801273B4<41){
                if (ovl662_D_801273A0 < 8) {
                    func_8004F24C(1);
                    if (ovl662_D_801273B4 == 0x28) {
                        ovl662_D_801273A8[ovl662_D_801273A0] = D_8009F7E4[0];
                    } else {
                        ovl662_D_801273A8[ovl662_D_801273A0] = D_8009F7E4[ovl662_D_801273B4+1];
                    }
                    if (ovl662_D_801273A0 < 7) {
                        ovl662_D_801273A0 = (u8) ovl662_D_801273A0 + 1;
                    }
                }
}else if(ovl662_D_801273B4==41){
                if (ovl662_D_801273A0 > 0) {
                    ovl662_D_801273A0 = (u8) ovl662_D_801273A0 - 1;
                }
}else if(ovl662_D_801273B4==42){
                D_800AC5D0 = 0x40;
                D_800F4D8A = 1;
                func_8002B75C(D_800F3920.prefix.progress.prefix00.fields.name,(char*)ovl662_D_801273A8);
            }
        } else if (cancel == 1) {
            func_8004F24C(2);
            ovl662_D_801273A8[ovl662_D_801273A0] = D_8009F7E4[0];
            if (ovl662_D_801273A0 > 0) {
                ovl662_D_801273A0 = (u8) ovl662_D_801273A0 - 1;
            }
        }
    } else if (phase == 1) {
        func_8002AF8C(0, 1, 1);
        fadeStep = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16) fadeStep;
        func_800207D4(&D_800ACC14->head70, (0x40 - fadeStep) * 4, 0x45);
        if ((s16) D_800AC5D0 <= 0) {
            D_800FFC18 = 5;
            D_800F4C6A = phase;
            D_800F3A26 = 0x47;
            D_800F4D8A = 0;
        }
    }
    func_8003D6F0(0);
    func_8003D6F0(1);
    func_8002A344(0x1008B);
}
