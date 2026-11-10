#include "rr/overlay662_buttons.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
void ovl662_draw_button_highlight(s32 index)
{
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    GarageButton *button=(GarageButton *)ovl662_button_rect_lookup[index];
    s16 timer=button->timer0E;
    s32 sprite;
    if(timer==0)sprite=button->sprite08;
    else {
        sprite=button->sprite08+1;
        if(timer>0)button->timer0E=timer-1;
    }
    scratch->cursor=ovl662_emit_indexed_sprite(sprite,scratch->cursor,(s16)button->rectangle[0],(s16)button->rectangle[1]);
}
