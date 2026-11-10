/* CC1_FLAGS: -mno-split-addresses */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libgpu/sys_drain.h"
#include "rr/interrupts.h"

extern int GpuSavedIntrMask;

int _exeque(void) {
    int dmaBusyMask, commandReadyMask;
    if (*DMA2_CHCR & 0x01000000) {
        return 1;
    }
    GpuSavedIntrMask = SetIntrMask(0);
    while (_qin != _qout && (dmaBusyMask = 0x01000000, (*DMA2_CHCR & dmaBusyMask) == 0)) {
        if (((_qout + 1) & 0x3F) == _qin && GpuResetState.completion0C == 0) {
            DMACallback(2, 0);
        }
        while ((commandReadyMask = 0x04000000, (*GPU_STATUS & commandReadyMask) == 0)) {
        }
        GpuCommandQueue[_qout].op(GpuCommandQueue[_qout].p0, GpuCommandQueue[_qout].p1);
        _qout = (_qout + 1) & 0x3F;
    }
    SetIntrMask(GpuSavedIntrMask);
    if (_qin == _qout && (*DMA2_CHCR & 0x01000000) == 0) {
        GpuCompletionCallback completion;
        if (GpuResetState.pending08 != 0 && (completion = GpuResetState.completion0C) != 0) {
            GpuResetState80 *state = &GpuResetState;
            /* Publish completion before user code can rearm the queue. */
            *(volatile u32 *)&state->pending08 = 0;
            completion();
        }
    }
    return (_qin - _qout) & 0x3F;
}
