#include "rr/commands.h"
#include "psyq/libcard/cardc09.h"
#include "psyq/libcard/cardc10.h"

s32 _cardCmd5BVarBegin(s32 request, s32 parameter, u8 *buffer, u8 *first) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    CardCurrentCommand = 14;
    CardCommandReader = _cardCmd5BVarTx;
    CardWriteHandler = _cardCmd5BVarRx;
    active = 2;
    CardCmdSector = parameter;
    CardCmdBuffer = buffer;
    CardCmdResultPtrA = first;
    CardCmdCount = 0;
    CardCmdParam.value = 0;
    CardCommandStatus = active;
    return result;
}
