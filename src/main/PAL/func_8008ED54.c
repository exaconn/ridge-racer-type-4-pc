#include "rr/cd_read_stop.h"
#include "rr/cd.h"

extern RrLibCompletionCallback volatile D_800A67DC;
extern void func_8008C580(s32);
extern s32 func_8008B154(s32);
extern RrLibCompletionCallback func_8008C118(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C12C(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C55C(RrLibCompletionCallback);
extern void func_8008E7F4(void);

#define RR_READ_OWNER_OFFSET \
    (sizeof(RrLibReadRequestState)-sizeof(RrLibReadRestoreTail))
#define RR_PAIR_OWNER_OFFSET (RR_READ_OWNER_OFFSET+sizeof(RrLibCompletionCallback))
#define RR_PAIR_TAIL(pair) \
    ((volatile RrLibReadRestoreTail*)((u8*)(pair)-sizeof(RrLibCompletionCallback)))
#define RR_PAIR_REQUEST(pair) \
    ((volatile RrLibReadRequestState*)((u8*)(pair)-RR_PAIR_OWNER_OFFSET))
#define RR_PAIR_PENDING(pair) (RR_PAIR_REQUEST(pair)->finished24)

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
