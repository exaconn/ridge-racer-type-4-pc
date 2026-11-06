#include "rr/overlay662_screen_init.h"
void ovl662_func_80118ED8(void) {
    D_800F2DE4 = 0;
    ScreenFrame = 0;
    ovl662_D_801272D4 = 0;
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    func_8003A198();
    D_800F4D70.second = D_800102F0 + 0x2D000;
    func_8003A1B4();
    D_800ACC18 = D_800F4D70.second;
    func_8003E928(0, 24, 88, 68, 32, 0, 0, 1, 0, 83);
    func_8003E928(1, 24, 128, 68, 32, 0, 0, 1, 0, 42);
    func_8003E928(2, 24, 168, 68, 32, 0, 0, 1, 0, 85);
    func_80037110();
    func_8003D228(1, 24, 232, 84, 34, 0, 0x501);
    func_80013890(5);
    D_800F4C6A = 1;
    D_800F3A26 = 68;
    D_800ACC18 = D_800F4D70.first = D_800F4D40;
    D_800F4D8A = 0;
}
