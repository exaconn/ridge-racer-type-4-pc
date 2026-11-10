/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/gpu_reset.h"

s32 ResetGraph(s32 mode)
{
    GpuResetState80 *state;
    switch(mode & 7) {
    case 0:
    case 3:
        DebugPrintStub(ResetGraphFmt,&GpuDriverTable,&GpuResetState);
    case 5:
        state=&GpuResetState;
        GPU_memset((s8 *)state,0,128);
        ResetCallback();
        GPU_cw((unsigned long)GpuFuncTable & 0xFFFFFF);
        state->kind00=_reset(mode);
        state->enabled01=1;
        /* Preserve the explicit byte reload after the reset service. */
        state->debug02.width=D_800ABB40[*(volatile u8 *)&state->kind00].value;
        state->debug02.height=D_800ABB4C.dimensions[state->kind00].value;
        GPU_memset((s8 *)state->queue10,-1,0x5C);
        GPU_memset((s8 *)state->tail6C,-1,0x14);
        return state->kind00;
    default:
        if(D_800ABAC2.level>=2) GPU_printf(StrResetGraphFmt,mode);
        return GpuFuncTable->reset34(1);
    }
}
