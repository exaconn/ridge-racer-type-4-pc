/* GCC_VERSION: 2.7.2 */
#include "psyq/libpad/padintr.h"
struct R { int a, b; };
extern struct R *I_STAT_Pad;
extern int (*D_800ABC90)();
int _padIsVsync() {
    struct R *r = I_STAT_Pad;
    if (!(r->b & 1)) return 0;
    if (!(r->a & 1)) return 0;
    if (D_800ABC90) D_800ABC90();
    return 1;
}
