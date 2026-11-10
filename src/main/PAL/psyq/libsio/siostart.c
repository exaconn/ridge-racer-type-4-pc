/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr/serial.h"
#include "psyq/libsio/siostart.h"

extern int EnterCriticalSection();
extern int ExitCriticalSection();
extern int AddDrv();
extern s32 SioDeviceBlock;
s32 FlushCache();
/* extern */
s32 DelDrv(s32 *);
/* extern */
extern s32 StrSioDeviceName;

void DelSIO(void);
void _sio_Flush(void);
s32 _sio_write_async(u8 *buffer, s32 length);

void AddSIO(void) {
    s32 temp_s0;

    temp_s0 = EnterCriticalSection();
    AddDrv(&SioDeviceBlock);
    if (temp_s0 == 1) {
        ExitCriticalSection();
    }
}

void DelSIO(void) {
    s32 temp_s0;

    temp_s0 = EnterCriticalSection();
    DelDrv(&StrSioDeviceName);
    FlushCache();
    if (temp_s0 == 1) {
        ExitCriticalSection();
    }
}


void _sio_Flush(void) {
}

s32 _sio_write_async(u8 *buffer, s32 length) {
    volatile SerialRequest *request = &SioRequest;
    SerialPortRegisters *registers;

    if (request->active == 0) {
        request->remaining = length;
        request->cursor = buffer;
        registers = SIO1_REGS;
        request->active = 1;
        /* Keep the separate read/write operations on the control register. */
        registers->control |= 0x800;
        registers->control |= 0x20;
        return 0;
    }
    return -1;
}
