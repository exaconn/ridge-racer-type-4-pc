#include "rr/library.h"
#include "psyq/libcd/s_1.h"
#include "psyq/libcd/dsmedia2.h"
#include "psyq/libcd/dsstop.h"

extern s32 CdControl(s32 command, const u8 *parameters, u8 *result);
extern void CdDataCallback(LibCompletionCallback);

void CdRead2(s32 flags) {
    u8 buf[8];
    buf[0] = flags;
    CdControl(0xE, buf, 0);
    if (flags & 0x100) {
        if (flags & 0x20) {
            {
                s32 *base;
                /* PAL mapping: base[0x348 / 4] is CdReadModeFlag. */
                base = (s32 *)0x80110000U;
                base[0x348 / 4] = 0;
            }
        } else {
            {
                s32 *base;
                s32 one = 1;
                /* PAL mapping: base[0x348 / 4] is CdReadModeFlag. */
                base = (s32 *)0x80110000U;
                base[0x348 / 4] = one;
            }
        }
        CdDataCallback(data_ready_callback);
        CdReadyCallback(StCdInterrupt2);
    }
    {
        const u8 *zero = 0;
        CdControl(0x1B, zero, 0);
    }
}
