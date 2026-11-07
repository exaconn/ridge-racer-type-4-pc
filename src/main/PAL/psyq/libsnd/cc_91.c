/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
#include "psyq/libsnd/cc_91.h"
void _SsSetNrpnVabAttr16(int a0, int a1, int a2, int a3) { unsigned char v = *((unsigned char *)&a3 + 0x24); SsUtSetReverbDepth(v, v); }
