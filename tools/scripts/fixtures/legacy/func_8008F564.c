/* GCC_VERSION: 2.7.2 */
#include "types.h"

extern s32 CdReadyCallback();
extern s32 CdDataCallback();
extern s32 CdAltReadyCallback();
extern s32 CdAltDataCallback();
extern s32 EnterCriticalSection();
extern s32 ExitCriticalSection();
extern s32 D_800A64A8;
extern s8 *D_800A6800;
extern s8 *D_800A680C;

void StUnSetRing(void) {
    EnterCriticalSection();
    if (D_800A64A8 == 1) {
        CdAltDataCallback(0);
        CdAltReadyCallback(0);
    } else {
        CdDataCallback(0);
        CdReadyCallback(0);
    }
    *D_800A6800 = 0;
    {
        s8 *port = D_800A680C;
        /* ASM_BARRIER: preserve the pointer's load delay before the final call. */
        __asm__ volatile("nop" : : "r"(port));
        *port = 0;
    }
    ExitCriticalSection();
}
