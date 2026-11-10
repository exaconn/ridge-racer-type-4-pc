/* RODATA 0x80012108 0x4c */
/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/serial.h"
#include "rr/bios_file_types.h"
#include "rr/interrupts.h"
#include "psyq/libsio/siorecv.h"
#include "psyq/libsio/siosend.h"
#include "psyq/libsio/siostart.h"
#include "psyq/libsio/siowait.h"

/* ASM_BARRIER: retain a constant data base in a register so its fields use base offsets. */
#define KEEP_BASE(p) __asm__ volatile("" : "=r"(p) : "0"(p))

extern s32 ResetGraph(s32);
extern s32 EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern volatile s32 SioPollHook;
extern s16 D_800ABD34[];
extern volatile s32 *SioDataReg;
extern s32 DeliverEvent(u32, s32);
extern void SysEnqIntRP();
extern void SysDeqIntRP();
extern char SioIrqQueueEntry[];

s32 _sio_control(u32 arg0, u32 arg1, u32 arg2);
s32 _sio_irq_handler(void);
int _sio_init(void);
s32 _sio_open(void);
s32 _sio_close(void);
int _sio_inout(BiosFileControlBlock *file, int direction);
s32 _sio_unsupported(void);

s32 _sio_control(u32 arg0, u32 arg1, u32 arg2) {
    s32 ret = 0;

    switch (arg0) {
    case 0:
        switch (arg1) {
        case 0:
            ret = SIO1_REGS->status;
            break;
        case 1:
            ret = (SIO1_REGS->control >> 1) & 1;
            if (SIO1_REGS->control & 0x20) {
                ret |= 2;
            }
            break;
        case 2:
            ret = SioShadowRegs.mode;
            break;
        case 3:
            ret = 0x1FA400 / SioShadowRegs.baud;
            break;
        case 4:
            ret = 1;
            break;
        case 5:
            if (arg2 == 0) {
                ret = *(volatile s32 *)&SioReceiveRequest.request.remaining;
            } else {
                ret = SioRequest.remaining;
            }
            break;
        case 6: {
            volatile SerialRequest *req = &SioRequest;
            if (arg2 == 0) {
                req = &SioReceiveRequest.request;
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
            register SerialPortRegisters *p asm("$3") = &SioShadowRegs;
            s32 dtr;
            u32 dtr_copy;
            u32 ctrl;
            u32 v;
            p->control = SIO1_REGS->control & ~0x22;
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
            SIO1_REGS->control = SioShadowRegs.control;
            break;
        }
        case 2: {
            SerialPortRegisters *p = &SioShadowRegs;
            p->mode = arg2;
            SIO1_REGS->mode = p->mode;
            break;
        }
        case 3: {
            SerialPortRegisters *p;
            if ((0x1FA400 % arg2) != 0) {
                ret = -1;
                break;
            }
            p = &SioShadowRegs;
            p->baud = 0x1FA400 / arg2;
            SIO1_REGS->baud = p->baud;
            SIO1_REGS->control |= 0x10;
            break;
        }
        case 4: {
            SerialPortRegisters *p;
            if (arg2 >= 1 && arg2 <= 8) {
                ret = D_800ABD34[arg2];
                if (ret < 0) {
                    break;
                }
                p = &SioShadowRegs;
                p->control &= ~0x300;
                p->control |= ret;
                SIO1_REGS->control = p->control;
            }
            break;
        }
        }
        break;
    case 2:
        switch (arg1) {
        case 0: {
            s32 v = EnterCriticalSection();
            SerialPortRegisters *port = SIO1_REGS;
            port->control |= 0x50;
            port->mode = SioShadowRegs.mode;
            port->control = SioShadowRegs.control;
            port->baud = SioShadowRegs.baud;
            port->control |= 0x10;
            ret = 0;
            if (v == 1) {
                ExitCriticalSection();
            }
            break;
        }
        case 1:
            ret = 0;
            SIO1_REGS->control |= 0x10;
            break;
        case 2: {
            s32 v = EnterCriticalSection();
            SIO1_REGS->control &= ~0x400;
            SioReceiveRequest.request.active = 0;
            SioReceiveRequest.request.remaining = 0;
            if (v == 1) {
                ExitCriticalSection();
            }
            break;
        }
        case 3: {
            s32 v = EnterCriticalSection();
            SIO1_REGS->control &= ~0x820;
            SioRequest.active = 0;
            SioRequest.remaining = 0;
            if (v == 1) {
                ExitCriticalSection();
            }
            break;
        }
        }
        break;
    case 3:
        switch (arg1) {
        case 0:
            if (arg2 != 0) {
                SIO1_REGS->control |= 0x20;
            } else {
                SIO1_REGS->control &= ~0x20;
            }
            break;
        case 1:
            if (SIO1_REGS->status & 0x100) {
                ret = 1;
            }
            break;
        }
        break;
    case 4:
        if (arg1 == 0) {
            s32 *q = &SioPollHook;
            ret = *q;
            *(s32 *)q = arg2;
        }
        break;
    case 5:
        if (arg1 == 0) {
            ResetGraph(5);
        }
        break;
    }
    return ret;
}

s32 _sio_irq_check(void) {
    InterruptRegisters *interrupts = I_STAT_Sio;
    if (interrupts->pending & interrupts->mask & 0x100) {
        volatile SerialReceiveRequest *request = &SioReceiveRequest;
        if (request->request.active) {
            SerialPortRegisters *port = SIO1_REGS;
            if (!(port->status & 1)) {
                return 1;
            }
            port->control &= 0xFBFF;
        }
        return 1;
    }
    return 0;
}

s32 _sio_irq_handler(void) {
    s32 count;
    if (I_STAT_Sio->pending & 0x100) {
        I_STAT_Sio->pending = ~0x100;
    }
    if (SIO1_REGS->status & 0x38) {
        volatile u16 *cfg;
        volatile SerialRequest *req;

        SIO1_REGS->control &= ~0x820;
        cfg = &SioShadowRegs.mode;
        KEEP_BASE(cfg);
        req = &SioRequest;

        req->remaining = 0;
        req->active = 0;
        SIO1_REGS->control;
        SIO1_REGS->control = 0x50;
        SIO1_REGS->mode = cfg[0];
        SIO1_REGS->baud = cfg[3];
        SIO1_REGS->control = SIO1_REGS->control | 0x10;
        DeliverEvent(0xF000000B, 0x8000);
        return 0;
    }
    {
        volatile SerialRequest *req = &SioRequest;
        if (req->active != 0 && req->remaining != 0 && (SIO1_REGS->status & 2)) {
            count = SioRxFifoSizeTable[(SioShadowRegs.control & 0x300) >> 8];
            switch (count) {
            case 1:
                *SioRequest.cursor = SIO1_REGS->data;
                break;
            case 2:
                *SioRequest.cursor = SIO1_REGS->data;
                SioRequest.cursor[1] = SIO1_REGS->data;
                break;
            case 4:
                *(s32 *)SioRequest.cursor = *SioDataReg;
                break;
            case 8:
                *(s32 *)SioRequest.cursor = *SioDataReg;
                *((s32 *)SioRequest.cursor + 1) = *SioDataReg;
                break;
            }
            {
                volatile SerialRequest *tx = &SioRequest;
                tx->cursor += count;
                tx->remaining -= count;
                if (tx->remaining == 0) {
                    tx->active = 0;
                    SIO1_REGS->control = SIO1_REGS->control & ~0x820;
                    DeliverEvent(0xF000000B, 0x400);
                }
            }
            SIO1_REGS->control ^= 2;
        }
    }
    {
        volatile SerialReceiveRequest *rx = &SioReceiveRequest;
        if (rx->request.active != 0 && (SIO1_REGS->status & 1) &&
            ((SIO1_REGS->status & 0x80) == rx->receive_status)) {
            if (rx->request.remaining == 0) {
                rx->request.active = 0;
                DeliverEvent(0xF000000B, 0x800);
            } else {
                rx->receive_status ^= 0x80;
                SIO1_REGS->data = *SioReceiveRequest.request.cursor;
                rx->request.cursor += 1;
                rx->request.remaining -= 1;
                SIO1_REGS->control |= 0x400;
            }
        }
    }
    SIO1_REGS->control |= 0x10;
    return 0;
}

int _sio_init(void) {
    SerialPortRegisters *a = &SioShadowRegs;
    SerialPortRegisters *b = SIO1_REGS;
    char *s;
    int r;
    int t;
    volatile SerialRequest *e;
    volatile SerialReceiveRequest *d;
    a->control = 5;
    a->mode = 0xCE;
    a->baud = 0xD8;
    b->control = 0x50;
    b->mode = a->mode;
    b->baud = a->baud;
    b->control |= 0x10;
    b->control = a->control;
    s = SioIrqQueueEntry;
    SysDeqIntRP(3, s);
    SysEnqIntRP(3, s);
    I_STAT_Sio->mask |= 0x100;
    e = &SioRequest;
    d = &SioReceiveRequest;
    e->active = 0;
    d->request.active = e->active;
    e->cursor = 0;
    d->request.cursor = e->cursor;
    e->remaining = 0;
    t = e->remaining;
    SioPollHook = 0;
    /* ASM_BARRIER: return constant 0 through a register (original li v0,0 after the last store) */
    __asm__ volatile("" : "=r"(r) : "0"(0));
    d->request.remaining = t;
    return r;
}

s32 _sio_open(void) {
    return 0;
}

s32 _sio_close(void) {
    return 0;
}

s32 _sio_deinit(void) {
    InterruptRegisters *interrupts=I_STAT_Sio;
    SerialPortRegisters *port;
    volatile u16 *control;
    interrupts->mask &= ~0x100U;
    port=SIO1_REGS;
    port->control |= 0x50;
    control=&SioShadowRegs.control;
    /* The final control write occupies the original call's delay slot. */
    port->control=*control;
    SysDeqIntRP(3,SioIrqQueueEntry);
    {
        /* Share the request bases across the store/reload pairs. */
        volatile SerialRequest *send=&SioRequest;
        volatile SerialReceiveRequest *receive;
        /* ASM_BARRIER: Reload request words through the original value register. */
        register s32 active __asm__("$3");
        send->active=0;
        active=send->active;

        receive=&SioReceiveRequest;
        receive->request.active=active;
        send->remaining=0;
        active=send->remaining;
        receive->request.remaining=active;
        return 0;
    }
}


int _sio_inout(BiosFileControlBlock *file, int direction)
{
    int result;
    file->error = 0;
    if (direction == 1) {
        if (file->status & 0x8000) {
            if (_sio_write_async(file->transfer_address, file->transfer_length) != 0) {
                file->error = 0x10;
                return -1;
            }
            return 0;
        }
        result = _sio_write(file->transfer_address, file->transfer_length);
        if (result < 0) file->error = 5;
        return result;
    }
    if (direction == 2) {
        if (file->status & 0x8000) {
            if (_sio_read_async(file->transfer_address, file->transfer_length) == 0) return 0;
            file->error = 0x10;
            return -1;
        }
        result = _sio_read(file->transfer_address, file->transfer_length);
        if (result < 0) file->error = 5;
        return result;
    }
    file->error = 0x16;
    return -1;
}

s32 _sio_unsupported(void) {
    return 0;
}
