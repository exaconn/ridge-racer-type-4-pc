#include "game/system/pad.h"
#include "rr/overlay662_garage.h"
#include "rr/tables.h"
extern void *PsyqMemset(void *, s32, u32);
void ovl662_read_mouse_cursor_delta(GaragePoint *output)
{
    GaragePoint point;
    s32 value;
    s32 y;
    PsyqMemset(&point,0,4);
    if (PlayerControllers[1].input00.fields.type == 0x12) {
        value = (s32)(PlayerControllers[1].axes14.bytes[0] * 0x1000000U) >> 25;
        if (value < -3 || value > 3) value += value / 2;
        if (value) point.x=value;
        y = PlayerControllers[1].axes14.signedBytes[1];
        if (y < -7 || y > 7) y += (y + (s32)((u32)y >> 31)) >> 1;
        if (y) point.y=y;
    }
    if ((u16)point.x | (u16)point.y) {
        output->x=point.x;
        output->y=point.y;
    }
}
