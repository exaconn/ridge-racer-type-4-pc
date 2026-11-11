#include "rr/library.h"
#include "psyq/libcd/bios_1.h"
#include "rr/cd_init.h"

extern s32 DebugPrintStub(const char *, ...);
extern const char D_800118A8[];
int DeliverEvent(int, int);

int CdInitAttempt();
void def_cbsync(s32 selector, u8 *data);
void def_cbready(s32 selector, u8 *data);
void def_cbread(s32 event, s32 argument);

s32 CdInit(void) {
    s32 i=4;
    s32 result;
loop:
    {
    s32 status=CdInitAttempt();
    s32 expected=1;
    if(status==expected) {
        result=1;
        CD_cbsync=def_cbsync;
        CD_cbready=def_cbready;
        CD_cbread=def_cbread;
        {
            u32 *page;
            page=(u32 *)0x800A0000U;
            page[0x63F4/4]=0;
        }
        goto out;
    }
    }
    i--;
    if(i!=-1) goto loop;
    DebugPrintStub(D_800118A8);
    result=0;
out:
    return result;
}

int CdInitAttempt() {
    int r;
    if (CD_init() == 0) { r = CD_initvol() == 0; } else r = 0;
    return r;
}

/* extern */
void def_cbsync(s32 selector, u8 *data) {
    DeliverEvent(0xF0000003, 0x20);
}

/* extern */
void def_cbready(s32 selector, u8 *data) {
    DeliverEvent(0xF0000003, 0x40);
}

/* extern */
void def_cbread(s32 event, s32 argument) {
    DeliverEvent(0xF0000003, 0x40);
}
