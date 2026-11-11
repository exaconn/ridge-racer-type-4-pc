#include "rr/gpu_scratch_types.h"
#include "rr/frame_storage_types.h"
#include "rr/screen_tiles.h"
#include "psyq/libgpu/primatt5.h"
#include "psyq/libgpu/settrans.h"
#include "rr/sprite_emit.h"
#include "rr/animated_glyph.h"
void AddScreenFadeTiles(GpuDmaTag *ordering,s32 intensity,u32 mode) {
    GpuTilePacket *tile;
    s32 index;
    s32 column;
    GpuRectangleFields *rectangle;
    if(intensity<0)intensity=0;
    else if(intensity>=256)intensity=255;
    tile=(GpuTilePacket *)RR_GPU_SCRATCH->cursor;
    index=0;
    do {
        SetTile(tile);
        SetSemiTrans(tile,1);
        column=index&1;
        rectangle=&CurrentDrawBuf->buffer.environment00.draw00.clip;
        tile->x=column*160;
        tile->w=160;
        tile->y=(rectangle->h/2)*(index/2);
        tile->r=intensity;tile->g=intensity;tile->b=intensity;
        tile->h=rectangle->h/2;
        AddPrim(ordering,tile);
        tile++;
        index++;
    }while(index<4);
    RR_GPU_SCRATCH->cursor=(u8 *)tile;
    RR_GPU_SCRATCH->cursor=AddDrawModePrim(ordering,(GpuDrawModePacket *)tile,(u16)mode);
}
