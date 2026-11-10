#include "game/system/pad.h"
#include "rr/overlay662_input.h"
void ovl662_func_8011D338(GaragePoint *output)
{
    GaragePoint point;
    s32 step,delta;
    u32 axis;
    PsyqMemset(&point,0,4);
    if (PlayerControllers[0].input00.fields.type == 0x73 && PlayerControllers[0].kind1C != 0 && PlayerControllers[0].flag40 == 0) {
        step=1;
        if (PlayerControllers[0].input00.fields.buttons & 8) step=4;
        axis=PlayerControllers[0].axes14.bytes[2];
        if (axis <112) {
            delta=(axis-112)*step;
            if (ovl662_D_80126C14[0] <0) ovl662_D_80126C14[0]+=delta;
            else ovl662_D_80126C14[0]=delta;
            point.x=ovl662_D_80126C14[0]/112;
            ovl662_D_80126C14[0]-=point.x*112;
        } else if (axis>=145) {
            delta=(axis-143)*step;
            if (ovl662_D_80126C14[0] >0) ovl662_D_80126C14[0]+=delta;
            else ovl662_D_80126C14[0]=delta;
            point.x=ovl662_D_80126C14[0]/112;
            ovl662_D_80126C14[0]-=point.x*112;
        } else ovl662_D_80126C14[0]=0;
        axis=PlayerControllers[0].axes14.bytes[3];
        if (axis <112) {
            delta=(axis-112)*step;
            if (ovl662_D_80126C14[1] <0) ovl662_D_80126C14[1]+=delta;
            else ovl662_D_80126C14[1]=delta;
            point.y=(s16)(ovl662_D_80126C14[1]/112)*2;
            ovl662_D_80126C14[1]-=point.y*112;
        } else if (axis>=145) {
            delta=(axis-143)*step;
            if (ovl662_D_80126C14[1] >0) ovl662_D_80126C14[1]+=delta;
            else ovl662_D_80126C14[1]=delta;
            point.y=(s16)(ovl662_D_80126C14[1]/112)*2;
            ovl662_D_80126C14[1]-=point.y*112;
        } else ovl662_D_80126C14[1]=0;
    }
    if ((u16)point.x | (u16)point.y) {output->x=point.x;output->y=point.y;}
}
