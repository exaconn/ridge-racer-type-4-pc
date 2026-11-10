#include "rr/draw_environment.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "rr/sprite_emit.h"
#include "scratchpad.h"

extern s32 DrawBufIndex;

/* Queues the letterbox drawing-environment / fade packets for one pass. */
void ovl668_emit_letterbox_pass(s32 mode) {
    DrawEnvironment env;
    u32 *packet;
    u32 *ot;
    GpuTilePacket *tile;

    switch (mode) {
    case 0:
        SetDefDrawEnv(&env, 0, (DrawBufIndex * 256U) | 40, 320, 160);
        packet = (u32 *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
        SetDrawEnv(packet, &env);
        ot = &CurrentDrawBuf->buffer.ordering70.words[703];
        *packet = (*packet & 0xFF000000) | (*ot & 0x00FFFFFF);
        *ot = (*ot & 0xFF000000) | (GpuDmaEncodeAddress(packet) & 0x00FFFFFF);
        packet += 16;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)packet;
        break;
    case 1:
        SetDefDrawEnv(&env, 0, DrawBufIndex * 256U, 320, 256);
        packet = (u32 *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
        SetDrawEnv(packet, &env);
        ot = &CurrentDrawBuf->buffer.ordering70.words[0];
        *packet = (*packet & 0xFF000000) | (*ot & 0x00FFFFFF);
        *ot = (*ot & 0xFF000000) | (GpuDmaEncodeAddress(packet) & 0x00FFFFFF);
        packet += 16;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)packet;
        break;
    case 2:
        tile = (GpuTilePacket *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
        tile->tag.bits.length = 3;
        tile->code = 0x62;
        tile->y = 40;
        tile->w = 320;
        tile->h = 160;
        tile->r = 24;
        tile->g = 24;
        tile->b = 12;
        tile->x = 0;
        ot = &CurrentDrawBuf->buffer.ordering70.words[0];
        tile->tag.bits.address = ((GpuDmaTag *)ot)->bits.address;
        *ot = (*ot & 0xFF000000) | (GpuDmaEncodeAddress(tile) & 0x00FFFFFF);
        tile++;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)tile;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddDrawModePrim(ot, (GpuDrawModePacket *)tile, 37);
        break;
    case 3:
        SetDefDrawEnv(&env, 0, (DrawBufIndex * 256U) | 80, 320, 160);
        packet = (u32 *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
        SetDrawEnv(packet, &env);
        ot = &CurrentDrawBuf->buffer.ordering70.words[0];
        *packet = (*packet & 0xFF000000) | (*ot & 0x00FFFFFF);
        *ot = (*ot & 0xFF000000) | (GpuDmaEncodeAddress(packet) & 0x00FFFFFF);
        packet += 16;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)packet;
        break;
    }
}
