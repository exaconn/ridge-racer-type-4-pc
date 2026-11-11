/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr_asm.h"
#include "rr/commands.h"
#include "rr/serial.h"
#include "psyq/libcard/cardc11.h"
#include "psyq/libcard/cardc12.h"
#include "psyq/libcard/cardc10.h"

s32 _cardCmd5CVarBegin(s32 request, s32 parameter, u8 *buffer, u8 *first);

s32 _cardCmd5BVarRx(s32 value) {
    s32 saved=value;
    s32 received;
    SerialPortRegisters *port;
    s32 result;
    if(CardRxCount==3) {
        port=CardCmdSioRegs;
        RR_NOP_DEP_ON(port);
        while(!(port->status & 2)) { }
        CardCmdCount=CardCmdSioRegs->data;
    } else if(CardRxCount>=4) {
        if(CardRxCount==CardCmdCount+4U) {
            port=CardCmdSioRegs;
            RR_NOP_DEP_ON(port);
            while(!(port->status & 2)) { }
            received=CardCmdSioRegs->data;
            CardCmdBuffer=CardCmdResultPtrA;
            CardCmdParam.value=1;
            CardBytesRemaining=received+1;
        }
        {
            s32 count=CardCmdCount;
            s32 phase=CardRxCount;
            if((s32)(count+5U)<phase) {
                /* Preserve native pointer width and the original wrapping 32-bit difference. */
                unsigned long address=(phase-count);
                address+=(unsigned long)CardCmdBuffer;
                ((u8 *)address)[-6]=saved;
            }
        }
    }
    result=0;
    return result;
}

s32 _cardCmd5CVarBegin(s32 request, s32 parameter, u8 *buffer, u8 *first) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    CardCurrentCommand = 15;
    CardCommandReader = _cardCmd5CVarTx;
    CardWriteHandler = _cardCmd5CVarRx;
    active = 2;
    CardCmdSector = parameter;
    CardCmdBuffer = buffer;
    CardCmdResultPtrA = first;
    CardCmdCount = 0;
    CardCmdParam.value = 0;
    CardCommandStatus = active;
    return result;
}
