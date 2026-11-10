#include "game/math/rotmat.h"
#include "psyq/libgte/gtemat5.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat.h"
#include "rr/matrix_workspace.h"
#include "types.h"
#include "rr/scene_camera_state.h"
#include "game/car/bounds.h"
#include "rr/player_cars.h"
#include "rr/camera_curve_tables.h"
#include "scratchpad.h"
#include "rr/matrix_words.h"
#include "psyq/libgpu/gpudisp.h"
#include "psyq/libgte/gtemat7.h"
#include "game/menu/menutran.h"
#include "game/gfx/scene1.h"
#include "rr/hud_number.h"
#include "rr/bios_devices.h"
#include "game/gfx/gfxotag.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu_address.h"
#include "game/data/globals.h"






extern s32 SceneOverlayEnabled;
extern GpuRectangleFields CarCameraSpringStiffness[];
extern GpuRectangleFields CarCameraPitchOffsets[][4];
extern int SetModelTexturePage();
extern int ReplayCamBankWeights[];
extern int DrawSubOtFlag;
extern short SelectedMakerIndex;
extern int ScaleMatrix();

void InitSceneOverlayState(void);
void SetSceneOverlayColor(int a0, u8 a1, u8 a2, u8 a3);
void SetSceneOverlayEnabled(s32 arg0);
void EmitSceneOverlaySprites(s32 a0, s32 a1, u8 *a2, s32 a3, s32 a4);

void InitSceneOverlayState(void) {
    RaceContext.overlayBlendE0 = 1;
    RaceContext.overlayEnabledD4 = 0;
    RaceContext.overlayStyleD8 = 0;
    RaceContext.overlayModeDC = 0;
    RaceContext.overlayColorE4[0] = 0x90;
    RaceContext.overlayColorE4[1] = 0x90;
    RaceContext.overlayColorE4[2] = 0x90;
}

void SetSceneOverlayColor(int a0, u8 a1, u8 a2, u8 a3) {
    RaceContext.overlayModeDC = a0;
    RaceContext.overlayColorE4[0] = a1;
    RaceContext.overlayColorE4[1] = a2;
    RaceContext.overlayColorE4[2] = a3;
}

void SetSceneOverlayEnabled(s32 arg0) {
    SceneOverlayEnabled = arg0;
}

void EmitSceneOverlaySprites(s32 a0, s32 a1, u8 *a2, s32 a3, s32 a4) {
    s32 i;
    GpuRectangleFields *uv;
    SpritePacket *s2;
    GpuDrawModePacket *s0;
    s16 y, x;
    /* ASM_BARRIER: Keep SCRATCHPAD in $s1 inside loop without hoisting */
    register GpuDrawModePacket **p1f80 asm("$17");
    unsigned long atlasAddress;
    s32 offset;

    atlasAddress = (unsigned long)CarCameraPitchOffsets;
    for (i = 0; i < 4; i++) {
        s2 = *(SpritePacket **)SCRATCHPAD;
        s2->len = 4;
        s2->code = 0x64;
        if (a4 != 0) {
            s2->code |= 2;
        } else {
            s2->code &= ~2;
        }
        if (a1 != 0) {
            s2->code |= 1;
        } else {
            s2->code &= ~1;
        }
        s2->r = a2[0];
        s2->g = a2[1];
        s2->b = a2[2];
        s2->x = CarCameraSpringStiffness[i].x;
        s2->y = CarCameraSpringStiffness[i].y;
        s2->w = CarCameraSpringStiffness[i].w;
        s2->h = CarCameraSpringStiffness[i].h;
        offset = (i << 3) + (a0 << 5);
        uv = (GpuRectangleFields *)(offset + atlasAddress);
        s2->u = uv->x;
        s2->v = uv->y;
        ((GpuCommandPacket *)s2)->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
        CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = GpuDmaEncodeAddress(s2);
        s2++;
        s0 = (GpuDrawModePacket *)((GpuCommandPacket *)s2 + 1);
        ((GpuCommandPacket *)s2)->tag.bits.length = 1;
        y = uv->h;
        x = uv->w;
        ((GpuCommandPacket *)s2)->command = 0xE1000200 | (((a3 & 3) << 5) | 0x100 | ((s16)((y & 0x100) >> 4)) | ((x & 0x3FF) >> 6) | ((s16)(y & 0x200) << 2));
        ((GpuCommandPacket *)s2)->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
        CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = GpuDmaEncodeAddress(s2);
        p1f80 = (GpuDrawModePacket **)SCRATCHPAD;
        *p1f80 = s0;
        SetDrawStp(s0, 0);
        s0->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
        CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = GpuDmaEncodeAddress(s0);
        *p1f80 = s0 + 1;
    }
}

