#include "game/system/pad.h"
#include "rr/overlay662_input.h"

void ovl662_func_8011D730(u32 *output)
{
    u32 result=0;
    s32 axis;
    if (PlayerControllers[0].input00.fields.type == 0x73 && PlayerControllers[0].kind1C != 0 && PlayerControllers[0].flag40 == 0) {
        axis=PlayerControllers[0].axes14.bytes[3];
        if (axis < 32) {
            ovl662_D_80126C18++;
            if ((s32)ovl662_D_80126C18 >= 47) ovl662_D_80126C18=40;
            if (ovl662_D_80126C18 == 1 || ovl662_D_80126C18 == 40) result |= 1;
            *output=result;
        } else {
            ovl662_D_80126C18=0;
        }
        if (axis >= 225) {
            ovl662_D_80126C1C++;
            if ((s32)ovl662_D_80126C1C >= 47) ovl662_D_80126C1C=40;
            if (ovl662_D_80126C1C == 1 || ovl662_D_80126C1C == 40) result |= 2;
            *output=result;
        } else {
            ovl662_D_80126C1C=0;
        }
        axis=PlayerControllers[0].axes14.bytes[2];
        if (axis < 32) {
            ovl662_D_80126C20++;
            if ((s32)ovl662_D_80126C20 >= 47) ovl662_D_80126C20=40;
            if (ovl662_D_80126C20 == 1 || ovl662_D_80126C20 == 40) result |= 4;
            *output=result;
        } else {
            ovl662_D_80126C20=0;
        }
        if (axis >= 225) {
            ovl662_D_80126C24++;
            if ((s32)ovl662_D_80126C24 >= 47) ovl662_D_80126C24=40;
            if (ovl662_D_80126C24 == 1 || ovl662_D_80126C24 == 40) result |= 8;
            *output=result;
        } else {
            ovl662_D_80126C24=0;
        }
    }
}
