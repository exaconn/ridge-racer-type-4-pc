#include "rr/cd_read_callback.h"
#include "rr/cd.h"

extern s32 func_8008B154(s32);
extern RrLibCompletionCallback func_8008C118(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C12C(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C55C(RrLibCompletionCallback);
extern s32 func_8008C53C(u8 *,u32);
extern void func_8008C580(s32);
extern s32 CdPosToInt(void *);
extern s32 func_800963DC(const char *,...);
extern const char D_80011D28[];
extern void func_8008EA9C(void),func_8008E7F4(void);

void func_8008E828(s32 event,u8 *data) {
    u8 header[12];
    register volatile RrLibReadRequestState *initial=&D_800A67B0;
    register volatile RrLibReadRequestState *state;
    register volatile RrLibReadRequestState *transfer;
    register volatile RrLibReadRequestState *failed;
    register u32 *completed;
    register volatile RrCdPositionView *position;
    register volatile RrCdPositionView *deferred;
    RrCdDataNotify notify;
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
                if (D_800A63F4&1) {
                    func_8008C55C(0);
                    func_8008C53C(header,3);
                    func_8008C580(0);
                    func_8008C55C(func_8008EA9C);
                } else {
                    func_8008C51C(header,3);
                }
                now=CdPosToInt(header);
                completed=&D_800A67D0;
                /* ASM_BARRIER: capture the complete counter address before load. */
                __asm__("" : "+r"(completed));
                if ((u32)now!=*completed) {
                    func_800963DC(D_80011D28);
                    failed=(RrLibReadRequestState *)((u8 *)completed-
                                                     RR_READ_COMPLETED_OWNER_OFFSET);
                    failed->remaining14=-1;
                }
            }
            /* ASM_BARRIER: reload flags at the common transfer join. */
            __asm__("" : );
            if (D_800A63F4&1) {
                /* ASM_BARRIER: keep the deferred address inside its branch. */
                __asm__("" : );
                deferred=&D_800A67B8;
                /* ASM_BARRIER: form the complete window before loading arguments. */
                __asm__("" : "+r"(deferred));
                func_8008C53C(deferred->position08,deferred->step10);
            } else {
                /* ASM_BARRIER: keep the immediate address inside its branch. */
                __asm__("" : );
                position=&D_800A67B8;
                /* ASM_BARRIER: retain one captured window across the transfer. */
                __asm__("" : "+r"(position));
                func_8008C51C(position->position08,position->step10);
                transfer=(RrLibReadRequestState *)((u8 *)position-
                                                   RR_READ_POSITION_OWNER_OFFSET);
                transfer->position08=position->position08+(position->step10<<2);
                transfer->remaining14=(s32)((u32)transfer->remaining14-1U);
                /* The object is ordinary; qualify the load for a fresh value,
                 * then allow its publication in the unconditional jump delay. */
                ((RrLibReadRequestState *)transfer)->completed20=transfer->completed20+1U;
            }
        }
    } else {
        initial->remaining14=-1;
    }
    now=func_8008B154(-1);
    state=&D_800A67B0;
    /* ASM_BARRIER: form the complete state address after the clock returns. */
    __asm__("" : "+r"(state));
    state->retry18=now;
    if (state->remaining14<0) {
        func_8008EB6C(1);
    }
    now=func_8008B154(-1);
    if ((s32)(state->start1C+1200U)<now) {
        state->remaining14=-1;
    }
    if (state->remaining14==0 ||
        (now=func_8008B154(-1),(s32)(state->start1C+1200U)<now)) {
        func_8008C12C(state->restore.saved2C);
        if (D_800A63F4&1) {
            func_8008C55C(state->restore.saved30);
        }
        func_8008C118(func_8008E7F4);
        CdControlF(9,0);
        /* PAL page+0x63F0 contains the data notification function pointer. */
        notify_page=(u8 *)0x800A0000U;
        notify=*(RrCdDataNotify *)(notify_page+0x63F0);
        ((RrLibReadRequestState *)state)->finished24=1;
        if (notify) {
            notify(state->remaining14==0?2:5,data);
        }
    }
}