void QueueDrawStp(void)
{
    GpuDrawModePacket *packet = *(GpuDrawModePacket **)SCRATCHPAD;
    u32 *ordering;
    SetDrawStp(packet, 1);
    ordering = &CurrentDrawBuf->buffer.ordering70.words[703];
    packet->tag.word = (packet->tag.word & 0xFF000000U) | (*ordering & 0xFFFFFFU);
    *ordering = (*ordering & 0xFF000000U) | (GpuDmaEncodeAddress(packet) & 0xFFFFFFU);
    packet++;
    *(GpuDrawModePacket **)SCRATCHPAD = packet;
}

void DrawSceneOverlayIfActive(void) {
    if (RaceContext.overlayEnabledD4 != 0) {
        EmitSceneOverlaySprites(DrawBufIndex, RaceContext.overlayStyleD8, &RaceContext.overlayColorE4[0], RaceContext.overlayModeDC, RaceContext.overlayBlendE0);
    }
}

void UpdateAndDrawSceneOverlay(void) {
    if (Pad0Buttons & 0x10) {
        RaceContext.overlayToggle3DC ^= 1;
        SetSceneOverlayEnabled(RaceContext.overlayToggle3DC);
    }
    DrawSceneOverlayIfActive();
}

void InitCameraState(int mode, int flags)
{
    int distance;
    u32 bit;
    SceneCameraState160 *camera;
    RaceContext.camera24C = &RaceContext.cameraEC;
    PsyqBzero(RaceContext.camera24C, sizeof(SceneCameraState160));
    RaceContext.camera24C->lighting78.matrix.fields.vectors[0].x = 0x1C1;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[0].y = -0xDF8;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[0].z = -0x79A;
    RaceContext.camera24C->lighting78.matrix.fields.colors[0].red = 0x80;
    RaceContext.camera24C->lighting78.matrix.fields.colors[0].green = 0x80;
    RaceContext.camera24C->lighting78.matrix.fields.colors[0].blue = 0x80;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[1].x = 0xB9A;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[1].y = -0xADB;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[1].z = 0x1E2;
    RaceContext.camera24C->lighting78.matrix.fields.colors[1].red = 0x40;
    RaceContext.camera24C->lighting78.matrix.fields.colors[1].green = 0x40;
    RaceContext.camera24C->lighting78.matrix.fields.colors[1].blue = 0x40;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[2].x = -0xA4D;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[2].y = -0x642;
    RaceContext.camera24C->lighting78.matrix.fields.vectors[2].z = 0xA86;
    RaceContext.camera24C->lighting78.matrix.fields.colors[2].red = 0x20;
    RaceContext.camera24C->lighting78.matrix.fields.colors[2].green = 0x20;
    RaceContext.camera24C->lighting78.matrix.fields.colors[2].blue = 0x20;
    RaceContext.camera24C->lighting78.ambient[0] = 0x40;
    RaceContext.camera24C->lighting78.ambient[1] = 0x40;
    RaceContext.camera24C->lighting78.ambient[2] = 0x40;
    RaceContext.camera24C->position110.xyz.x = 0;
    RaceContext.camera24C->position110.xyz.y = -0xF0;
    RaceContext.camera24C->position110.xyz.z = 0;
    RaceContext.camera24C->rotation120.fields.x = 0;
    RaceContext.camera24C->rotation120.fields.y = 0;
    RaceContext.camera24C->rotation120.fields.z = 0;
    RaceContext.camera24C->value12C = 0x800;
    RaceContext.camera24C->value130 = 0x1B8;
    RaceContext.camera24C->parts18[0].position00.xyz.x = -0xC0;
    RaceContext.camera24C->parts18[1].position00.xyz.x = 0xC0;
    if (!(ReplayCamBankWeights[SelectedMakerIndex] & ((bit = 1) << SelectedMakerCarIndex))) {
        RaceContext.camera24C->extentA0 = 0xA9;
        RaceContext.camera24C->mirrorA4 = 0;
    } else {
        RaceContext.camera24C->extentA0 = 0xCA;
        RaceContext.camera24C->mirrorA4 = 1;
    }
    camera = RaceContext.camera24C;
    ShowcaseCamDepthCurve[0].a = *(int *)&camera->extentA0 * 4;
    distance = *(int *)&camera->extentA0 - 4;
    distance = distance - (CarBoundsTable[0]->unk08 + (PlayerCars[0].halfSpan10E / 2));
    *(int *)&camera->distance100 = distance;
    RaceContext.camera24C->mode13C = mode;
    RaceContext.camera24C->mode140 = flags;
    SetModelTexturePage(0x19, distance, &RaceContext, camera);
    DrawSubOtFlag = 0;
}

