/* GCC_VERSION: 2.7.2 */
#include "rr/interrupts.h"
#include "rr/serial.h"

extern RrSerialPortRegisters D_801146C0;
extern volatile s32 *D_800ABD1C;
extern s32 func_80096E64(u32, s32);

/* ASM_BARRIER: retain a constant data base in a register so its fields use base offsets. */
#define KEEP_BASE(p) __asm__ volatile("" : "=r"(p) : "0"(p))

s32 func_8009B3FC(void) {
    s32 count;

    if (D_800ABD30->pending & 0x100) {
        D_800ABD30->pending = ~0x100;
    }
    if (D_800ABD18->status & 0x38) {
        volatile u16 *cfg;
        volatile RrSerialRequest *req;

        D_800ABD18->control &= ~0x820;
        cfg = &D_801146C0.mode;
        /* ASM_BARRIER: keep the configuration base for its mode/baud fields. */
        KEEP_BASE(cfg);
        req = &D_801146E8;
        /* ASM_BARRIER: keep the send request base for both cleared fields. */
        KEEP_BASE(req);
        req->second = 0;
        req->active = 0;
        D_800ABD18->control;
        D_800ABD18->control = 0x50;
        D_800ABD18->mode = cfg[0];
        D_800ABD18->baud = cfg[3];
        *(u16 *)&D_800ABD18->control = D_800ABD18->control | 0x10;
        func_80096E64(0xF000000B, 0x8000);
        return 0;
    }
    {
        volatile RrSerialRequest *req = &D_801146E8;
        /* ASM_BARRIER: keep the send request base for the active/second checks. */
        KEEP_BASE(req);
        if (req->active != 0 && req->second != 0 && (D_800ABD18->status & 2)) {
            s32 index = (D_801146C0.control & 0x300) >> 7;
            /* ASM_BARRIER: finish the table index before the table page lui (original assembler macro order). */
            __asm__ volatile("" : : "r"(index));
            count = 0x800B0000;
            /* ASM_BARRIER: build the size table address in the result register (lui/addu/lh %lo). */
            KEEP_BASE(count);
            count += index;
            count = *(s16 *)(count - 0x42B8);
            switch (count) {
            case 1:
                *(u8 *)D_801146E8.first = D_800ABD18->data;
                break;
            case 2:
                *(u8 *)D_801146E8.first = D_800ABD18->data;
                *((u8 *)D_801146E8.first + 1) = D_800ABD18->data;
                break;
            case 4:
                *(s32 *)D_801146E8.first = *D_800ABD1C;
                break;
            case 8:
                *(s32 *)D_801146E8.first = *D_800ABD1C;
                *((s32 *)D_801146E8.first + 1) = *D_800ABD1C;
                break;
            }
            {
                volatile RrSerialRequest *tx = &D_801146E8;
                /* ASM_BARRIER: keep the send request base for the count update. */
                KEEP_BASE(tx);
                tx->first += count;
                tx->second -= count;
                if (tx->second == 0) {
                    tx->active = 0;
                    *(u16 *)&D_800ABD18->control = D_800ABD18->control & ~0x820;
                    func_80096E64(0xF000000B, 0x400);
                }
            }
            D_800ABD18->control ^= 2;
        }
    }
    {
        volatile RrSerialReceiveRequest *rx = (volatile RrSerialReceiveRequest *)&D_801146D8;
        /* ASM_BARRIER: keep the receive request base for its fields. */
        KEEP_BASE(rx);
        if (rx->request.active != 0 && (D_800ABD18->status & 1) &&
            ((D_800ABD18->status & 0x80) == rx->receive_status)) {
            if (rx->request.second == 0) {
                rx->request.active = 0;
                func_80096E64(0xF000000B, 0x800);
            } else {
                rx->receive_status ^= 0x80;
                D_800ABD18->data = *(u8 *)D_801146D8.request.first;
                rx->request.first += 1;
                rx->request.second -= 1;
                D_800ABD18->control |= 0x400;
            }
        }
    }
    D_800ABD18->control |= 0x10;
    return 0;
}
