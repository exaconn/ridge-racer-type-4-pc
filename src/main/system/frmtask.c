#include "rr/gpu_address.h"
#include "game/ui/uipart1.h"
#include "game/sound/cdpos.h"
#include "game/gfx/drawlist.h"
/* RODATA 0x800102cc 0x24 */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"
#include "rr/frame_loop.h"
#include "rr/gpu_scratch_types.h"
#include "game/data/cdfile2.h"
#include "game/system/frmenv.h"
#include "game/system/pad.h"
#include "game/effect/spfade.h"
#include "psyq/libgpu/gpuenv.h"
#include "psyq/libgpu/gpudisp.h"
#include "game/system/frmtask.h"
#include "game/data/cdfile1.h"
#include "game/sound/sndslot.h"
#include "rr/course_choices.h"
#include "rr/menu_background.h"
#include "rr/driver_control.h"
#include "rr/overlay668_stream.h"
#include "game/sound/cdstart.h"
#include "game/course/trkintp2.h"
#include "rr/screen_tiles.h"
#include "game/sound/cdplay.h"

extern s16 FadeCountdown;

/* Host scratchpad buffer and the GPU packet cursor/ordering pointers that
 * occupied its first two words on the PS1 (include/scratchpad.h). */
_Alignas(16) u8 RrScratchpad[RR_SCRATCHPAD_SIZE];
GpuScratchPointers GpuScratch;

void FrmTaskModeEnter(void);
void FrmTaskModeUpdate(void);

void FrmTaskModeEnter(void) {
    FrmTaskState = FRAME_TASK_READ_START;
    FadeCountdown = -1;
    ModeGroup = 0;
    ModeIndex = 6;
    SubmodeState = 0;
}


void ResetFrmTaskState(void) {
    FrmTaskState = FRAME_TASK_READ_START;
    FadeCountdown = -1;
}

void FrmTaskModeUpdate(void) {
    FrameTaskFields *rampState;
    FrameTaskState *initialState;
    FrameTaskFields *revealState;
    FrameTaskState *animationState;
    GpuScratchPointers *scratch;
    GpuCommandPacket *packet;
    int fileIndex;

    switch (FrmTaskState) {
    case FRAME_TASK_READ_START:
        fileIndex = GetR4FileIndex(0, 0x15);
        if (CdFileReadStart(fileIndex, LoadCursor)) {
            FrmTaskState = FRAME_TASK_READ_WAIT;
        }
        return;
    case FRAME_TASK_READ_WAIT:
        if (CdFileBytesLoaded()) {
            FrmTaskState = FRAME_TASK_SETUP;
        }
        return;
    case FRAME_TASK_SETUP:
        CdAudioInit();
        CdAudioSelectTrack(0, 0x24);
        CdAudioBeginPlay(0);
        LoadTimList((s32 *)LoadCursor);
        FrmTaskState = FRAME_TASK_REVEAL;
        initialState = (FrameTaskState *)LoadCursor;
        ActiveFrameTaskScript = initialState;
        LoadCursor = (u8 *)(initialState + 1);
        initialState->fields.intensity[0] = 0;
        initialState->fields.intensity[1] = 0;
        initialState->fields.intensity[2] = 0;
        initialState->fields.ramp[0] = 0;
        initialState->fields.ramp[1] = 0;
        initialState->fields.ramp[2] = 8;
        initialState->fields.spriteTick = 0;
        initialState->fields.frame = 0;
    case FRAME_TASK_REVEAL:
        revealState = &ActiveFrameTaskScript->fields;
        if (revealState->frame < 0x40) {
            if (revealState->frame == 8) {
                revealState->ramp[0] = 4;
            } else if (revealState->frame == 0x20) {
                revealState->ramp[1] = 4;
            }
            break;
        }
        FrmTaskState = FRAME_TASK_FADE;
    case FRAME_TASK_FADE:
        SetMenuFrameOptions(1, 1, 0);
        if (FadeCountdown > 0) {
            FadeCountdown--;
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.tags[0], (0x40 - FadeCountdown) * 4, 0x45);
            if (FadeCountdown == 0) {
                ModeGroup = 1;
                ModeIndex = 3;
                SubmodeState = 0;
            }
        } else if (MenuReturnSubmodeIndex) {
            FadeCountdown = 0x40;
            CdAudioFade(0, 0x40);
        }
        break;
    default:
        break;
    }
    /* Accumulators wrap through the corresponding unsigned halfword view. */
    rampState = &ActiveFrameTaskScript->fields;
    RampValueToLimit((u16 *)&rampState->intensity[0], &rampState->ramp[0], 0x80);
    rampState = &ActiveFrameTaskScript->fields;
    RampValueToLimit((u16 *)&rampState->intensity[1], &rampState->ramp[1], 0x80);
    rampState = &ActiveFrameTaskScript->fields;
    RampValueToLimit((u16 *)&rampState->intensity[2], &rampState->ramp[2], 0x80);
    EmitPresetSprite(ActiveFrameTaskScript->fields.intensity[0], 0x2F);
    EmitPresetSprite(ActiveFrameTaskScript->fields.intensity[1], 0x30);
    animationState = ActiveFrameTaskScript;
    EmitSpriteGrid(animationState->fields.intensity[2], &animationState->grid);
    scratch = RR_GPU_SCRATCH;
    packet = (GpuCommandPacket *)scratch->cursor;
    packet->command = 0xE100022A;
    RR_GPU_ADD_PRIM(scratch->order, packet, 1);
    scratch->order->length = 0;
    scratch->cursor += sizeof(GpuCommandPacket);
    UpdateScreenFade(0x1000C);
    ActiveFrameTaskScript->fields.frame++;
}


