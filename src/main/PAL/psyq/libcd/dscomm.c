#include "rr/cd_read_callback.h"
#include "rr/cd.h"
#include "rr/library.h"
#include "rr/cd_read_request.h"
#include "rr/cd_read_stop.h"
#include "psyq/libcd/s_1.h"
#include "psyq/libcd/s_2.h"
#include "psyq/libcd/s_3.h"
#include "psyq/libcd/dsread2.h"
#include "rr/overlay668_stream.h"
#include "rr/audio_cd_controller.h"

#define RR_READ_OWNER_OFFSET \
    (sizeof(LibReadRequestState)-sizeof(LibReadRestoreTail))
#define RR_PAIR_OWNER_OFFSET (RR_READ_OWNER_OFFSET+sizeof(LibCompletionCallback))
#define RR_PAIR_TAIL(pair) \
    ((volatile LibReadRestoreTail*)((u8*)(pair)-sizeof(LibCompletionCallback)))
#define RR_PAIR_REQUEST(pair) \
    ((volatile LibReadRequestState*)((u8*)(pair)-RR_PAIR_OWNER_OFFSET))
#define RR_PAIR_PENDING(pair) (RR_PAIR_REQUEST(pair)->finished24)

extern LibCompletionCallback CdDataCallback(LibCompletionCallback);
extern s32 PsyqPuts(const char *,...);
extern const char CdReadSectorErrorMsg[];
extern void cb_data(void);
extern const char StrCdReadShellOpen[];
extern const char StrCdReadRetry[];
extern volatile u32 D_800A67BC;
extern LibCompletionCallback volatile D_800A67DC;

void cb_read(s32 event,u8 *data);
void cb_data(void);
s32 CdRead(s32 sectors,u8 *buffer,u32 mode);

void cb_read(s32 event,u8 *data) {
    u8 header[12];
    register volatile LibReadRequestState *initial=&CdReadState, *state, *transfer, *failed;
    register u32 *completed;
    register CdPositionView *position;
    register CdPositionView *deferred;
    CdDataNotify notify;
    s32 now;
    register u8 *notify_page;

    /* ASM_BARRIER: materialize the initial full address before saving data. */
    __asm__("" : "+r"(initial));
    initial->restore.notify34=data;
    if ((u8)event==1) {
        if (initial->remaining14>0) {
            if (initial->step10==512) {
                /* ASM_BARRIER: retain flags address formation inside this arm. */
                __asm__("" : );
                if (CD_read_dma_mode&1) {
                    CdDataCallback(0);
                    CdGetSector2(header,3);
                    CdDataSync(0);
                    CdDataCallback(cb_data);
                } else {
                    CdGetSector(header,3);
                }
                now=CdPosToInt(header);
                completed=&D_800A67D0;
                /* ASM_BARRIER: capture the complete counter address before load. */
                __asm__("" : "+r"(completed));
                if (now!=*completed) {
                    PsyqPuts(CdReadSectorErrorMsg);
                    failed=(LibReadRequestState *)((u8 *)completed-
                                                     RR_READ_COMPLETED_OWNER_OFFSET);
                    failed->remaining14=-1;
                }
            }
            /* ASM_BARRIER: reload flags at the common transfer join. */
            __asm__("" : );
            if (CD_read_dma_mode&1) {
                /* ASM_BARRIER: keep the deferred address inside its branch. */
                __asm__("" : );
                deferred=&D_800A67B8;
                /* ASM_BARRIER: form the complete window before loading arguments. */
                __asm__("" : "+r"(deferred));
                CdGetSector2(deferred->position08,deferred->step10);
            } else {
                /* ASM_BARRIER: keep the immediate address inside its branch. */
                __asm__("" : );
                position=&D_800A67B8;
                /* ASM_BARRIER: retain one captured window across the transfer. */
                __asm__("" : "+r"(position));
                CdGetSector(position->position08,position->step10);
                transfer=(LibReadRequestState *)((u8 *)position-
                                                   RR_READ_POSITION_OWNER_OFFSET);
                transfer->position08=position->position08+(position->step10<<2);
                transfer->remaining14=(transfer->remaining14-1U);
                /* The object is ordinary; qualify the load for a fresh value,
                 * then allow its publication in the unconditional jump delay. */
                ((LibReadRequestState *)transfer)->completed20=transfer->completed20+1U;
            }
        }
    } else {
        initial->remaining14=-1;
    }
    now=VSync(-1);
    state=&CdReadState;
    /* ASM_BARRIER: form the complete state address after the clock returns. */
    __asm__("" : "+r"(state));
    state->retry18=now;
    if (state->remaining14<0) {
        cd_read_retry(1);
    }
    now=VSync(-1);
    if ((s32)(state->start1C+1200U)<now) {
        state->remaining14=-1;
    }
    if (state->remaining14==0 ||
        (now=VSync(-1),(s32)(state->start1C+1200U)<now)) {
        CdReadyCallback(state->restore.saved2C);
        if (CD_read_dma_mode&1) {
            CdDataCallback(state->restore.saved30);
        }
        CdSyncCallback(cd_read_restore_sync);
        CdControlF(9,0);
        /* PAL page+0x63F0 contains the data notification function pointer. */
        notify_page=(u8 *)0x800A0000U;
        notify=*(CdDataNotify *)(notify_page+0x63F0);
        ((LibReadRequestState *)state)->finished24=1;
        if (notify) {
            notify(state->remaining14==0?2:5,data);
        }
    }
}

