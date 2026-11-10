#include "rr/overlay662_sprites.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000)
#endif
u8 *ovl662_func_8011B2D4(s32 index,u8 *cursor,s32 x,s32 y)
{
    u32 *ordering=RR_GPU_SCRATCH->order+128;
    SpritePacket *packet=(SpritePacket *)cursor;
    GarageSpriteRecord *record;
    s32 i;
    if (index<0) {
        for (i=0;i<72;i++) {
            record=&ovl662_D_80126260[i];
            ovl662_D_80127548[record->key0C]=record;
        }
        return 0;
    }
    record=ovl662_D_80127548[index];
    packet->len=4;
    packet->code=0x64;
    if (record->flags10 & 1) packet->code = packet->code | 2;
    else packet->code = packet->code & ~2;
    packet->x=x;packet->y=y;
    packet->code |=1;
    packet->w=record->width04;
    packet->h=record->height06;
    packet->u=record->u08;packet->v=record->v09;
    packet->clut=record->clut0A;
    packet->addr=ordering[record->depth00];
    ordering[record->depth00]=(ordering[record->depth00]&0xFF000000) | ((unsigned long)packet&0xFFFFFF);
    return (u8 *)(packet+1);
}
