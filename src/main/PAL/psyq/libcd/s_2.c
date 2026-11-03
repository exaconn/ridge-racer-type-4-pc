/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr/cd.h"
#include "m2c_macros.h"

/* Contiguous CD control variants share command metadata and callback state.
 * This grouping does not establish the original compiler unit boundary. */
#include "rr/library.h"
#include "psyq/libcd/bios_1.h"

s32 CdControl(s32 requested_command,const u8 *parameter_buffer,u8 *result_buffer);
s32 CdControlF(s32 requested_command,const u8 *parameter_buffer);
s32 CdControlB(s32 requested_command,const u8 *parameter_buffer,u8 *result_buffer);
s32 func_8008C4FC(void);
s32 func_8008C51C(u8 *buffer, u32 words);
s32 func_8008C53C(void);

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
    saved=D_800A6488;
    /* ASM_BARRIER: Capture the saved callback before computing the selected metadata entry. */
    __asm__("" : "+r"(saved) : "r"(table) : "$2");
    offset=code*sizeof(*table);
    metadata=(u32 *)((unsigned long)offset+(unsigned long)table);
    /* ASM_BARRIER: Retain the success state independently of the return register. */
    __asm__("" : "=r"(state) : "0"(0));
    terminal=-1;
loop:
    D_800A6488 = 0;
    {
    /* ASM_BARRIER: Keep the command-1 comparison in the original temporary. */
    register s32 one __asm__("$8")=1;
    /* ASM_BARRIER: Retain the command-1 comparison value. */
    __asm__("" : "+r"(one));
    if (code != one) {
        if (D_800A6494.bytes.status & 0x10) {
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
    D_800A6488 = saved;
    if (CD_cw(command & 0xFF, parameters, result, 0) == 0) goto done;
retry:
    remaining--;
    if (remaining != terminal) goto loop;
    D_800A6488 = saved;
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
    s32 one;
    /* ASM_BARRIER: Retain the byte command separately from its full input. */
    register s32 code __asm__("$18");
    /* ASM_BARRIER: Retain the command metadata base before indexing it. */
    register u32 *table __asm__("$3");
    RrLibDataCallback saved;
    /* ASM_BARRIER: Retain the selected metadata entry across all raw device calls. */
    register u32 *metadata __asm__("$21");
    /* ASM_BARRIER: Form the metadata byte offset before preserving its selected pointer. */
    register u32 offset __asm__("$2");
    s32 state;
    s32 terminal;
    one=1;
    code=command&255;
    table=D_800A6400;
    saved=D_800A6488;
    /* ASM_BARRIER: Capture the saved callback before computing the selected metadata entry. */
    __asm__("" : "+r"(saved) : "r"(table) : "$2");
    offset=code*sizeof(*table);
    metadata=(u32 *)((unsigned long)offset+(unsigned long)table);
    /* ASM_BARRIER: Retain the success state independently of the return register. */
    __asm__("" : "=r"(state) : "0"(0));
    terminal=-1;
loop:
    {
        /* ASM_BARRIER: Use the original address temporary for callback clearing. */
        register RrLibDataCallback *page __asm__("$1");
        /* ASM_BARRIER: Finish retry setup before materializing the callback page. */
        __asm__("" : : : "$1");
        page=(RrLibDataCallback *)0x800A0000U;
        /* ASM_BARRIER: Expose callback clearing for the command-test delay slot. */
        __asm__("" : "+r"(page));
        page[0x6488/sizeof(*page)]=0;
    }
    {
    if (code != one) {
        if (D_800A6494.bytes.status & 0x10) {
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
            {
                u8 *result_zero;
                /* ASM_BARRIER: Prepare the null result separately from mode zero. */
                __asm__("" : "=r"(result_zero) : "0"(0));
                if (CD_cw(2, parameters, result_zero, 0) != 0) goto retry;
            }
        }
    }
    D_800A6488 = saved;
    if (CD_cw(command & 0xFF, parameters, 0, 1) == 0) goto done;
retry:
    remaining--;
    if (remaining != terminal) goto loop;
    D_800A6488 = saved;
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
    RrLibDataCallback saved;
    u32 *metadata;
    u32 offset;
    s32 outcome;
    s32 terminal;
    s32 one=1;
    code=command&255;
    table=D_800A6400;
    saved=D_800A6488;
    offset=code*sizeof(*table);
    metadata=(u32 *)((unsigned long)offset+(unsigned long)table);
    terminal=-1;
loop:
    {
        /* ASM_BARRIER: Retain the original address temporary for callback clearing. */
        register RrLibDataCallback *page __asm__("$1");
        page=(RrLibDataCallback *)0x800A0000U;
        /* ASM_BARRIER: Expose callback clearing in the command-test delay slot. */
        __asm__("" : "+r"(page));
        page[0x6488/sizeof(*page)]=0;
    }
    {
    if (code != one) {
        if (D_800A6494.bytes.status & 0x10) {
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
    D_800A6488 = saved;
    if (CD_cw(command & 0xFF, parameters, result, 0) != 0) {
retry:
        remaining--;
        outcome=-1;
        if(remaining!=terminal) goto loop;
        D_800A6488=saved;
    } else outcome=0;
    if(outcome==0) {
        outcome=CD_sync(0,result)==2;
    } else {
        outcome=0;
    }
    return outcome;
    }
}

s32 func_8008C4FC(void) {
    func_8008D5D4();
    return 1;
}

s32 func_8008C51C(u8 *buffer, u32 words) {
    return CD_getsector(buffer, words) == 0;
}

/* legacy typing of shared globals in this part of the unit */
#define CD_getsector2 ((int (*)())CD_getsector2)

s32 func_8008C53C(void) {
    return CD_getsector2() == 0;
}

#undef CD_getsector2
