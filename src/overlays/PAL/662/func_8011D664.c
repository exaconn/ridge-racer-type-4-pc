#include "game/system/pad.h"
#include "rr/overlay662_input.h"

void ovl662_func_8011D664(GaragePoint *output)
{
    GaragePoint point;
    u32 buttons;
    s32 step;
    PsyqMemset(&point,0,4);
    if (PlayerControllers[0].input00.fields.type != 0x12) {
        buttons = PlayerControllers[0].input00.fields.buttons;
        step=1;
        if (buttons & 8) step=4;
        if (buttons & 0x1000) point.y=-(step*2);
        if (buttons & 0x4000) point.y=step*2;
        if (buttons & 0x8000) point.x=-step;
        if (buttons & 0x2000) point.x=step;
    }
    if ((u16)point.x | (u16)point.y) {
        output->x=point.x;
        output->y=point.y;
    }
}
