#include "game/system/pad.h"
#include "rr/overlay662_input.h"

void ovl662_poll_dpad_direction_repeat(u32 *output)
{
    u32 result=0;
    if (PlayerControllers[0].input00.fields.type != 0x12) {
        if (PlayerControllers[0].input00.fields.buttons & 0x1000) {
            ovl662_dpad_repeat_up++;
            if ((s32)ovl662_dpad_repeat_up >= 47) ovl662_dpad_repeat_up=40;
            if (ovl662_dpad_repeat_up == 1 || ovl662_dpad_repeat_up == 40) result |= 1;
            *output=result;
        } else {
            ovl662_dpad_repeat_up=0;
        }
        if (PlayerControllers[0].input00.fields.buttons & 0x4000) {
            ovl662_dpad_repeat_down++;
            if ((s32)ovl662_dpad_repeat_down >= 47) ovl662_dpad_repeat_down=40;
            if (ovl662_dpad_repeat_down == 1 || ovl662_dpad_repeat_down == 40) result |= 2;
            *output=result;
        } else {
            ovl662_dpad_repeat_down=0;
        }
        if (PlayerControllers[0].input00.fields.buttons & 0x8000) {
            ovl662_dpad_repeat_left++;
            if ((s32)ovl662_dpad_repeat_left >= 47) ovl662_dpad_repeat_left=40;
            if (ovl662_dpad_repeat_left == 1 || ovl662_dpad_repeat_left == 40) result |= 4;
            *output=result;
        } else {
            ovl662_dpad_repeat_left=0;
        }
        if (PlayerControllers[0].input00.fields.buttons & 0x2000) {
            ovl662_dpad_repeat_right++;
            if ((s32)ovl662_dpad_repeat_right >= 47) ovl662_dpad_repeat_right=40;
            if (ovl662_dpad_repeat_right == 1 || ovl662_dpad_repeat_right == 40) result |= 8;
            *output=result;
        } else {
            ovl662_dpad_repeat_right=0;
        }
    }
}
