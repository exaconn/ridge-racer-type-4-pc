/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/library.h"
#include "psyq/libgpu/sys_drain.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/gpu.h"
#include "rr/gpu_reset.h"

int DrawOTag2();
void GPU_memset(s8 *arg0, s8 arg1, s32 arg2);

int DrawOTag2(a0) int a0; {
    if (D_800ABAC2.level >= 2) GPU_printf(D_80011F38, a0);
    GpuTimeoutDeadline = VSync(-1) + 0xF0;
    GpuAlarmCount = 0;
    while ((*DMA2_CHCR & 0x1000000) || (*GPU_STATUS & 0x4000000) == 0) {
        if (get_alarm() != 0) return -1;
    }
    DMACallback(2, GPU_DmaDoneRequeue);
    GpuFuncTable->draw_ordering_table(a0);
    return 0;
}

void GPU_DmaDoneRequeue(void) {
    DMACallback(2,_exeque);
}

void GPU_memset(s8 *arg0, s8 arg1, s32 arg2) {
    s32 var_v0;
    s8 *var_a0;

    var_a0 = arg0;
    var_v0 = arg2 - 1;
    if (arg2 != 0) {
        do {
            *var_a0 = arg1;
            var_v0 -= 1;
            var_a0 += 1;
        } while (var_v0 != -1);
    }
}
