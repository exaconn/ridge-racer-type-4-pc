/* GCC_VERSION: 2.7.2 */
#include "psyq/libpad/padintr.h"
struct R { int a, b; };
extern struct R *I_STAT_Pad;
extern int (*PadVsyncHook)();
int _padIsVsync() {
    struct R *r = I_STAT_Pad;
    if (!(r->b & 1)) return 0;
    if (!(r->a & 1)) return 0;
    if (PadVsyncHook) PadVsyncHook();
    return 1;
}
