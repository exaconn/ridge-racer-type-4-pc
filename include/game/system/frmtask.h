#ifndef RR_GAME_SYSTEM_FRMTASK_H
#define RR_GAME_SYSTEM_FRMTASK_H

#include "types.h"
#include "rr/frame_task_types.h"

enum {
    FRAME_TASK_READ_START = 0,
    FRAME_TASK_READ_WAIT = 1,
    FRAME_TASK_SETUP = 2,
    FRAME_TASK_REVEAL = 7,
    FRAME_TASK_FADE = 8
};
extern s32 FrmTaskState;
extern FrameTaskState *ActiveFrameTaskScript;

void ResetFrmTaskState(void);
void FrmTaskModeEnter(void);
void FrmTaskModeUpdate(void);
void MainLoop(void);

/* SDK startup calls made by MainLoop (PsyZ libetc/libapi). */
long SetVideoMode(long mode);

#endif /* RR_GAME_SYSTEM_FRMTASK_H */
