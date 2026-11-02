#include "rr/geometry.h"
#include "rr_asm.h"
RrVec3i *func_80091574(const RrVec3i *input,RrVec3i *output) {
    RR_GTE_LWC2_IR(input);
    /* ASM_BARRIER: Leave the original GTE input-load pipeline gap before SQR. */
    RR_NOP_DEP();
    RR_GTE_SQR12();
    RR_GTE_SWC2_MAC123(output);
    /* ASM_BARRIER: Finish hardware stores before forming the returned pointer. */
    __asm__("" : : "r"(output) : "$2");
    return output;
}
RrVec3i *func_8009159C(const RrVec3i *input,RrVec3i *output) {
    RR_GTE_LWC2_IR(input);
    /* ASM_BARRIER: Leave the original GTE input-load pipeline gap before SQR. */
    RR_NOP_DEP();
    RR_GTE_SQR0();
    RR_GTE_SWC2_MAC123(output);
    /* ASM_BARRIER: Finish hardware stores before forming the returned pointer. */
    __asm__("" : : "r"(output) : "$2");
    return output;
}
