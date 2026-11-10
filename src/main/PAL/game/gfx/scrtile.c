/* CC1_FLAGS: -fno-strength-reduce */
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/screen_tiles.h"
#include "psyq/libgpu/primatt5.h"
#include "psyq/libgpu/settrans.h"
#include "rr/sprite_emit.h"
#include "rr/animated_glyph.h"
void AddScreenFadeTiles(u32 *ordering,s32 intensity,u32 mode) {
    u8 *next,*fields,*packet;
    s32 index;
    s16 y;
    u16 last_height;
    /* ASM_BARRIER: prepare the ordering-table argument before advancing the packet. */
    register u32 *link_order __asm__("$4");
    s32 column;
    s32 x;
    GpuRectangleFields *rectangle;
    if(intensity<0)intensity=0;
    else if(intensity>=256)intensity=255;
    next=*(u8**)SCRATCHPAD;
    index=0;
    fields=next+RR_FIELD_OFFSET(GpuTilePacket16,b);
    do {
        SetTile(next);
        SetSemiTrans(next,1);
        column=index&1;
        x=column*160;
        rectangle=&CurrentDrawBuf->buffer.environment00.draw00.clip;
        ((s16*)fields)[1]=x;
        y=(rectangle->h/2)*(index/2);
        link_order=ordering;
        packet=next;
        next+=sizeof(GpuTilePacket16);
        ((s16*)fields)[3]=160;
        ((s16*)fields)[2]=y;
        last_height=rectangle->h;
        index++;
        fields[-2]=intensity;fields[-1]=intensity;fields[0]=intensity;
        ((s16*)fields)[4]=(s16)last_height/2;
        AddPrim(link_order,(u32*)packet);
        fields+=sizeof(GpuTilePacket16);
    }while(index<4);
    *(u8**)SCRATCHPAD=next;
    *(u8**)SCRATCHPAD=AddDrawModePrim(ordering,(GpuDrawModePacket *)next,(u16)mode);
}
