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
extern s32 FrmTaskState;
extern short *ActiveFrameTaskScript;
extern int MenuReturnSubmodeIndex;
extern void CdAudioInit(void);
extern void SetMenuFrameOptions();
extern void EmitSpriteGrid();

void FrmTaskModeEnter(void);
void FrmTaskModeUpdate(void);

void FrmTaskModeEnter(void) {
    FrmTaskState = 0;
    FadeCountdown = -1;
    ModeGroup = 0;
    ModeIndex = 6;
    SubmodeState = 0;
}


void ResetFrmTaskState(void) {
    FrmTaskState = 0;
    FadeCountdown = -1;
}

void FrmTaskModeUpdate(void) {
    short *p;
    short *q;
    short *r;
    volatile int *g;
    int *s_;
    int *d_;
    int v;

    switch (FrmTaskState) {
    case 0:
        v = GetR4FileIndex(0, 0x15);
        if (CdFileReadStart(v, LoadCursor)) {
            FrmTaskState = 1;
        }
        return;
    case 1:
        if (CdFileBytesLoaded()) {
            FrmTaskState = 2;
        }
        return;
    case 2:
        CdAudioInit();
        CdAudioSelectTrack(0, 0x24);
        CdAudioBeginPlay(0);
        LoadTimList(LoadCursor);
        FrmTaskState = 7;
        q = (short *)LoadCursor;
        ActiveFrameTaskScript = q;
        LoadCursor = (u8 *)q + 0x10;
        q[1] = 0;
        q[2] = 0;
        q[3] = 0;
        q[4] = 0;
        q[5] = 0;
        q[6] = 8;
        q[7] = 0;
        q[0] = 0;
    case 7:
        r = ActiveFrameTaskScript;
        if (r[0] < 0x40) {
            if (r[0] == 8) {
                r[4] = 4;
            } else if (r[0] == 0x20) {
                r[5] = 4;
            }
            goto common;
        }
        FrmTaskState = 8;
    case 8:
        SetMenuFrameOptions(1, 1, 0);
        if (FadeCountdown > 0) {
            FadeCountdown--;
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - FadeCountdown) * 4, 0x45);
            if (FadeCountdown == 0) {
                ModeGroup = 1;
                ModeIndex = 3;
                SubmodeState = 0;
            }
        } else if (MenuReturnSubmodeIndex) {
            FadeCountdown = 0x40;
            CdAudioFade(0, 0x40);
        }
    case 3: case 4: case 5: case 6:
    default:
    common:
        p = ActiveFrameTaskScript;
        RampValueToLimit(p + 1, p + 4, 0x80);
        p = ActiveFrameTaskScript;
        RampValueToLimit(p + 2, p + 5, 0x80);
        p = ActiveFrameTaskScript;
        RampValueToLimit(p + 3, p + 6, 0x80);
        EmitPresetSprite(ActiveFrameTaskScript[1], 0x2F);
        EmitPresetSprite(ActiveFrameTaskScript[2], 0x30);
        p = ActiveFrameTaskScript;
        EmitSpriteGrid(p[3]);
        g = (int *)SCRATCHPAD;
        ((int *)g[0])[1] = 0xE100022A;
        s_ = (int *)g[1];
        d_ = (int *)g[0];
        *d_ = *s_;
        ((char *)g[0])[3] = 1;
        *(int *)g[1] = g[0];
        ((char *)g[1])[3] = 0;
        *(int *)SCRATCHPAD += 8;
        UpdateScreenFade(0x1000C);
        ActiveFrameTaskScript[0]++;
    }
}


void MainLoop(void) {
    s32 threshold;
    u8 *bank_page;
    u32 bank;
    FrameStorage *frame;
    MainLoopStartupHook();
    SetVideoMode(1);
    ResetCallback();
    bank_page=(u8 *)0x800F0000U;
    SetMem(2);
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
            /* ASM_BARRIER: materialize the frame arena in V1 before indexing it. */
            register FrameStorage *frames asm("$3");
            GpuScratchPointers *scratch;
            u32 *ordering;
            frames=DrawBuffers;
            bank=FrameCounter;
            bank &=1;
            frame=&frames[bank];
            ordering=frame->buffer.ordering70.words;
            CurrentDrawBuf=frame;
            ((s32 *)bank_page)[0x4BF0/4]=bank;
            scratch=(GpuScratchPointers *)SCRATCHPAD;
            scratch->order=ordering;
            scratch->cursor=frame->buffer.packets1670;
            ClearOTagR(ordering,length);
        }
        ClearOTagR(CurrentDrawBuf->buffer.secondaryOrderingB70.words,0x2C0);
        CdFileStep();
        SoundUpdate();
        ModeFuncTable[ModeGroup][ModeIndex]();
        DrawSync(0);
        threshold=VSyncThreshold;
        do {} while (VSync(1)<threshold);
        VSync(0);
        PutDispEnv(&CurrentDrawBuf->buffer.environment00.display5C);
        PutDrawEnv(&CurrentDrawBuf->buffer.environment00.draw00);
        DrawOTag(&CurrentDrawBuf->buffer.ordering70.words[703]);
        if(DrawSubOtFlag) DrawOTag(&CurrentDrawBuf->buffer.secondaryOrderingB70.words[703]);
        PadUpdate();
        FrameCounter++;
    }
}
