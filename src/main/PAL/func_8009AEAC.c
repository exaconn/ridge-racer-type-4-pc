/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
/* RODATA 0x80012108 0x4c */
#include "rr/serial.h"

extern s32 func_800929D4(s32);
extern s32 func_80096F04(void);
extern void func_80096F14(void);

extern RrSerialPortRegisters D_801146C0;
extern volatile s32 D_801146D0;
extern s16 D_800ABD34[];

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