void MainLoop(void) {
    s32 threshold;
    u32 bank;
    FrameStorage *frame;
    MainLoopStartupHook();
    SetVideoMode(1);
    ResetCallback();
    SystemInit();
    VSyncThreshold=128;
    SetDispMask(0);
    ModeFrame=0;
    ModeGroup=1;
    ModeIndex=1;
    SubmodeState=0;
    FrameCounter=0;
    for (;;) {
        {
            s32 length=0x2C0;
            FrameStorage *frames;
            GpuScratchPointers *scratch;
            GpuDmaTag *ordering;
            frames=DrawBuffers;
            bank=FrameCounter;
            bank &=1;
            frame=&frames[bank];
            ordering=frame->buffer.ordering70.tags;
            CurrentDrawBuf=frame;
            DrawBufIndex=bank;
            scratch=RR_GPU_SCRATCH;
            scratch->order=ordering;
            scratch->cursor=frame->buffer.packets1670;
            ClearOTagR(ordering,length);
        }
        ClearOTagR(CurrentDrawBuf->buffer.secondaryOrderingB70.tags,0x2C0);
        CdFileStep();
        SoundUpdate();
        ModeFuncTable[ModeGroup][ModeIndex]();
        DrawSync(0);
        threshold=VSyncThreshold;
        do {} while (VSync(1)<threshold);
        if (HostFrameFields > 1) {
            VSync(HostFrameFields);
        } else {
            VSync(0);
        }
        HostFrameFields = 1;
        PutDispEnv(&CurrentDrawBuf->buffer.environment00.display5C);
        PutDrawEnv(&CurrentDrawBuf->buffer.environment00.draw00);
        DrawOTag(&CurrentDrawBuf->buffer.ordering70.tags[703]);
        if(DrawSubOtFlag) DrawOTag(&CurrentDrawBuf->buffer.secondaryOrderingB70.tags[703]);
        PadUpdate();
        FrameCounter++;
    }
}

/* Initial values of the globals this module owns. */
u32 FrameCounter;
s32 HostFrameFields = 1;
FrameTaskState *ActiveFrameTaskScript;
s32 FrmTaskState;
