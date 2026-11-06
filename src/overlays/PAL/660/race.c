#include "rr/overlay660.h"

void ovl660_func_80117084(RrRaceVehicleViews *player){
 RrRaceVehicleControl *control=&player->parts.control;
 if((u32)D_800FF6B0-2U<2){
  s32 allow=0;
  if(control->steering94.count22<=0)allow=player->vehicle.half1EA!=D_800AD522;
  else if(player->vehicle.first<=(s32)(0U-(u32)D_800F2EB8))allow=1;
  if(allow==1){D_800FF6B0=5;D_800AC5D0=75;func_8004FC18(0,71);}
 }
}

void ovl660_func_8011712C(void)
{
    func_80036068();
    func_8003CAB4();
    D_800F4C6A = 1;
    D_800F3A26 = 53;
    D_800F4D8A = 0;
    func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70, 255, 69);
}

void ovl660_func_8011718C(void) {
    s32 fadeFlags;
    s8 paused;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    if ((((u32)D_800FF6B0 - 1U) < 2U) && (D_800F3A38[0].buttons08 & 0x800)) {
        if (D_800AC5D0 <= 0) {
            D_800AC5D0 = 0xF;
            paused = D_800F4C68 == 0;
            D_800F4C68 = paused;
            if (paused != 0) {
                func_8004FA04();
                D_800F2FDC = 0;
            } else {
                func_800348E8(D_800F4D44);
                func_8004FA50();
            }
        } else {
            goto update_pause_timer;
        }
    } else if (D_800AC5D0 > 0) {
update_pause_timer:
        if (D_800FF6B0 < 4) {
            D_800AC5D0 = (u16) D_800AC5D0 - 1;
        }
    }
    if (D_800F4C68 != 0) {
        func_80034844(D_800F4D44);
        D_800F2DE4 = (s32)((u32)D_800F2DE4 - 1U);
        func_800349F4(D_800ACC14->ordering.ordering70, D_800F2FDC);
        func_8003BFC8(1);
        func_8002BA28(0);
        func_80033F3C(D_800ACC14->ordering.ordering70, D_800F2DE4);
        func_80021350(D_800F4C70.first, 0);
        func_800211A4(0);
        func_80021684((D_800ABEF8[0].input * 0xA0) / 1168, 0);
        func_800219BC(D_800ABEF8[0].half27A, 0);
        func_80021E2C(0);
        func_80022098(D_800ACC14->ordering.ordering70, 0);
        func_80020B10(D_800F4D44);
        func_8006E1AC();
        func_8006EF8C(0);
        func_8002E0D8(0);
        RR_660_FRAME_SCRATCH->word78 = D_800F2A64;
        func_8006ECE8(0);
        func_800747EC(0);
        func_80036FD4(0);
        func_80070734(ScreenFrame);
        return;
    }
    ScreenFrame = (s32)((u32)ScreenFrame + 1U);
    if ((u32)D_800F2DE4 >= 75U && D_800FF6B0 == 0) {
        func_8004FED4();
        D_800FF6B0=1;
        func_800172DC();
        func_80014A8C(-1);
    } else if (D_800FF6B0 == 1) {
            if ((u32) D_800F2DE4 >= 0xB0U) {
                func_80022CD4(D_800ABEF8);
                D_800FF6B0 = 2;
                func_80052A30(1);
            }
    } else if (D_800FF6B0 >= 4) {
            if (D_800AC5D0 < 0x19) {
                fadeFlags = 0x45;
                if (D_800FF6B0 == 4) {
                    fadeFlags = 0x25;
                }
                func_800207D4(D_800ACC14->ordering.ordering70, (0x19 - D_800AC5D0) * 0xA, fadeFlags);
            }
            if (D_800AC5D0 <= 0) {
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
    } else if (D_800FF6B0 == 0) {
            if ((u32) D_800F2DE4 >= 0x33U) {
                func_800207D4(D_800ACC14->ordering.ordering70, (D_800F2DE4 - 0x32) * 0xA, 0x25);
                D_800ADAF0[1].environment.rgb24 = 0;
                D_800ADAF0[0].environment.rgb24 = 0;
            } else if ((u32) D_800F2DE4 < 0x19U) {
                func_800207D4(D_800ACC14->ordering.ordering70, (0x19 - D_800F2DE4) * 0xA, 0x45);
                D_800ADAF0[1].environment.rgb24 = 1;
                D_800ADAF0[0].environment.rgb24 = 1;
            }
            RR_660_FRAME_SCRATCH->pointers.cursor = func_8001FC28(D_800ACC14->ordering.ordering70, RR_660_FRAME_SCRATCH->pointers.cursor, 0, 0, 0x140, 0x100);
    }
    func_8003C254(D_800ABEF8);
    func_8003BFC8(0);
    ovl660_func_80117084((RrRaceVehicleViews *)(void *)D_800ABEF8);
    func_8003C96C();
    if (D_800FF6B0 < 4) {
        func_80033F3C(D_800ACC14->ordering.ordering70, D_800F2DE4);
        func_80034630(D_800F2DE4);
    }
    func_800396E4((char *)(void *)D_800ABEF8);
    if (((u32)D_800FF6B0 - 1U) < 3U) {
        func_8002BA28(0);
        func_80021350(D_800F4C70.first, 0);
        func_800211A4(0);
        func_80021684((D_800ABEF8[0].input * 0xA0) / 1168, 0);
        func_800219BC(D_800ABEF8[0].half27A, 0);
        func_80021E2C(0);
        func_80022098(D_800ACC14->ordering.ordering70, 0);
        func_80020B10(D_800F4D44);
    }
    func_80037D88(D_800F4D44);
    func_800735D8(0);
    if (D_800FF6B0 == 0) {
        if ((u32) D_800F2DE4 < 0x4BU) {
            func_80033474(D_800ABEF8, D_800F2DE4);
        } else {
            goto draw_vehicle;
        }
    } else {
draw_vehicle:
        func_80033BE4((struct w3g2p *)(void *)D_800ABEF8);
    }
    func_8006E1AC();
    func_8002E0D8(0);
    if (D_800ABEF8[0].half1EA != D_800AD522) {
        if (D_800FF6B0 < 4) {
            func_800225DC(0);
        }
    }
    RR_660_FRAME_SCRATCH->word78 = (s32) D_800F2A64;
    func_8006ECE8(0);
    func_800747EC(0);
    func_80036FD4(0);
    func_8005007C(D_800F4C70.first, D_800F2AB4.first, 0);
    if (D_800FF6B0 >= 4) {
        func_80036030();
        if (D_800FF6B0 == 6) {
            func_80036068();
        }
    }
}
