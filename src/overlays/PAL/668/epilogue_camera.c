#include "rr/overlay668_track_project.h"
#include "rr/overlay668_camera.h"
#include "psyq/libgte/gtemat5.h"
#include "types.h"
#include "rr/frame_storage_types.h"
#include "psyq/libgpu/gpudisp.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/gpu_scratch_types.h"
#include "rr/gpu_address.h"
#include "rr/sprite_emit.h"
#include "scratchpad.h"

/* The initializer supplies its position pointer; this body uses globals. */
void ovl668_project_camera_to_track(s32 *unused_position) {
    TrackOffsetArgument offsets[2];
    s32 position[3];
    s32 height;
    s32 idx;

    (void)unused_position;

    position[0] = RaceContext.position450[0] >> 2;
    position[1] = RaceContext.position450[1] >> 2;
    position[2] = RaceContext.position450[2] >> 2;
    idx = FindTrackNode(position, RaceContext.node498, (TrackNodeCount + 1U));
    RaceContext.node498 = idx;
    offsets[0].value = (RaceContext.position450[0] / 4) - (u16)TrackNodes[idx].road.x;
    offsets[1].value = (RaceContext.position450[2] / 4) - (u16)TrackNodes[idx].road.z;
    ProjectToTrackSegment(offsets[0], offsets[1], RaceContext.node498, &RaceContext.fraction49C, &RaceContext.lateral49E, &height);
}

void ovl668_step_epilogue_camera_yaw(void) {
    s32 unused[4];
    s32 var_a3;
    s32 idx1;
    s32 idx2;
    u32 old_unk462;
    s16 new_unk462;
    s32 diff;
    u32 unk4A0;

    var_a3 = RaceContext.fraction49C;
    idx1 = RaceContext.node498 - ((3072 - var_a3) / 1024);
    idx2 = idx1 - 1;
    if (idx1 < 0) {
        idx1 += TrackNodeCount;
        idx2 = idx1 - 1;
    }
    if (idx2 < 0) {
        idx2 += TrackNodeCount;
    }
    InterpolateLanePointXYZ(&TrackNodes[idx1], &TrackNodes[idx2], 0, (1024 - var_a3) & 0x3FF, RaceContext.target468);
    RaceContext.delta478[0] = ((u32)RaceContext.target468[0] - (RaceContext.position450[0] / 4));
    RaceContext.delta478[1] = ((u32)RaceContext.target468[1] - (RaceContext.position450[1] / 4));
    RaceContext.delta478[2] = ((u32)RaceContext.target468[2] - (RaceContext.position450[2] / 4));
    old_unk462 = RaceContext.yaw462;
    new_unk462 = 1024 - Atan2Table(RaceContext.delta478[0], RaceContext.delta478[2]);
    old_unk462 = (new_unk462 - old_unk462) * 2;
    diff = old_unk462;
    old_unk462 &= 0xFFF;
    RaceContext.yaw462 = new_unk462;
    if (diff & 0x800) {
        old_unk462 |= ~0xFFF;
    }
    unk4A0 = RaceContext.velocity4A0;
    RaceContext.velocity4A0 = (unk4A0 + ((s32)((old_unk462 - unk4A0) * 15U) / 16));
    RaceContext.roll464 = (s32)((u32)RaceContext.velocity4A0 + RaceContext.roll464) / 2;
}

void ovl668_step_epilogue_camera_pitch(void) {
    s32 sp18[4];
    s32 sp28[4];
    s32 sp38[4];
    s32 unused[2];
    s32 idx;
    /* ASM_BARRIER: Keep the neighbour index in v1 across the two sample calculations. */
    register s32 next __asm__("$3");
    u32 dx;
    u32 dz;

    idx = RaceContext.node498;
    next = idx + 1;
    if (next >= TrackNodeCount) {
        next = 0;
    }
    InterpolateLanePointXYZ(&TrackNodes[idx], &TrackNodes[next], RaceContext.lateral49E, RaceContext.fraction49C, sp18);
    next = RaceContext.node498;
    idx = next - 1;
    RaceContext.position450[1] = (sp18[1] - 120U);
    if (idx < 0) {
        idx += TrackNodeCount;
    }
    InterpolateLanePointXYZ(&TrackNodes[idx], &TrackNodes[next], RaceContext.lateral49E, RaceContext.fraction49C, sp28);
    dx = (((u32)sp28[0] - sp18[0]) << 2);
    sp38[0] = dx;
    dz = (((u32)sp28[2] - sp18[2]) << 2);
    sp38[2] = dz;
    sp38[1] = ((u32)sp28[1] - sp18[1]);
    RaceContext.pitch460 = 1024 - Atan2Table(sp38[1], CordicSqrt((dx * dx + dz * dz)) >> 6);
}

