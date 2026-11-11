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
    volatile LibReadRequestState *initial=&CdReadState, *state, *transfer, *failed;
    u32 *completed;
    CdPositionView *position;
    CdPositionView *deferred;
    CdDataNotify notify;
    s32 now;
    u8 *notify_page;

    initial->restore.notify34=data;
    if ((u8)event==1) {
        if (initial->remaining14>0) {
            if (initial->step10==512) {
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
                if (now!=*completed) {
                    PsyqPuts(CdReadSectorErrorMsg);
                    failed=(LibReadRequestState *)((u8 *)completed-
                                                     RR_READ_COMPLETED_OWNER_OFFSET);
                    failed->remaining14=-1;
                }
            }
            if (CD_read_dma_mode&1) {
                deferred=&D_800A67B8;
                CdGetSector2(deferred->position08,deferred->step10);
            } else {
                position=&D_800A67B8;
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
        notify=((CdDataNotify *)notify_page)[0x63F0 / sizeof(CdDataNotify)];
        ((LibReadRequestState *)state)->finished24=1;
        if (notify) {
            notify(state->remaining14==0?2:5,data);
        }
    }
}

void cb_data(void) {
    volatile LibTransferState38 *state = &CdReadState;
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
        callback = ((LibNotificationCallback *)page)[0x63F0 / sizeof(LibNotificationCallback)];
        /* Ordinary state publication permits the original branch-delay store. */
        ((LibTransferState38 *)state)->finished24 = 1;
        if (callback != 0) callback(2, state->notify_arg34);
    }
}

s32 cd_read_retry(s32 restart) {
    u8 mode_byte;
    u32 requested;
    LibReadRequestState *state;
    u32 *mode_slot;
    volatile LibReadRequestState *shell_state, *failed_state;
    u32 timestamp;
    s32 completed;
    LibCompletionCallback callback;
    s32 command;
    const u8 *parameters;

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
        PsyqPuts(StrCdReadRetry);
        command = 9;
        parameters = 0;
        CdControl(command, parameters, 0);
        if (!CdControl(2, CdLastPos(), 0)) {
            goto failed;
        }
    }
    CdFlush();
    mode_slot = &D_800A67BC;
    requested = *mode_slot;
    mode_byte = requested;
    requested &= 255;
    if (requested != CdMode() || restart != 0) {
        if (!CdControl(14, &mode_byte, 0)) {
failed:
            failed_state = &CdReadState;
            failed_state->remaining14 = -1;
            return failed_state->remaining14;
        }
    }
    completed = CdPosToInt(CdLastPos());
    callback = (LibCompletionCallback)cb_read;
    state = &CdReadState;
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
    LibReadRequestState *publish;
    LibCompletionCallback *saved;
    LibCompletionCallback restored;
    LibCompletionCallback *pair;
    s32 *pending_a,*pending_b;
    u32 first_tick,second_tick;
    s32 decision;
    if(CD_read_dma_mode&1)CdDataSync(0);
    root=&CdReadState;
    root->remaining14=0;
    if(root->finished24!=0)goto begin_first_wait;
    decision=1;
    goto after_first_wait;
first_timeout:
    saved=&CdReadSavedSyncCallback;
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
        pair=&D_800A67DC;
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
    LibReadRequestState *state;
    s32 *pending;
    LibReadRequestState *control;
    u32 mode_stage=mode;
    s32 command,kind;
    LibCompletionCallback *restore_slot;
    LibCompletionCallback restored;
    u32 default_step;
    LibCompletionCallback zero;
    s32 result,busy;
    LibReadRequestState *fallback;
    const u8 *parameters;
    pending=&D_800A67D4;
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
    control=&CdReadState;
    control->mode0C=format;
    kind=control->mode0C&0x30;
    switch(kind) {
/* The original mode-dispatch block also holds timeout recovery, which returns
 * to setup before selecting the transfer step. */
timeout:
        restore_slot=&CdReadSavedSyncCallback;
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
        fallback=&CdReadState;
        fallback->step10=default_step;
        break;
    }
    state=&CdReadState;
    zero=0;
    state->mode0C|=0x20;
    state->buffer04=destination;
    state->sectors00=count;
    state->restore.saved28=CdSyncCallback(zero);
    state->restore.saved2C=CdReadyCallback(0);
    if(CD_read_dma_mode&1)state->restore.saved30=CdDataCallback(0);
    state->start1C=VSync(-1);
    if(CdStatus()&0xE0) {
        command=9;
        parameters=0;
        CdControlB(command,parameters,0);
    }
    result=cd_read_retry(0)>0;
    return result;
}

