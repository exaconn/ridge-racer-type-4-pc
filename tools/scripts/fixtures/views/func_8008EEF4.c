#include "rr/cd.h"

#include "rr/library.h"

#include "rr/cd_read_request.h"

#define RR_READ_OWNER_OFFSET \
    (sizeof(RrLibReadRequestState)-sizeof(RrLibReadRestoreTail))

#define RR_PAIR_OWNER_OFFSET (RR_READ_OWNER_OFFSET+sizeof(RrLibCompletionCallback))

#define RR_PAIR_TAIL(pair) \
    ((volatile RrLibReadRestoreTail*)((u8*)(pair)-sizeof(RrLibCompletionCallback)))

#define RR_PAIR_REQUEST(pair) \
    ((volatile RrLibReadRequestState*)((u8*)(pair)-RR_PAIR_OWNER_OFFSET))

#define RR_PAIR_PENDING(pair) (RR_PAIR_REQUEST(pair)->finished24)

/* legacy typing of shared globals in this part of the unit */
#define D_800A67B0 (*(RrLibTransferState38 (*))&D_800A67B0)

#define func_8008C12C ((void (*)(RrLibCompletionCallback))func_8008C12C)

#define func_8008C55C ((void (*)(RrLibCompletionCallback))func_8008C55C)

#define func_8008C118 ((void (*)(RrLibCompletionCallback))func_8008C118)

#define func_8008E7F4 ((void (*)(void))func_8008E7F4)

#undef D_800A67B0

#undef func_8008C12C

#undef func_8008C55C

#undef func_8008C118

#undef func_8008E7F4

/* legacy typing of shared globals in this part of the unit */
#define func_8008E7F4 ((void (*)(void))func_8008E7F4)

#undef func_8008E7F4

/* legacy typing of shared globals in this part of the unit */
#define func_8008C3B0 ((s32 (*)(s32,const u8*,u8*))func_8008C3B0)

#undef func_8008C3B0

extern s32 func_8008B154(s32);

extern RrLibCompletionCallback func_8008C118(RrLibCompletionCallback);

extern RrLibCompletionCallback func_8008C12C(RrLibCompletionCallback);

extern RrLibCompletionCallback func_8008C55C(RrLibCompletionCallback);

extern u8 func_8008BF94(void);

s32 func_8008EEF4(s32 sectors,u8 *buffer,u32 mode) {
    s32 count=sectors;
    u8 *destination=buffer;
    /* ASM_BARRIER: preserve request mode across pending-completion helpers. */
    register u32 format __asm__("$18");
    u32 start;
    /* ASM_BARRIER: retain the complete request address across callback setters. */
    register RrLibReadRequestState *state __asm__("$16");
    volatile s32 *pending;
    /* ASM_BARRIER: use the original constructor argument register for case stores. */
    register RrLibReadRequestState *control __asm__("$4");
    u32 mode_stage=mode;
    s32 command,kind;
    RrLibCompletionCallback *restore_slot;
    RrLibCompletionCallback restored;
    u32 default_step;
    RrLibCompletionCallback zero;
    s32 result,busy;
    /* ASM_BARRIER: materialize the complete fallback address in its temporary. */
    register RrLibReadRequestState *fallback __asm__("$3");
    const u8 *parameters;
    pending=&D_800A67D4;
    /* ASM_BARRIER: capture the pending flag's complete address before polling. */
    __asm__("" : "+r"(pending));
    busy=*pending;
    format=mode_stage;
    if(busy!=0) {
        start=func_8008B154(-1);
        if(*pending!=0) {
wait:
            if((u32)func_8008B154(-1)-start>=121U)goto timeout;
            if(*pending!=0)goto wait;
        }
    }
setup:
    /* ASM_BARRIER: keep the timeout jump aimed at the complete constructor entry. */
    __asm__("" : );
    control=&D_800A67B0;
    control->mode0C=format;
    kind=control->mode0C&0x30;
    switch(kind) {
/* The original mode-dispatch block also holds timeout recovery, which returns
 * to setup before selecting the transfer step. */
timeout:
        restore_slot=&D_800A67D8;
        /* ASM_BARRIER: capture the saved-callback slot before deriving its owner. */
        __asm__("" : "+r"(restore_slot));
        restored=*restore_slot;
        state=(RrLibReadRequestState*)((u8*)restore_slot-
              (sizeof(RrLibReadRequestState)-sizeof(RrLibReadRestoreTail)));
        func_8008C118(restored);
        ((volatile RrLibReadRequestState*)state)->finished24=0;
        goto setup;
    case 0:
        control->step10=0x200;
        break;
    case 32:
        control->step10=0x249;
        break;
    default:
        default_step=0x246;
        /* ASM_BARRIER: stage the fallback value before its address temporary. */
        __asm__("" : "+r"(default_step) : : "$3");
        fallback=&D_800A67B0;
        /* ASM_BARRIER: retain the complete fallback address for its field store. */
        __asm__("" : "+r"(fallback));
        fallback->step10=default_step;
        break;
    }
    state=&D_800A67B0;
    zero=0;
    state->mode0C|=0x20;
    /* ASM_BARRIER: publish mode before the request buffer and sector count. */
    __asm__("" : "+r"(state), "+r"(zero) : : "memory");
    state->buffer04=destination;
    state->sectors00=count;
    state->restore.saved28=func_8008C118(zero);
    state->restore.saved2C=func_8008C12C(0);
    if(D_800A63F4&1)state->restore.saved30=func_8008C55C(0);
    state->start1C=func_8008B154(-1);
    if(func_8008BF94()&0xE0) {
        command=9;
        parameters=0;
        /* ASM_BARRIER: form the command and parameter arguments independently. */
        __asm__("" : "+r"(parameters) : "r"(command));
        func_8008C3B0(command,parameters,0);
    }
    result=func_8008EB6C(0)>0;
    /* ASM_BARRIER: finish the Boolean result before restoring saved temporaries. */
    __asm__("" : "+r"(result) : : "$17", "$19");
    return result;
}