void ovl668_apply_epilogue_camera(void) {
    PreviewMatrixWorkspace sp10;
    PreviewVector8 sp30;
    s32 *scratch = RR_REPLAY_SCRATCH_WORDS;

    scratch[6] = RaceContext.pitch460;
    scratch[7] = RaceContext.yaw462;
    scratch[8] = RaceContext.roll464;
    scratch[2] = RaceContext.position450[0];
    scratch[3] = RaceContext.position450[1];
    scratch[4] = RaceContext.position450[2];
    sp30.x = scratch[6];
    sp30.y = scratch[7];
    sp30.z = scratch[8];
    RotMatrix(&sp30, &sp10.matrix);
    ApplyMatrixSV((const MatrixWords32 *)(&sp10.matrix), (const Vec3s8 *)(RaceContext.offset488), (s16 *)(&sp30));
    PlayerCars[0].position20.words[0] = (RaceContext.position450[0] / 4) + sp30.x;
    PlayerCars[0].position20.words[1] = (RaceContext.position450[1] / 4) + sp30.y;
    PlayerCars[0].position20.words[2] = (RaceContext.position450[2] / 4) + sp30.z;
    PlayerCars[0].rot50 = RaceContext.pitch460 + RaceContext.angle490;
    PlayerCars[0].rot54 = RaceContext.yaw462;
    PlayerCars[0].rot58 = RaceContext.roll464;
    BuildCameraMatrices();
}

s32 ovl668_calc_credit_line_fade(s16 arg0) {
    if (arg0 >= 157) {
        return 0;
    }
    if (arg0 >= 140) {
        return (156 - arg0) << 4;
    }
    if (arg0 < 48) {
        return 0;
    }
    if (arg0 < 65) {
        return (arg0 - 48) << 4;
    }
    return 256;
}

/* Textured gouraud quad (GPU command 0x3E, 13-word packet). */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad3;
    u8 r3, g3, b3, pad4;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad5;
} StrCreditQuad;


extern s32 EpilogueCreditScrollY;
const s32 ovl668_credit_tpages[2] = {0x35, 0x36};
const s32 ovl668_credit_clut = 0x7F94;
s32 ovl668_calc_credit_line_fade(s16 arg0);

typedef struct {
    StrCreditQuad *cursor;
} StrCreditScratch;

#define RR_STR_SCRATCH ((StrCreditScratch *)0x1F800000)

/* Draws one 8-pixel band of the scrolling credits texture. */
void ovl668_draw_credit_band(s32 line, s32 unused) {
    u8 color[4] = {0, 0, 255, 8};
    s32 page;
    u8 v;
    s32 y;
    s32 y2;
    s32 top;
    s32 bottom;
    StrCreditQuad *p;
    u32 *ot;

    page = line / 31;
    v = (line % 31) * 8;
    y = EpilogueCreditScrollY / 100 + line * 8;
    color[1] = v;
    if ((y - 1) < 239U) {
        top = ovl668_calc_credit_line_fade(y) * 160 / 256;
        y2 = y + 8;
        bottom = ovl668_calc_credit_line_fade(y2) * 160 / 256;
        p = RR_STR_SCRATCH->cursor;
        ((u8 *)p)[3] = 12;
        p->code = 0x3E;
        p->x0 = 32;
        p->y0 = y;
        p->x1 = 287;
        p->y1 = y;
        p->x2 = 32;
        p->y2 = y2;
        p->x3 = 287;
        p->y3 = y2;
        p->u0 = 0;
        p->v0 = v;
        p->u1 = 255;
        p->v1 = v;
        p->u2 = 0;
        p->v2 = v + 8;
        p->u3 = 255;
        p->v3 = v + 8;
        p->r0 = top;
        p->g0 = top;
        p->b0 = top;
        p->r1 = top;
        p->g1 = top;
        p->b1 = top;
        p->r2 = bottom;
        p->g2 = bottom;
        p->b2 = bottom;
        p->r3 = bottom;
        p->g3 = bottom;
        p->b3 = bottom;
        p->clut = ovl668_credit_clut;
        p->tpage = ovl668_credit_tpages[page];
        ot = &CurrentDrawBuf->buffer.ordering70.words[0];
        p->tag = (p->tag & 0xFF000000) | (*ot & 0x00FFFFFF);
        *ot = (*ot & 0xFF000000) | ((u32)p & 0x00FFFFFF);
        p++;
        RR_STR_SCRATCH->cursor = p;
    }
}

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
