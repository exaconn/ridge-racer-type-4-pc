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
} RrSerialPortRegisters;

typedef struct {
    s32 active;
    s32 first, second;
} RrSerialRequest;

typedef struct {
    RrSerialRequest request;
    u32 receive_status;
} RrSerialReceiveRequest;

extern RrSerialRequest volatile D_801146E8;
extern RrSerialReceiveRequest D_801146D8;
extern RrSerialPortRegisters *D_800ABDA8;
extern RrSerialPortRegisters *D_800ABD18;
extern RrSerialPortRegisters *D_800ABCCC;
s32 func_8009B85C(void);

extern RrSerialPortRegisters *volatile D_800ABDE0;
extern RrSerialPortRegisters *D_800ABDD0;
s32 func_8009BD44(s32 slot);
extern volatile u32 *D_800ABDD4;
extern s32 D_80114700;
void setRC2wait(s32 limit);
s32 chkRC2wait(void);
/* Negative input writes its complemented low byte; nonnegative input uses
 * the interrupt handshake. Returns captured data or the corresponding timeout. */
s32 func_8009BE18(s32 value);

RR_ASSERT_SIZE(RrSerialRequest, 0xC);
RR_ASSERT_OFFSET(RrSerialRequest, first, 4);
RR_ASSERT_OFFSET(RrSerialRequest, second, 8);
RR_ASSERT_SIZE(RrSerialReceiveRequest, 0x10);
RR_ASSERT_OFFSET(RrSerialReceiveRequest, receive_status, 0xC);

RR_ASSERT_SIZE(RrSerialPortRegisters, 0x10);
RR_ASSERT_OFFSET(RrSerialPortRegisters, status, 4);
RR_ASSERT_OFFSET(RrSerialPortRegisters, mode, 8);
RR_ASSERT_OFFSET(RrSerialPortRegisters, control, 0xA);
RR_ASSERT_OFFSET(RrSerialPortRegisters, baud, 0xE);

#endif
