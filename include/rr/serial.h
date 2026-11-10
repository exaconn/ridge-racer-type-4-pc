#ifndef RR_SERIAL_H
#define RR_SERIAL_H

#include "rr/layout.h"

typedef struct {
    volatile u8 data;
    u8 pad01[3];
    volatile u16 status;
    u8 pad06[2];
    volatile u16 mode;
    volatile u16 control;
    u8 pad0C[2];
    volatile u16 baud;
} SerialPortRegisters;

typedef struct {
    s32 active;
    u8 *cursor;
    s32 remaining;
} SerialRequest;

typedef struct {
    SerialRequest request;
    u32 receive_status;
} SerialReceiveRequest;

/* Cursor advances inside the caller-owned transfer buffer. The IRQ handler
 * shares active/remaining state with synchronous and asynchronous callers. */
extern SerialRequest volatile SioRequest;
extern volatile SerialReceiveRequest SioReceiveRequest;
extern SerialPortRegisters SioShadowRegs;
extern SerialPortRegisters *CardSio0Regs;
extern SerialPortRegisters *SIO1_REGS;
extern SerialPortRegisters *PadSio0Base;
/* Three independent SDK pointer slots initially select SIO0 at0x1F801040. */
extern SerialPortRegisters *JOY_DATA, *D_800ABD00;
s32 _sio_deinit(void);

extern SerialPortRegisters *volatile CardCmdSioRegs;
extern SerialPortRegisters *CardSio0Base;
s32 _cardSelectSlot(s32 slot);
extern volatile u32 *I_STAT_CardSel;
extern s32 D_80114700;
void setRC2wait(s32 limit);
s32 chkRC2wait(void);
/* Negative input writes its complemented low byte; nonnegative input uses
 * the interrupt handshake. Returns captured data or the corresponding timeout. */
s32 _cardXferByte(s32 value);

#if defined(__mips__)
RR_ASSERT_SIZE(SerialRequest, 0xC);
RR_ASSERT_OFFSET(SerialRequest, cursor, 4);
RR_ASSERT_OFFSET(SerialRequest, remaining, 8);
RR_ASSERT_SIZE(SerialReceiveRequest, 0x10);
RR_ASSERT_OFFSET(SerialReceiveRequest, receive_status, 0xC);
#endif

RR_ASSERT_SIZE(SerialPortRegisters, 0x10);
RR_ASSERT_OFFSET(SerialPortRegisters, status, 4);
RR_ASSERT_OFFSET(SerialPortRegisters, mode, 8);
RR_ASSERT_OFFSET(SerialPortRegisters, control, 0xA);
RR_ASSERT_OFFSET(SerialPortRegisters, baud, 0xE);

#endif