void SetupObjectLightMatrices(const SceneCameraPose18 *pose, const PreviewLighting40 *lighting, MatrixWords32 *light, MatrixWords32 *rotation) {
    MatrixWords32 basis, temporary, axis;
    MatrixWords32 color;
    VectorWords16 scale;
    int channel;
    scale = UnitScaleVector;
    BuildRotMatrixY(&axis, -pose->rotation10.y);
    MulMatrix2((const MatrixWords32 *)(SCRATCH_ROT_MATRIX), &axis);
    BuildRotMatrixX(&basis, -pose->rotation10.x);
    BuildRotMatrixZ(&temporary, -pose->rotation10.z);
    MulMatrix(&basis, &temporary);
    MulMatrix2(&axis, &basis);
    BuildRotMatrixY(&axis, pose->rotation10.y);
    BuildRotMatrixX(&temporary, pose->rotation10.x);
    MulMatrix(&axis, &temporary);
    BuildRotMatrixZ(&temporary, pose->rotation10.z);
    MulMatrix2(&axis, &temporary);
    *rotation = temporary;
    channel = 0;
    do {
        LightMatrix.matrix.fields.rotation00[channel][0] = lighting->matrix.fields.vectors[channel].x;
        LightMatrix.matrix.fields.rotation00[channel][1] = lighting->matrix.fields.vectors[channel].y;
        LightMatrix.matrix.fields.rotation00[channel][2] = lighting->matrix.fields.vectors[channel].z;
        color.fields.rotation00[0][channel] = lighting->matrix.fields.colors[channel].red * 16;
        color.fields.rotation00[1][channel] = lighting->matrix.fields.colors[channel].green * 16;
        color.fields.rotation00[2][channel] = lighting->matrix.fields.colors[channel].blue * 16;
        channel++;
    } while (channel < 3);
    MulMatrix0(&LightMatrix.matrix, &basis, &temporary);
    SetLightMatrix(&temporary);
    SetColorMatrix(&color);
    SetBackColor(lighting->ambient[0], lighting->ambient[1], lighting->ambient[2]);
    ScaleMatrix(basis.w, scale.words);
    *light = basis;
}

void TransformPoint(const MatrixWords32 *rotation, const Vec3i *origin,
                    const Vec3i *point, Vec3i *output,
                    MatrixWords32 *rotationWorkspace) {
    Vec3s8 local;
    /* The existing fifth call slot is unused by this implementation. */
    (void)rotationWorkspace;
    local.fields.x = (s16)point->x;
    local.fields.y = (s16)point->y;
    local.fields.z = (s16)point->z;
    ApplyMatrix(rotation, &local, output);
    /* Unsigned accumulation retains the target's word wrap. */
    {
        s32 transformed = output->x;
        u32 translation = origin->x;
        output->x = transformed + translation;
    }
    {
        s32 transformed = output->y;
        u32 translation = origin->y;
        output->y = transformed + translation;
    }
    {
        s32 transformed = output->z;
        u32 translation = origin->z;
        output->z = transformed + translation;
    }
}
