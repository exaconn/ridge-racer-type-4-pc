/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libgpu/sys_drain.h"
#include "rr/interrupts.h"

extern int GpuQueueSavedIntrMask;

void _addque(GpuOpFunc op, void *payload, unsigned long parameter);
int _addque2(GpuOpFunc op, void *p0, int len, unsigned long p1);

void _addque(GpuOpFunc op, void *payload, unsigned long parameter) { _addque2(op, payload, 0, parameter); }

int _addque2(GpuOpFunc op, void *p0, int len, unsigned long p1) {
    int i;
    GpuResetState80 *sys;

    set_alarm();
    while (((_qin + 1) & 0x3F) == _qout) {
        if (get_alarm() != 0) {
            return -1;
        }
        _exeque();
    }
    GpuQueueSavedIntrMask = SetIntrMask(0);
    sys = &GpuResetState;
    sys->pending08 = 1;
    if (sys->enabled01 == 0 || (_qin == _qout && (*DMA2_CHCR & 0x01000000) == 0 && sys->completion0C == 0)) {
        while ((*GPU_STATUS & 0x04000000) == 0)
;
        op(p0, p1);
        SetIntrMask(GpuQueueSavedIntrMask);
        return 0;
    }
    DMACallback(2, (LibInterruptCallback)_exeque);
    if (len != 0) {
        for (i = 0; i < len / 4; i++) {
            GpuCommandQueue[_qin].buf[i] = ((int *)p0)[i];
        }
        GpuCommandQueue[_qin].p0 = (int *)GpuCommandQueue[_qin].buf;
    } else {
        GpuCommandQueue[_qin].p0 = p0;
    }
    GpuCommandQueue[_qin].p1 = p1;
    GpuCommandQueue[_qin].op = op;
    _qin = (_qin + 1) & 0x3F;
    SetIntrMask(GpuQueueSavedIntrMask);
    _exeque();
    return (_qin - _qout) & 0x3F;
}
