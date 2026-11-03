/* RODATA 0x80012108 0x4c */
/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/serial.h"
#include "rr/interrupts.h"

/* ASM_BARRIER: retain a constant data base in a register so its fields use base offsets. */
#define KEEP_BASE(p) __asm__ volatile("" : "=r"(p) : "0"(p))

extern s32 func_800929D4(s32);
extern s32 func_80096F04(void);
extern void func_80096F14(void);
extern RrSerialPortRegisters D_801146C0;
extern volatile s32 D_801146D0;
extern s16 D_800ABD34[];
extern volatile s32 *D_800ABD1C;
extern s32 func_80096E64(u32, s32);

s32 func_8009AEAC(u32 arg0, u32 arg1, u32 arg2);
s32 func_8009B380(void);
s32 func_8009B3FC(void);

s32 func_8009AEAC(u32 arg0, u32 arg1, u32 arg2) {
    s32 ret = 0;

    switch (arg0) {
    case 0:
        switch (arg1) {
        case 0:
            ret = D_800ABD18->status;
            break;
        case 1:
            ret = (D_800ABD18->control >> 1) & 1;
            if (D_800ABD18->control & 0x20) {
                ret |= 2;
            }
            break;
        case 2:
            ret = D_801146C0.mode;
            break;
        case 3:
            ret = 0x1FA400 / D_801146C0.baud;
            break;
        case 4:
            ret = 1;
            break;
        case 5:
            if (arg2 == 0) {
                ret = *(volatile s32 *)&D_801146D8.request.second;
            } else {
                ret = D_801146E8.second;
            }
            break;
        case 6: {
            volatile RrSerialRequest *req = &D_801146E8;
            if (arg2 == 0) {
                req = &D_801146D8.request;
            }
            ret = req->active != 0;
            break;
        }
        }
        break;
    case 1:
        switch (arg1) {
        case 0:
            break;
        case 1: {
            /* ASM_BARRIER: pin the port pointer to $v1 like the original allocation */
            register RrSerialPortRegisters *p asm("$3") = &D_801146C0;
            s32 dtr;
            u32 dtr_copy;
            u32 ctrl;
            u32 v;
            p->control = D_800ABD18->control & ~0x22;
            dtr = (arg2 & 1) << 1;
            dtr_copy = dtr;
            ctrl = p->control;
            if (arg2 & 2) {
                v = ctrl | 0x20;
                v |= dtr;
            } else {
                v = ctrl | dtr_copy;
            }
            p->control = v;
            D_800ABD18->control = D_801146C0.control;
            break;
        }
        case 2: {
            RrSerialPortRegisters *p = &D_801146C0;
            p->mode = arg2;
            D_800ABD18->mode = p->mode;
            break;
        }
        case 3: {
            RrSerialPortRegisters *p;
            if ((0x1FA400 % arg2) != 0) {
                ret = -1;
                break;
            }
            p = &D_801146C0;
            p->baud = 0x1FA400 / arg2;
            D_800ABD18->baud = p->baud;
            D_800ABD18->control |= 0x10;
            break;
        }
        case 4: {
            RrSerialPortRegisters *p;
            if (arg2 >= 1 && arg2 <= 8) {
                ret = D_800ABD34[arg2];
                if (ret < 0) {
                    break;
                }
                p = &D_801146C0;
                p->control &= ~0x300;
                p->control |= ret;
                D_800ABD18->control = p->control;
            }
            break;
        }
        }
        break;
    case 2:
        switch (arg1) {
        case 0: {
            s32 v = func_80096F04();
            RrSerialPortRegisters *port = D_800ABD18;
            port->control |= 0x50;
            port->mode = D_801146C0.mode;
            port->control = D_801146C0.control;
            port->baud = D_801146C0.baud;
            port->control |= 0x10;
            ret = 0;
            if (v == 1) {
                func_80096F14();
            }
            break;
        }
        case 1:
            ret = 0;
            D_800ABD18->control |= 0x10;
            break;
        case 2: {
            s32 v = func_80096F04();
            D_800ABD18->control &= ~0x400;
            D_801146D8.request.active = 0;
            D_801146D8.request.second = 0;
            if (v == 1) {
                func_80096F14();
            }
            break;
        }
        case 3: {
            s32 v = func_80096F04();
            D_800ABD18->control &= ~0x820;
            D_801146E8.active = 0;
            D_801146E8.second = 0;
            if (v == 1) {
                func_80096F14();
            }
            break;
        }
        }
        break;
    case 3:
        switch (arg1) {
        case 0:
            if (arg2 != 0) {
                D_800ABD18->control |= 0x20;
            } else {
                D_800ABD18->control &= ~0x20;
            }
            break;
        case 1:
            if (D_800ABD18->status & 0x100) {
                ret = 1;
            }
            break;
        }
        break;
    case 4:
        if (arg1 == 0) {
            volatile s32 *q = &D_801146D0;
            ret = *q;
            *(s32 *)q = arg2;
        }
        break;
    case 5:
        if (arg1 == 0) {
            func_800929D4(5);
        }
        break;
    }
    return ret;
}

s32 func_8009B380(void) {
    RrInterruptRegisters *interrupts=D_800ABD30;
    /* ASM_BARRIER: Keep the predicate result in the common return register. */
    register s32 result;
    if(interrupts->pending & interrupts->mask & 0x100) {
        RrSerialReceiveRequest *request=&D_801146D8;
        /* ASM_BARRIER: Keep the original address materialization before the active load. */
        __asm__("" : "+r"(request));
        if(request->request.active) {
            RrSerialPortRegisters *port=D_800ABD18;
            if(!(port->status & 1)) {
                result=1;
                goto done;
            }
            port->control &= 0xFBFF;
        }
        result=1;
    } else result=0;
done:
    /* ASM_BARRIER: Keep the selected result before the shared return instruction. */
    __asm__ volatile("" : : "r"(result));
    return result;
}

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
