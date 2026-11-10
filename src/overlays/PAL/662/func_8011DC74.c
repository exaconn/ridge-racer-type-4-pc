/* CC1_FLAGS: -fno-schedule-insns */


#include "rr/overlay662_garage.h"
GaragePoint ovl662_get_rect_center(s32 index)
{
    GaragePoint point;
    GarageRectangle *rectangle=ovl662_button_rect_lookup[index];
    s32 width=(s16)(*rectangle)[2];
    s32 x=(s16)(*rectangle)[0];
    s32 y=(s16)(*rectangle)[1];
    u16 height=(*rectangle)[3];
    x += width >> 1;
    y += (s16)height >> 1;
    point.x=x;
    point.y=y;
    return point;
}
