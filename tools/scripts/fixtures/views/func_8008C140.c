/* CC1_FLAGS: -mno-split-addresses */

#include "rr/cd.h"

/* Contiguous CD control variants share command metadata and callback state.
 * This grouping does not establish the original compiler unit boundary. */
#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define CD_getsector2 ((int (*)())CD_getsector2)

#undef CD_getsector2

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
    /* ASM_BARRIER: Retain the byte command separately from its full input. */
    register s32 code __asm__("$19")=command&255;
    u32 *table;
    RrLibDataCallback saved;
    u32 *metadata;
    u32 offset;
    /* ASM_BARRIER: Retain the independent success/failure state. */
    register s32 state __asm__("$23");
    s32 terminal;
    table=D_800A6400;
    saved=CD_cbsync;
    /* ASM_BARRIER: Capture the saved callback before computing the selected metadata entry. */
    __asm__("" : "+r"(saved) : "r"(table) : "$2");
    offset=code*sizeof(*table);
    metadata=(u32 *)((unsigned long)offset+(unsigned long)table);
    /* ASM_BARRIER: Retain the success state independently of the return register. */
    __asm__("" : "=r"(state) : "0"(0));
    terminal=-1;
loop:
    CD_cbsync = 0;
    {
    /* ASM_BARRIER: Keep the command-1 comparison in the original temporary. */
    register s32 one __asm__("$8")=1;
    /* ASM_BARRIER: Retain the command-1 comparison value. */
    __asm__("" : "+r"(one));
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
    if (CD_cw(command & 0xFF, parameters, result, 0) == 0) goto done;
retry:
    remaining--;
    if (remaining != terminal) goto loop;
    CD_cbsync = saved;
    state = -1;
done:
    return state + 1;
    }
}
