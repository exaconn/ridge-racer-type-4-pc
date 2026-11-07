/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

int DeliverEvent(int, int);

/* extern */
void func_8008BF60(s32 event, s32 argument) {
    DeliverEvent(0xF0000003, 0x40);
}
