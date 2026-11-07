#include "rr/overlay662_input.h"

void ovl662_func_8011D8E4(u32 *output)
{
    u32 result=0;
    if (PlayerControllers[0].type01 != 0x12) {
        if (PlayerControllers[0].buttons02 & 0x1000) {
            ovl662_D_80126C28++;
            if ((s32)ovl662_D_80126C28 >= 47) ovl662_D_80126C28=40;
            if (ovl662_D_80126C28 == 1 || ovl662_D_80126C28 == 40) result |= 1;
            *output=result;
        } else {
            ovl662_D_80126C28=0;
        }
        if (PlayerControllers[0].buttons02 & 0x4000) {
            ovl662_D_80126C2C++;
            if ((s32)ovl662_D_80126C2C >= 47) ovl662_D_80126C2C=40;
            if (ovl662_D_80126C2C == 1 || ovl662_D_80126C2C == 40) result |= 2;
            *output=result;
        } else {
            ovl662_D_80126C2C=0;
        }
        if (PlayerControllers[0].buttons02 & 0x8000) {
            ovl662_D_80126C30++;
            if ((s32)ovl662_D_80126C30 >= 47) ovl662_D_80126C30=40;
            if (ovl662_D_80126C30 == 1 || ovl662_D_80126C30 == 40) result |= 4;
            *output=result;
        } else {
            ovl662_D_80126C30=0;
        }
        if (PlayerControllers[0].buttons02 & 0x2000) {
            ovl662_D_80126C34++;
            if ((s32)ovl662_D_80126C34 >= 47) ovl662_D_80126C34=40;
            if (ovl662_D_80126C34 == 1 || ovl662_D_80126C34 == 40) result |= 8;
            *output=result;
        } else {
            ovl662_D_80126C34=0;
        }
    }
}