void cb_data(void) {
    register volatile LibTransferState38 *state = &CdReadState;
    /* ASM_BARRIER: Keep the complete state address in its saved register. */
    __asm__("" : "=r"(state) : "0"(state));
    state->position08 += state->step10 << 2;
    state->remaining14 -= 1;
    state->completed20 += 1;
    if (state->remaining14 == 0) {
        u8 *page;
        LibNotificationCallback callback;
        CdReadyCallback(state->restore2C);
        if (CD_read_dma_mode & 1) CdDataCallback(state->restore30);
        CdSyncCallback(cd_read_restore_sync);
        CdControlF(9, 0);
        /* PAL mapping: page + 0x63F0 is &CD_cbread. */
        page = (u8 *)0x800A0000U;
        callback = *(LibNotificationCallback *)(page + 0x63F0);
        /* Ordinary state publication permits the original branch-delay store. */
        ((LibTransferState38 *)state)->finished24 = 1;
        if (callback != 0) callback(2, state->notify_arg34);
    }
}

s32 cd_read_retry(s32 restart) {
    u8 mode_byte;
    register u32 requested;
    register LibReadRequestState *state;
    /* ASM_BARRIER: keep the captured mode address in s1 and preserve the raw
     * restart argument in s2 through the command and status helpers. */
    register u32 *mode_slot __asm__("$17");
    register volatile LibReadRequestState *shell_state, *failed_state;
    u32 timestamp;
    s32 completed;
    register LibCompletionCallback callback;
    /* ASM_BARRIER: a0/a1 stage independent command/null-parameter values;
     * neither bound value is used across a call that can overwrite it. */
    register s32 command __asm__("$4");
    register const u8 *parameters __asm__("$5");

    CdSyncCallback(0);
    CdReadyCallback(0);
    if (CD_read_dma_mode & 1) {
        CdDataCallback(0);
    }
    if (CdStatus() & 0x10) {
        if (!(VSync(-1) & 63)) {
            PsyqPuts(StrCdReadShellOpen);
        }
        CdControlF(1, 0);
        timestamp = VSync(-1);
        shell_state = &CdReadState;
        shell_state->start1C = timestamp;
        shell_state->remaining14 = -1;
        return shell_state->remaining14;
    }
    if (restart != 0) {
        /* ASM_BARRIER: keep restart-only diagnostic address formation in this
         * branch instead of copying it into its predecessor's delay slot. */
        __asm__("" : );
        PsyqPuts(StrCdReadRetry);
        command = 9;
        parameters = 0;
        /* ASM_BARRIER: stage the null parameter after the command value. */
        __asm__("" : "+r"(parameters) : "r"(command));
        CdControl(command, parameters, 0);
        if (!CdControl(2, CdLastPos(), 0)) {
            goto failed;
        }
    }
    CdFlush();
    /* ASM_BARRIER: form the captured mode address after reset returns. */
    __asm__("" : : : "$17");
    mode_slot = &D_800A67BC;
    /* ASM_BARRIER: preserve the complete address before the mode load. */
    __asm__("" : "+r"(mode_slot));
    requested = *mode_slot;
    mode_byte = requested;
    requested &= 255;
    if (requested != CdMode() || restart != 0) {
        if (!CdControl(14, &mode_byte, 0)) {
failed:
            /* ASM_BARRIER: retain one shared failure block with fresh state
             * publication and reload, rather than duplicating address setup. */
            __asm__("" : );
            failed_state = &CdReadState;
            failed_state->remaining14 = -1;
            return failed_state->remaining14;
        }
    }
    completed = CdPosToInt(CdLastPos());
    /* ASM_BARRIER: the success-state address is formed after decoding. */
    __asm__("" : : : "$16");
    callback = (LibCompletionCallback)cb_read;
    state = &CdReadState;
    /* ASM_BARRIER: stage the callback address before the state address, then
     * publish the decoded sector in the callback setter's delay slot. */
    __asm__("" : "+r"(state) : "r"(callback));
    state->completed20 = completed;
    CdReadyCallback(callback);
    command = 6;
    if (CD_read_dma_mode & 1) {
        CdDataCallback(cb_data);
        command = 6;
    }
    state->position08 = state->buffer04;
    CdControlF(command, 0);
    state->remaining14 = state->sectors00;
    ((volatile LibReadRequestState *)state)->retry18 = VSync(-1);
    return ((volatile LibReadRequestState *)state)->remaining14;
}

