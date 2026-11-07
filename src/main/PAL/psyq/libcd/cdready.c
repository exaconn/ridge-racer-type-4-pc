/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "psyq/libcd/bios_1.h"
#include "rr/cd_init.h"

extern s32 func_8001E84C(const char *, ...);
int DeliverEvent(int, int);

int func_8008BED4();
void def_cbsync(s32 selector, u8 *data);
void def_cbready(s32 selector, u8 *data);
void def_cbread(s32 event, s32 argument);

s32 CdInit(void) {
    s32 i=4;
    /* ASM_BARRIER: Keep the success value in the common return register. */
    register s32 result __asm__("$2");
loop:
    {
    s32 status=func_8008BED4();
    s32 expected=1;
    /* ASM_BARRIER: Keep the probe result comparison separate from returned success. */
    __asm__("" : "+r"(expected));
    if(status==expected) {
        result=1;
        CD_cbsync=def_cbsync;
        CD_cbready=def_cbready;
        CD_cbread=def_cbread;
        {
            /* ASM_BARRIER: Preserve the assembler temporary for the final state store. */
            register u32 *page __asm__("$1");
            /* ASM_BARRIER: Publish callback pointers before the completion state. */
            __asm__("" : : : "$1", "memory");
            page=(u32 *)0x800A0000U;
            /* ASM_BARRIER: Expose the success cleanup store in the jump delay slot. */
            __asm__("" : "+r"(page));
            page[0x63F4/4]=0;
        }
        goto out;
    }
    }
    /* ASM_BARRIER: Keep the retry decrement on the unsuccessful probe path. */
    __asm__ volatile("" : : : "memory");
    i--;
    if(i!=-1) goto loop;
    {
        const char *message;
        /* ASM_BARRIER: Load the error message only after all retry attempts fail. */
        __asm__("" : : : "$4");
        message=(const char *)0x80010000U;
        /* ASM_BARRIER: Expose the message offset for the print-call delay slot. */
        __asm__("" : "+r"(message));
        func_8001E84C(message+0x18A8);
    }
    result=0;
out:
    return result;
}

int func_8008BED4() {
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
