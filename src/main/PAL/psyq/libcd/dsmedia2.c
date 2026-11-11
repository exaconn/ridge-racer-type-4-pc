/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr_asm.h"
#include "rr/library.h"
#include "psyq/libcd/s_4.h"
#include "psyq/libcd/dsstrm4.h"
#include "psyq/libcd/s_1.h"
#include "psyq/libcd/dsmedia2.h"
#include "psyq/libcd/dsstrm32.h"
#include "rr/cd.h"
#include "rr/overlay668_stream.h"
#include "rr/audio_cd_controller.h"

extern s32 CdDataCallback(void (*func)(void));
extern s32 EnterCriticalSection(void);
extern s32 ExitCriticalSection(void);
extern s32 D_800A64A8;
extern s8 *D_800A6800;
extern s8 *D_800A680C;
extern s32 D_80112754;
extern LibPackedWord D_80112750;
extern void (*StDataReadyFunc)(void);

void StUnSetRing(void);
int StGetBackloc(int a);

void StClearRing(void) {
    int a = StRingSize;
    StRingReadIndex = 0;
    StRingFrameStartIndex = 0;
    StRingWriteIndex = 0;
    StStreamStarted = 0;
    init_ring_status(0, a);
    CdMediaChangeCount = 0;
    StCurSectorOffset = 0;
    StCurFrameNo = 0;
}

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
        RR_NOP_DEP_ON(port);
        *port = 0;
    }
    ExitCriticalSection();
}

void data_ready_callback(void)
{
    s32 value;
    s32 next;
    void (*callback)(void);
    LibQueueRecord32 *p = StRingHeaders + StRingFrameStartIndex;
    p->state = 2;
    D_80112750 = p->word1C;
    value = p->value08;
    next = StRingWriteIndex;
    callback = StDataReadyFunc;
    {
        u8 *base;
        /* ASM_BARRIER: Read value, next index, and callback before publishing state. */
        __asm__ volatile("" : : "r"(value), "r"(next), "r"(callback) : "$1", "memory");
        D_80112754 = value;
        /* PAL mapping: base + 0x2F8 is &StRingFrameStartIndex. */
        base = (u8 *)0x80110000U;
        ((s32 *)base)[0x2F8 / sizeof(s32)] = next;
    }
    if (callback != 0) callback();
    StStreamStarted = 0;
}

int StGetBackloc(int a) {
    int r;
    if (CdReadModeFlag == 0) {
        CdIntToPos(CdPosToInt(&D_80112750) + 1, a);
        r = D_80112754;
    } else {
        r = -1;
    }
    return r;
}