void CdReadBreak(void) {
    volatile LibReadRequestState *root;
    /* ASM_BARRIER: form the completion-publication address in its original temporary. */
    register LibReadRequestState *publish __asm__("$2");
    /* ASM_BARRIER: retain the callback slot while deriving its containing record. */
    register LibCompletionCallback *saved __asm__("$16");
    LibCompletionCallback restored;
    /* ASM_BARRIER: retain the callback-pair address through the stop sequence. */
    register LibCompletionCallback *pair __asm__("$17");
    s32 *pending_a,*pending_b;
    u32 first_tick,second_tick;
    s32 decision;
    if(CD_read_dma_mode&1)CdDataSync(0);
    root=&CdReadState;
    /* ASM_BARRIER: capture the complete state address before initial publication. */
    __asm__("" : "+r"(root));
    root->remaining14=0;
    if(root->finished24!=0)goto begin_first_wait;
    decision=1;
    goto after_first_wait;
first_timeout:
    saved=&CdReadSavedSyncCallback;
    /* ASM_BARRIER: materialize the saved-callback slot before its first load. */
    __asm__("" : "+r"(saved));
    restored=*saved;
    root=(LibReadRequestState*)((u8*)saved-RR_READ_OWNER_OFFSET);
    CdSyncCallback(restored);
    decision=-1;
    /* The field's object type is ordinary s32. This publication follows callback
     * restoration and retains the original branch-delay store. */
    ((LibReadRequestState*)root)->finished24=0;
    goto after_first_wait;
second_timeout:
    saved=&CdReadSavedSyncCallback;
    /* ASM_BARRIER: materialize the saved-callback slot before the second timeout load. */
    __asm__("" : "+r"(saved));
    restored=*saved;
    root=(LibReadRequestState*)((u8*)saved-RR_READ_OWNER_OFFSET);
    CdSyncCallback(restored);
    root->finished24=0;
    goto done;
begin_first_wait:
    first_tick=VSync(-1);
    if(root->finished24!=0) {
        pending_a=&root->finished24;
        do {
            if(VSync(-1)-first_tick>=121U)goto first_timeout;
        } while(*pending_a!=0);
    }
    decision=0;
after_first_wait:
    if(decision!=0) {
        /* ASM_BARRIER: keep callback restoration after the completed-wait branch. */
        __asm__("" : );
        pair=&D_800A67DC;
        /* ASM_BARRIER: capture the complete callback-pair address before setters. */
        __asm__("" : "+r"(pair));
        CdReadyCallback(RR_PAIR_TAIL(pair)->saved2C);
        if(CD_read_dma_mode&1)CdDataCallback(RR_PAIR_TAIL(pair)->saved30);
        CdSyncCallback(cd_read_restore_sync);
        CdControlF(9,0);
        publish=(LibReadRequestState*)((u8*)pair-RR_PAIR_OWNER_OFFSET);
        ((volatile LibReadRequestState*)publish)->finished24=1;
        if(RR_PAIR_PENDING(pair)!=0) {
            second_tick=VSync(-1);
            if(RR_PAIR_PENDING(pair)!=0) {
                pending_b=&RR_PAIR_PENDING(pair);
                do {
                    if(VSync(-1)-second_tick>=121U)goto second_timeout;
                } while(*pending_b!=0);
            }
        }
    }
done:
    return;
}

