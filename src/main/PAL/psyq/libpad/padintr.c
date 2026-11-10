/* GCC_VERSION: 2.7.2 */
#include "rr/interrupts.h"
#include "psyq/libpad/padintr.h"
extern int (*PadVsyncHook)();
s32 _padIsVsync(void) {
    InterruptRegisters *r = I_STAT_Pad;
    if (!(r->mask & 1)) return 0;
    if (!(r->pending & 1)) return 0;
    if (PadVsyncHook) PadVsyncHook();
    return 1;
}
