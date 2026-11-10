/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/cd.h"
#include "rr/cd_poll.h"
#include "psyq/libcd/bios_1.h"
#include "psyq/libcd/s_1.h"
#include "game/data/globals.h"

extern u8 CD_mode;
extern int CD_init();
extern char CD_str_none[];
extern s32 CD_cbsync;
extern s32 CD_cbready;

u8 CdLastCom(void);
void CdFlush();
char *CdComstr(unsigned char a0);
char *CdIntstr(unsigned char a0);
s32 CdSync(s32 mode,u8 *status);
s32 CdReady(s32 mode, u8 *status);

u8 CdStatus(void) {
    return CD_status.bytes.status;
}

s32 CdMode(void) {
    return CD_mode;
}

u8 CdLastCom(void) {
    return CD_com;
}

u8 *CdLastPos(void) {
    return CD_pos;
}

int CdReset(a0)
    int a0;
{
    if (a0 == 2) {
        CD_initintr();
        return 1;
    }
    if (CD_init() != 0) return 0;
    if (a0 == 1) {
        int r = CD_initvol();
        if (r != 0) return 0;
    }
    return 1;
}

void CdFlush() { CD_flush(); }

s32 CdSetDebug(s32 value) {
    s32 previous = CD_debug;
    s32 *base;
    /* PAL RAM mapping: CD_debug is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    base[(0x6490) / 4] = value;
    return previous;
}

char *CdComstr(unsigned char a0) { char *r; if (a0 < 0x1C) r = (char *)CD_comstr[a0]; else r = CD_str_none; return r; }

char *CdIntstr(unsigned char a0) { char *r; if (a0 < 7) r = (char *)CD_intstr[a0]; else r = CD_str_none; return r; }

s32 CdSync(s32 mode,u8 *status) {
    return CD_sync(mode,status);
}

s32 CdReady(s32 mode, u8 *status) {
    return CD_ready(mode, status);
}

s32 CdSyncCallback(s32 value) {
    s32 previous = CD_cbsync;
    s32 *base;
    /* PAL RAM mapping: CD_cbsync is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    base[(0x6488) / 4] = value;
    return previous;
}

s32 CdReadyCallback(s32 value) {
    s32 previous = CD_cbready;
    s32 *base;
    /* PAL RAM mapping: CD_cbready is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    base[(0x648C) / 4] = value;
    return previous;
}
