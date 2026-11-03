#include "rr/cd_read_request.h"
#include "rr/cd.h"
#include "rr/cd_read_stop.h"

#define RR_READ_OWNER_OFFSET \
    (sizeof(RrLibReadRequestState)-sizeof(RrLibReadRestoreTail))
#define RR_PAIR_OWNER_OFFSET (RR_READ_OWNER_OFFSET+sizeof(RrLibCompletionCallback))
#define RR_PAIR_TAIL(pair) \
    ((volatile RrLibReadRestoreTail*)((u8*)(pair)-sizeof(RrLibCompletionCallback)))
#define RR_PAIR_REQUEST(pair) \
    ((volatile RrLibReadRequestState*)((u8*)(pair)-RR_PAIR_OWNER_OFFSET))
#define RR_PAIR_PENDING(pair) (RR_PAIR_REQUEST(pair)->finished24)

extern s32 func_8008B154(s32);
extern u8 func_8008BF94(void);
extern s32 func_8008BFA4(void);
extern u8 *func_8008BFC4(void);
extern void func_8008C03C(void);
extern RrLibCompletionCallback func_8008C118(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C12C(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C55C(RrLibCompletionCallback);
extern s32 func_8008C6A4(void *);
extern s32 func_800963DC(const char *, ...);
extern const char D_80011D40[], D_80011D58[];
extern volatile u32 D_800A67BC;
extern void func_8008E828(s32, u8 *);
extern void func_8008EA9C(void);
extern RrLibCompletionCallback volatile D_800A67DC;
extern void func_8008C580(s32);
extern void func_8008E7F4(void);
extern s32 func_8008C3B0(s32,const u8*,u8*);

s32 func_8008EB6C(s32 restart);
void func_8008ED54(void);
s32 func_8008EEF4(s32 sectors,u8 *buffer,u32 mode);

s32 func_8008EB6C(s32 restart) {
    u8 mode_byte;
    register u32 requested;
    register RrLibReadRequestState *state;
    /* ASM_BARRIER: keep the captured mode address in s1 and preserve the raw
     * restart argument in s2 through the command and status helpers. */
    register volatile u32 *mode_slot __asm__("$17");
    register s32 retry __asm__("$18") = restart;
    register volatile RrLibReadRequestState *shell_state;
    register volatile RrLibReadRequestState *failed_state;
    u32 timestamp;
    s32 completed;
    register RrLibCompletionCallback callback;
    /* ASM_BARRIER: a0/a1 stage independent command/null-parameter values;
     * neither bound value is used across a call that can overwrite it. */
    register s32 command __asm__("$4");
    register const u8 *parameters __asm__("$5");

    func_8008C118(0);
    func_8008C12C(0);
    if (D_800A63F4 & 1) {
        func_8008C55C(0);
    }
    if (func_8008BF94() & 0x10) {
        if (!(func_8008B154(-1) & 63)) {
            func_800963DC(D_80011D40);
        }
        func_8008C27C(1, 0);
        timestamp = func_8008B154(-1);
        shell_state = &D_800A67B0;
        shell_state->start1C = timestamp;
        shell_state->remaining14 = -1;
        return shell_state->remaining14;
    }
    if (retry != 0) {
        /* ASM_BARRIER: keep retry-only diagnostic address formation in this
         * branch instead of copying it into its predecessor's delay slot. */
        __asm__("" : );
        func_800963DC(D_80011D58);
        command = 9;
        parameters = 0;
        /* ASM_BARRIER: stage the null parameter after the command value. */
        __asm__("" : "+r"(parameters) : "r"(command));
        func_8008C140(command, parameters, 0);
        if (!func_8008C140(2, func_8008BFC4(), 0)) {
            goto failed;
        }
    }
    func_8008C03C();
    /* ASM_BARRIER: form the captured mode address after reset returns. */
    __asm__("" : : : "$17");
    mode_slot = &D_800A67BC;
    /* ASM_BARRIER: preserve the complete address before the mode load. */
    __asm__("" : "+r"(mode_slot));
    requested = *mode_slot;
    mode_byte = requested;
    requested &= 255;
    if (requested != func_8008BFA4() || retry != 0) {
        if (!func_8008C140(14, &mode_byte, 0)) {
failed:
            /* ASM_BARRIER: retain one shared failure block with fresh state
             * publication and reload, rather than duplicating address setup. */
            __asm__("" : );
            failed_state = &D_800A67B0;
            failed_state->remaining14 = -1;
            return failed_state->remaining14;
        }
    }
    completed = func_8008C6A4(func_8008BFC4());
    /* ASM_BARRIER: the success-state address is formed after decoding. */
    __asm__("" : : : "$16");
    callback = (RrLibCompletionCallback)func_8008E828;
    state = &D_800A67B0;
    /* ASM_BARRIER: stage the callback address before the state address, then
     * publish the decoded sector in the callback setter's delay slot. */
    __asm__("" : "+r"(state) : "r"(callback));
    state->completed20 = completed;
    func_8008C12C(callback);
    command = 6;
    if (D_800A63F4 & 1) {
        func_8008C55C(func_8008EA9C);
        command = 6;
    }
    state->position08 = state->buffer04;
    func_8008C27C(command, 0);
    state->remaining14 = state->sectors00;
    ((volatile RrLibReadRequestState *)state)->retry18 = func_8008B154(-1);
    return ((volatile RrLibReadRequestState *)state)->remaining14;
}

void func_8008ED54(void) {
    volatile RrLibReadRequestState *root;
    /* ASM_BARRIER: form the completion-publication address in its original temporary. */
    register RrLibReadRequestState *publish __asm__("$2");
    /* ASM_BARRIER: retain the callback slot while deriving its containing record. */
    register RrLibCompletionCallback *saved __asm__("$16");
    RrLibCompletionCallback restored;
    /* ASM_BARRIER: retain the callback-pair address through the stop sequence. */
    register RrLibCompletionCallback volatile *pair __asm__("$17");
    volatile s32 *pending_a,*pending_b;
    u32 first_tick,second_tick;
    s32 decision;
    if(D_800A63F4&1)func_8008C580(0);
    root=&D_800A67B0;
    /* ASM_BARRIER: capture the complete state address before initial publication. */
    __asm__("" : "+r"(root));
    root->remaining14=0;
    if(root->finished24!=0)goto begin_first_wait;
    decision=1;
    goto after_first_wait;
first_timeout:
    saved=&D_800A67D8;
    /* ASM_BARRIER: materialize the saved-callback slot before its first load. */
    __asm__("" : "+r"(saved));
    restored=*saved;
    root=(RrLibReadRequestState*)((u8*)saved-RR_READ_OWNER_OFFSET);
    func_8008C118(restored);
    decision=-1;
    /* The field's object type is ordinary s32. This publication follows callback
     * restoration and retains the original branch-delay store. */
    ((RrLibReadRequestState*)root)->finished24=0;
    goto after_first_wait;
second_timeout:
    saved=&D_800A67D8;
    /* ASM_BARRIER: materialize the saved-callback slot before the second timeout load. */
    __asm__("" : "+r"(saved));
    restored=*saved;
    root=(RrLibReadRequestState*)((u8*)saved-RR_READ_OWNER_OFFSET);
    func_8008C118(restored);
    root->finished24=0;
    goto done;
begin_first_wait:
    first_tick=func_8008B154(-1);
    if(root->finished24!=0) {
        pending_a=&root->finished24;
first_poll:
        if((u32)func_8008B154(-1)-first_tick>=121U)goto first_timeout;
        if(*pending_a!=0)goto first_poll;
    }
    decision=0;
after_first_wait:
    if(decision!=0) {
        /* ASM_BARRIER: keep callback restoration after the completed-wait branch. */
        __asm__("" : );
        pair=&D_800A67DC;
        /* ASM_BARRIER: capture the complete callback-pair address before setters. */
        __asm__("" : "+r"(pair));
        func_8008C12C(RR_PAIR_TAIL(pair)->saved2C);
        if(D_800A63F4&1)func_8008C55C(RR_PAIR_TAIL(pair)->saved30);
        func_8008C118(func_8008E7F4);
        func_8008C27C(9,0);
        publish=(RrLibReadRequestState*)((u8*)pair-RR_PAIR_OWNER_OFFSET);
        ((volatile RrLibReadRequestState*)publish)->finished24=1;
        if(RR_PAIR_PENDING(pair)!=0) {
            second_tick=func_8008B154(-1);
            if(RR_PAIR_PENDING(pair)!=0) {
                pending_b=&RR_PAIR_PENDING(pair);
second_poll:
                if((u32)func_8008B154(-1)-second_tick>=121U)goto second_timeout;
                if(*pending_b!=0)goto second_poll;
            }
        }
    }
done:
    return;
}

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
