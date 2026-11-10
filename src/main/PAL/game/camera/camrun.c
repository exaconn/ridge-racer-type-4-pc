#include "game/car/carbound.h"
#include "rr/gpu_address.h"
#include "types.h"
#include "rr/camera_motion.h"
#include "rr/game_motion.h"
#include "scratchpad.h"
#include "game/camera/cammove.h"
#include "game/camera/campath.h"
#include "game/gfx/motion1.h"
#include "game/gfx/ring2.h"
#include "game/gfx/ring.h"
#include "psyq/libgpu/gpudisp.h"
#include "game/camera/camrun.h"
#include "rr/bios_devices.h"
#include "rr/frame_storage_types.h"
#include "rr/race_setup.h"
#include "rr/rro_model.h"
#include "game/camera/viewcam.h"

extern MotionClock20 D_800F7488;
extern MotionClock20 D_800F7624;
extern void SetGeomOffset(s32, s32);
extern void SetGeomScreen(s32);

void StepCameraScript(void) {
    RroSceneScratchPrefix *scratch = (RroSceneScratchPrefix *)SCRATCHPAD;
    s32 scriptChanged = 0;
    MotionClock20 *clock;

    if (VSyncThreshold == 0x80) {
        RaceContext.controller3D8->scale4C = 2;
    } else {
        RaceContext.controller3D8->scale4C = 1;
    }
    if (RaceContext.controller3D8->alternateClock5C == 0) {
        clock = &D_800F7624;
    } else {
        clock = &D_800F7488;
    }
    if (clock->active != 0) {
        if (++RaceContext.controller3D8->scriptTimer6A >= RaceContext.controller3D8->script64[RaceContext.controller3D8->scriptIndex68].duration) {
            RaceContext.controller3D8->scriptIndex68++;
            if (RaceContext.controller3D8->script64[RaceContext.controller3D8->scriptIndex68].operation < 0) {
                RaceContext.controller3D8->scriptIndex68 = 0;
            }
            RaceContext.controller3D8->scriptOperation6C = RaceContext.controller3D8->script64[RaceContext.controller3D8->scriptIndex68].operation;
            RaceContext.controller3D8->scriptArgument70 = RaceContext.controller3D8->script64[RaceContext.controller3D8->scriptIndex68].argument;
            scriptChanged = 1;
            RaceContext.controller3D8->scriptTimer6A = 0;
        }
    }
    switch (RaceContext.controller3D8->scriptOperation6C) {
    case 0:
        if (scriptChanged != 0) {
            InitCameraMotion(RaceContext.controller3D8->scriptArgument70);
        }
        UpdateCameraMotion(clock);
        break;
    case 1:
        if (scriptChanged != 0) {
            InitMotionPanel(RaceContext.controller3D8->scriptArgument70);
        }
        UpdateMotionPanel(clock);
        break;
    case 2:
        if (scriptChanged != 0) {
            InitScrollTrack(RaceContext.controller3D8->scriptArgument70);
        }
        UpdateScrollTrack(clock);
        break;
    case 3:
        if (scriptChanged != 0) {
            InitMotionTrack(RaceContext.controller3D8->scriptArgument70);
        }
        UpdateMotionTrack(clock);
        break;
    }
    if (RaceContext.controller3D8->drawEnabled54 != 0) {
        scratch->view.projection.pose08.position00.vector = RaceContext.controller3D8->position;
        scratch->view.projection.pose08.angles10.vector = RaceContext.controller3D8->target;
        BuildCameraMatrices();
        SetGeomScreen(RaceContext.controller3D8->viewDistance48);
        if (RaceContext.controller3D8->fullViewport50 == 0) {
            SetupViewport(6);
            SetGeomOffset(0xA0, 0xE8);
            ScaleMatrixRow1(2, (MatrixWords32 *)SCRATCH_ROT_MATRIX);
        }
        scratch->depthShift60 = 5;
        scratch->view.projection.pointers.order = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
        if (RaceContext.controller3D8->drawEnabled54 != 0) {
            switch (RaceContext.controller3D8->scriptOperation6C) {
            case 0:
                DrawPresentationMotionModel();
                break;
            case 1:
                DrawMotionControllerPanel();
                break;
            case 2:
                DrawRingEffects();
                break;
            case 3:
                DrawPresentationTrackModel();
                break;
            }
        }
        if (RaceContext.controller3D8->stencilEnabled58 == 0) {
            ((GpuDmaTag *)scratch->view.projection.pointers.order)->bits.address = CurrentDrawBuf->buffer.ordering70.tags[702].bits.address;
            CurrentDrawBuf->buffer.ordering70.tags[702].bits.address = GpuDmaEncodeAddress(scratch->view.projection.pointers.order + 703);
        } else {
            GpuDrawModePacket *packet = (GpuDrawModePacket *)((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor;
            GpuDrawModePacket *maskPacket;

            SetDrawStp(packet, 0);
            packet->command[0] |= 2;
            packet->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[703].bits.address;
            CurrentDrawBuf->buffer.ordering70.tags[703].bits.address = GpuDmaEncodeAddress(packet);
            packet++;
            ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
            ((GpuDmaTag *)scratch->view.projection.pointers.order)->bits.address = CurrentDrawBuf->buffer.ordering70.tags[703].bits.address;
            CurrentDrawBuf->buffer.ordering70.tags[703].bits.address = GpuDmaEncodeAddress(scratch->view.projection.pointers.order + 703);
            packet = (GpuDrawModePacket *)((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor;
            SetDrawStp(packet, 0);
            packet->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[703].bits.address;
            CurrentDrawBuf->buffer.ordering70.tags[703].bits.address = GpuDmaEncodeAddress(packet);
            packet++;
            ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
            ((GpuTilePacket *)packet)->tag.bits.length = 3;
            ((GpuTilePacket *)packet)->code = 0x60;
            ((GpuTilePacket *)packet)->r = 0;
            ((GpuTilePacket *)packet)->g = 0;
            ((GpuTilePacket *)packet)->b = 0;
            ((GpuTilePacket *)packet)->x = 0;
            ((GpuTilePacket *)packet)->y = 0;
            ((GpuTilePacket *)packet)->w = 0x140;
            ((GpuTilePacket *)packet)->h = 0xF0;
            packet->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[703].bits.address;
            CurrentDrawBuf->buffer.ordering70.tags[703].bits.address = GpuDmaEncodeAddress(packet);
            maskPacket = (GpuDrawModePacket *)((GpuTilePacket *)packet + 1);
            ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)maskPacket;
            SetDrawStp(maskPacket, 1);
            __asm__ ("" : "+r"(maskPacket)); /* ASM_BARRIER: prevent CSE from folding maskPacket + 1 into packet + 28 */
            packet = maskPacket + 1;
            maskPacket->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[703].bits.address;
            CurrentDrawBuf->buffer.ordering70.tags[703].bits.address = GpuDmaEncodeAddress(maskPacket);
            ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
        }
        scratch->view.projection.pointers.order = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
    }
}

void InitCameraMotion(int selection) {
    PsyqBzero(&RaceContext.controller3D8->motion74, sizeof(CameraMotion70));
    if (selection < 4) {
        RaceContext.controller3D8->motion74.mode04 = 0;
        RaceContext.controller3D8->motion74.keys0C = D_800A1AA0[selection];
    } else {
        selection -= 4;
        RaceContext.controller3D8->motion74.mode04 = 1;
        RaceContext.controller3D8->motion74.keys0C = D_800A1B60[selection];
    }
    RaceContext.controller3D8->motion74.modelScale6C = 0x2D0;
    RaceContext.controller3D8->motion74.start10[0] =
        RaceContext.controller3D8->motion74.current40[0] =
        RaceContext.controller3D8->motion74.keys0C->x;
    RaceContext.controller3D8->motion74.start10[1] =
        RaceContext.controller3D8->motion74.current40[1] =
        RaceContext.controller3D8->motion74.keys0C->y;
    RaceContext.controller3D8->motion74.start10[2] =
        RaceContext.controller3D8->motion74.current40[2] =
        RaceContext.controller3D8->motion74.keys0C->z;
    RaceContext.controller3D8->motion74.key08 = 1;
}
