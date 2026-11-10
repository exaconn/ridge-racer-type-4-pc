/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/screen_tiles.h"

extern void SetTile(void *);

extern void SetSemiTrans(void *,s32);

extern void AddPrim(u32 *,u32 *);

extern u8 *AddDrawModePrim(u32 *,u8 *,u16);

void AddScreenFadeTiles(u32 *ordering,s32 intensity,u32 mode) {
    u8 *next,*fields,*packet;
    s32 index;
    s16 y;
    u16 last_height;
    /* ASM_BARRIER: prepare the ordering-table argument before advancing the packet. */
    register u32 *link_order __asm__("$4");
    s32 column;
    s32 x;
    GpuRectangle *rectangle;
    if(intensity<0)intensity=0;
    else if(intensity>=256)intensity=255;
    next=*(u8**)0x1F800000U;
    index=0;
    fields=next+RR_FIELD_OFFSET(GpuTilePacket16,b);
    do {
        SetTile(next);
        SetSemiTrans(next,1);
        column=index&1;
        x=column*160;
        rectangle=CurrentDrawBuf;
        *(s16*)(fields+2)=x;
        y=(rectangle->rect.h/2)*(index/2);
        link_order=ordering;
        packet=next;
        next+=sizeof(GpuTilePacket16);
        *(s16*)(fields+6)=160;
        *(s16*)(fields+4)=y;
        last_height=rectangle->rect.h;
        index++;
        fields[-2]=intensity;fields[-1]=intensity;fields[0]=intensity;
        *(s16*)(fields+8)=(s16)last_height/2;
        AddPrim(link_order,(u32*)packet);
        fields+=sizeof(GpuTilePacket16);
    }while(index<4);
    *(u8**)0x1F800000U=next;
    *(u8**)0x1F800000U=AddDrawModePrim(ordering,next,(u16)mode);
}
