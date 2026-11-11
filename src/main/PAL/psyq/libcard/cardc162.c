/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/commands.h"
#include "psyq/libcard/cardc163.h"
#include "psyq/libcard/cardc162.h"

s32 _cardCmd5ABegin(s32 request, u8 *buffer);

s32 _cardCmd58Rx(s32 value) {
    s32 result;
    switch(CardRxCount) {
    case 3: CardBytesRemaining=value & 255; break;
    case 4: *(u32 *)CardCmdResultPtrA=value & 255; break;
    case 5: *CardCmdResultPtrB=value & 255; break;
    }
    result=0;
    return result;
}

s32 _cardCmd5ABegin(s32 request, u8 *buffer) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    CardCurrentCommand = 12;
    CardCommandReader = _cardCmd5ATx;
    CardWriteHandler = _cardCmd5ARx;
    active = 2;
    CardCmdBuffer = buffer;
    CardCommandStatus = active;
    return result;
}
