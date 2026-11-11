#include "rr/commands.h"
#include "psyq/libcard/cardc03.h"
#include "psyq/libcard/cardc02.h"

s32 _cardCmd59ParamBegin(s32 request, s32 parameter, u8 *buffer);

s32 _cardCmd59Rx(s32 value) {
    s32 result;
    switch(CardRxCount) {
    case 3: CardBytesRemaining=value & 255; break;
    case 4: *(u32 *)CardCmdResultPtrA=value & 255; break;
    case 5: *(u32 *)CardCmdResultPtrA+=(value & 255) * 0x100U; break;
    }
    result=0;
    return result;
}

s32 _cardCmd59ParamBegin(s32 request, s32 parameter, u8 *buffer) {
    s32 mode;
    if (parameter >= 0) {
        if (parameter >= 16) return 0;
        {
            s32 *base;
            /* PAL mapping: base[0x4724 / 4] is CardCmdSector. */
            base = (s32 *)0x80110000U;
            base[0x4724 / 4] = parameter;
        }
    } else CardCmdSector = -1;
    if (!rr_command_begin(request)) return 0;
    mode = 2;
    CardCommandReader = _cardCmd59ParamTx;
    CardCmdParam.buffer = buffer;
    CardCmdBuffer = buffer;
    CardCurrentCommand = mode;
    CardWriteHandler = _cardRxStatusByte;
    CardCommandStatus = mode;
    return 1;
}