s32 CdRead(s32 sectors,u8 *buffer,u32 mode) {
    s32 count=sectors;
    u8 *destination=buffer;
    u32 format;
    u32 start;
    /* ASM_BARRIER: retain the complete request address across callback setters. */
    register LibReadRequestState *state __asm__("$16");
    s32 *pending;
    /* ASM_BARRIER: use the original constructor argument register for case stores. */
    register LibReadRequestState *control __asm__("$4");
    u32 mode_stage=mode;
    s32 command,kind;
    LibCompletionCallback *restore_slot;
    LibCompletionCallback restored;
    u32 default_step;
    LibCompletionCallback zero;
    s32 result,busy;
    /* ASM_BARRIER: materialize the complete fallback address in its temporary. */
    register LibReadRequestState *fallback __asm__("$3");
    const u8 *parameters;
    pending=&D_800A67D4;
    /* ASM_BARRIER: capture the pending flag's complete address before polling. */
    __asm__("" : "+r"(pending));
    busy=*pending;
    format=mode_stage;
    if(busy!=0) {
        start=VSync(-1);
        if(*pending!=0) {
            do {
                if(VSync(-1)-start>=121U)goto timeout;
            } while(*pending!=0);
        }
    }
setup:
    /* ASM_BARRIER: keep the timeout jump aimed at the complete constructor entry. */
    __asm__("" : );
    control=&CdReadState;
    control->mode0C=format;
    kind=control->mode0C&0x30;
    switch(kind) {
/* The original mode-dispatch block also holds timeout recovery, which returns
 * to setup before selecting the transfer step. */
timeout:
        restore_slot=&CdReadSavedSyncCallback;
        /* ASM_BARRIER: capture the saved-callback slot before deriving its owner. */
        __asm__("" : "+r"(restore_slot));
        restored=*restore_slot;
        state=(LibReadRequestState*)((u8*)restore_slot-
              (sizeof(LibReadRequestState)-sizeof(LibReadRestoreTail)));
        CdSyncCallback(restored);
        ((volatile LibReadRequestState*)state)->finished24=0;
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
        fallback=&CdReadState;
        /* ASM_BARRIER: retain the complete fallback address for its field store. */
        __asm__("" : "+r"(fallback));
        fallback->step10=default_step;
        break;
    }
    state=&CdReadState;
    zero=0;
    state->mode0C|=0x20;
    /* ASM_BARRIER: publish mode before the request buffer and sector count. */
    __asm__("" : "+r"(state), "+r"(zero) : : "memory");
    state->buffer04=destination;
    state->sectors00=count;
    state->restore.saved28=CdSyncCallback(zero);
    state->restore.saved2C=CdReadyCallback(0);
    if(CD_read_dma_mode&1)state->restore.saved30=CdDataCallback(0);
    state->start1C=VSync(-1);
    if(CdStatus()&0xE0) {
        command=9;
        parameters=0;
        /* ASM_BARRIER: form the command and parameter arguments independently. */
        __asm__("" : "+r"(parameters) : "r"(command));
        CdControlB(command,parameters,0);
    }
    result=cd_read_retry(0)>0;
    /* ASM_BARRIER: finish the Boolean result before restoring saved temporaries. */
    __asm__("" : "+r"(result));
    return result;
}

