#include "rr/overlay662_progress_controller.h"

void ovl662_func_801161FC(void)
{
    s32 ready, changed, state, i, flags;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    ScreenFrame = (s32)((u32)ScreenFrame + 1U);
    switch ((u16)D_800F4D8A) {
    case 0:
        ready = func_8003B47C();
        if (ready == 1) {
            func_8002AF8C(1, 1, 1);
            D_800F4D8A = ready;
        }
        flags = 0x10002;
        break;
    case 1:
        changed = ovl662_func_8011662C(ovl662_D_801272C0, 0);
        state = D_800F3AFC->state10;
        if (state == 3) {
            if (changed) {
                ovl662_func_80116690();
            }
            if (D_800F3A38[0].a) {
                if ((s16)ovl662_D_801272D8[D_800F4D98.bytes[0]] > 0) {
                    PlaySfx(1);
                    D_800F2DE4 = 0;
                    D_800F4D8A = 2;
                    D_800F4D80->byte14 = state;
                    D_800F3AFC->state10 = 4;
                } else {
                    PlaySfx(4);
                }
            } else if (D_800F3A38[0].b) {
                PlaySfx(2);
                D_800F4D8A = 8;
                D_800AC5D0 = 64;
                D_800F4D80->byte14 = state;
                D_800F3AFC->state10 = 4;
            }
        }
        flags = 0x10002;
        break;
    case 2:
        if (D_800F3AFC->state10 == 11 && D_800F4D80->byte14 == 5) {
            ovl662_func_80115EC4(0);
            D_800F4D8A = 3;
        }
        flags = 0x10006;
        break;
    case 3:
        changed = ovl662_func_8011662C(ovl662_D_801272C0, 1);
        state = D_800F3AFC->state10;
        if (state == 3) {
            if (changed) {
                ovl662_func_80116808();
            }
            if (D_800F3A38[0].a) {
                if ((s16)ovl662_D_80127328[D_800F4D98.bytes[0]][D_800F4D98.bytes[1]] > 0) {
                    PlaySfx(1);
                    D_800F2DE4 = 0;
                    D_800F4D8A = 4;
                    D_800F4D80->byte14 = state;
                    D_800F3AFC->state10 = 4;
                } else {
                    PlaySfx(4);
                }
            } else if (D_800F3A38[0].b) {
                PlaySfx(2);
                ovl662_D_801272D4 = 1;
                D_800F4D8A = 4;
                D_800F4D80->byte14 = state;
                D_800F3AFC->state10 = 4;
            }
        }
        flags = 0x10006;
        break;
    case 4:
        if (D_800F3AFC->state10 == 11 && D_800F4D80->byte14 == 5) {
            i = 0;
            if (ovl662_D_801272D4 == 0) {
                ovl662_func_80116960();
                return;
            }
            for (; i < 4; i++) {
                D_800F4D8C[i].phase18 = (s32)((u32)D_800F4D8C[i].phase18 - 0x30000U);
            }
            ovl662_func_80116020();
            D_800F4D8A = 1;
            ovl662_D_801272D4 = 0;
        }
        flags = 0x10006;
        break;
    case 8:
        D_800AC5D0--;
        func_8002AF8C(0, 1, 1);
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,
                     (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            ovl662_func_8011512C();
            D_800F4C6A = 1;
            D_800F3A26 = 64;
            D_800F4D8A = 0;
        }
        flags = 0x10002;
        break;
    }
    func_8003EC88(0);
    func_8003D6F0(0);
    func_8002A344(flags);
}
