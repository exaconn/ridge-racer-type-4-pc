/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/cd.h"

/* Contiguous CD control variants share command metadata and callback state.
 * This grouping does not establish the original compiler unit boundary. */
#include "rr/library.h"
#include "psyq/libcd/bios_1.h"
#include "psyq/libcd/s_2.h"

s32 CdControl(s32 requested_command,const u8 *parameter_buffer,u8 *result_buffer);
s32 CdControlF(s32 requested_command,const u8 *parameter_buffer);
s32 CdControlB(s32 requested_command,const u8 *parameter_buffer,u8 *result_buffer);
s32 CdGetSector(u8 *buffer, u32 words);
s32 CdGetSector2(u8 *buffer, u32 words);


s32 CdControl(s32 requested_command,const u8 *parameter_buffer,u8 *result_buffer) {
    /* ASM_BARRIER: Retain the original full command across device calls. */
    register s32 command __asm__("$20");
    const u8 *parameters=parameter_buffer;
    u8 *result=result_buffer;
    /* ASM_BARRIER: Capture both pointer arguments before reusing the full command register. */
    __asm__("" : "+r"(parameters), "+r"(result) : : "$20");
    command=requested_command;
    {
        /* ASM_BARRIER: Retain the original retry counter. */
        register s32 remaining __asm__("$16")=3;
        s32 code=command&255;
        u32 *table=CD_com_info;
        LibDataCallback saved=CD_cbsync;
        u32 *metadata=code+table;
        s32 state=0;
        s32 terminal=-1;
        do {
            CD_cbsync = 0;
            {
                /* ASM_BARRIER: Keep the command-1 comparison in the original temporary. */
                register s32 one __asm__("$8")=1;
                if (code != one) {
                    if (CD_status.bytes.status & 0x10) {
                        s32 status_command=1;
                        const u8 *parameter_zero;
                        u8 *result_zero;
                        /* ASM_BARRIER: Materialize each zero independently before the status call. */
                        __asm__("" : "=r"(parameter_zero) : "0"(0));
                        /* ASM_BARRIER: Materialize each zero independently before the status call. */
                        __asm__("" : "=r"(result_zero) : "0"(0));
                        CD_cw(status_command, parameter_zero, result_zero, 0);
                    }
                }
            }
            if (parameters != 0) {
                if (*metadata != 0) {
                    if (CD_cw(2, parameters, result, 0) != 0) continue;
                }
            }
            CD_cbsync = saved;
            if (CD_cw(command & 0xFF, parameters, result, 0) == 0) goto done;
        } while (--remaining != terminal);
        CD_cbsync = saved;
        state = -1;
done:
        return state + 1;
    }
}

s32 CdControlF(s32 requested_command,const u8 *parameter_buffer) {
    /* ASM_BARRIER: Retain the original full command across device calls. */
    register s32 command __asm__("$19");
    const u8 *parameters=parameter_buffer;
    /* ASM_BARRIER: Capture the parameter pointer before reusing the full command register. */
    __asm__("" : "+r"(parameters) : : "$19");
    command=requested_command;
    {
        /* ASM_BARRIER: Retain the original retry counter. */
        register s32 remaining __asm__("$16")=3;
        s32 one=1;
        s32 code=command&255;
        u32 *table=CD_com_info;
        LibDataCallback saved=CD_cbsync;
        u32 *metadata=code+table;
        s32 state=0;
        s32 terminal=-1;
        do {
            {
                LibDataCallback *page;
                page=(LibDataCallback *)0x800A0000U;
                page[0x6488/sizeof(*page)]=0;
            }
            if (code != one) {
                if (CD_status.bytes.status & 0x10) {
                    s32 status_command=1;
                    const u8 *parameter_zero;
                    u8 *result_zero;
                    /* ASM_BARRIER: Materialize each zero independently before the status call. */
                    __asm__("" : "=r"(parameter_zero) : "0"(0));
                    /* ASM_BARRIER: Materialize each zero independently before the status call. */
                    __asm__("" : "=r"(result_zero) : "0"(0));
                    CD_cw(status_command, parameter_zero, result_zero, 0);
                }
            }
            if (parameters != 0) {
                if (*metadata != 0) {
                    u8 *result_zero;
                    /* ASM_BARRIER: Prepare the null result separately from mode zero. */
                    __asm__("" : "=r"(result_zero) : "0"(0));
                    if (CD_cw(2, parameters, result_zero, 0) != 0) continue;
                }
            }
            CD_cbsync = saved;
            if (CD_cw(command & 0xFF, parameters, 0, 1) == 0) goto done;
        } while (--remaining != terminal);
        CD_cbsync = saved;
        state = -1;
done:
        return state + 1;
    }
}

s32 CdControlB(s32 requested_command,const u8 *parameter_buffer,u8 *result_buffer) {
    /* ASM_BARRIER: Retain the original full command across device calls. */
    register s32 command __asm__("$20");
    const u8 *parameters=parameter_buffer;
    u8 *result=result_buffer;
    /* ASM_BARRIER: Capture both pointer arguments before reusing the full command register. */
    __asm__("" : "+r"(parameters), "+r"(result) : : "$20");
    command=requested_command;
    {
    /* ASM_BARRIER: Retain the original retry counter. */
    register s32 remaining __asm__("$16")=3;
    s32 code;
    u32 *table;
    LibDataCallback saved;
    u32 *metadata;
    s32 outcome;
    s32 terminal;
    s32 one=1;
    code=command&255;
    table=CD_com_info;
    saved=CD_cbsync;
    metadata=code+table;
    terminal=-1;
loop:
    {
        LibDataCallback *page;
        page=(LibDataCallback *)0x800A0000U;
        page[0x6488/sizeof(*page)]=0;
    }
    {
    if (code != one) {
        if (CD_status.bytes.status & 0x10) {
            s32 status_command=1;
            const u8 *parameter_zero;
            u8 *result_zero;
            /* ASM_BARRIER: Materialize each zero independently before the status call. */
            __asm__("" : "=r"(parameter_zero) : "0"(0));
            /* ASM_BARRIER: Materialize each zero independently before the status call. */
            __asm__("" : "=r"(result_zero) : "0"(0));
            CD_cw(status_command, parameter_zero, result_zero, 0);
        }
    }
    }
    if (parameters != 0) {
        if (*metadata != 0) {
            if (CD_cw(2, parameters, result, 0) != 0) goto retry;
        }
    }
    CD_cbsync = saved;
    if (CD_cw(command & 0xFF, parameters, result, 0) != 0) {
retry:
        remaining--;
        outcome=-1;
        if(remaining!=terminal) goto loop;
        CD_cbsync=saved;
    } else outcome=0;
    if(outcome==0) {
        outcome=CD_sync(0,result)==2;
    } else {
        outcome=0;
    }
    return outcome;
    }
}

s32 CdMix(void) {
    CD_vol();
    return 1;
}

s32 CdGetSector(u8 *buffer, u32 words) {
    return CD_getsector(buffer, words) == 0;
}

s32 CdGetSector2(u8 *buffer, u32 words) {
    return CD_getsector2(buffer, words) == 0;
}
