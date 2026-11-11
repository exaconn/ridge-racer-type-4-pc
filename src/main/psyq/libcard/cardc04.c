/* RODATA 0x80012158 0x20 */
#include "rr/commands.h"
#include "psyq/libcard/cardc07.h"
#include "psyq/libcard/cardc03.h"
#include "psyq/libcard/cardc04.h"

extern s32 _cardValidateSector(s32, s32);

s32 _cardCmd5BBegin(s32 request, u8 *buffer);
int _cardCmd5BTx();
s32 _cardCmd5BRx(s32 value);
s32 _cardCmd5BRangeBegin(s32 request, u8 *buffer, s32 sector, s32 input_count);
s32 _cardCmd5CRangeBegin(s32 request, u8 *buffer, s32 sector, s32 input_count);

s32 _cardCmd5BBegin(s32 request, u8 *buffer) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    CardCurrentCommand = 3;
    CardCommandReader = _cardCmd5BTx;
    CardWriteHandler = _cardCmd5BRx;
    active = 2;
    CardCmdBuffer = buffer;
    CardCommandStatus = active;
    return result;
}

int _cardCmd5BTx() {
    int r = 2;
    int d = CardTxCount;
    if (d == r) r = 0x5B;
    else r = 0;
    return r;
}

s32 _cardCmd5BRx(s32 value) {
    u8 v=value;
    s32 phase=CardRxCount;
    if(phase==5) CardBytesRemaining=v;
    else if(phase==10) CardCmdBuffer[4]=v-1U;
    else if(phase>=6) CardCmdBuffer[phase-6]=v;
    return 0;
}

s32 _cardCmd5BRangeBegin(s32 request, u8 *buffer, s32 sector, s32 input_count) {
    s32 result;
    s32 active;
    s32 count = input_count;
    if (!_cardValidateSector(0, sector)) return 0;
    if (!_cardValidateSector(0, ((u32)sector + count - 1))) return 0;
    if (!rr_command_begin(request)) return 0;
    CardCurrentCommand = 4;
    CardCommandReader = _cardCmd5BRangeTx;
    CardWriteHandler = _cardCmd5BRangeRx;
    active = 2;
    CardCmdBuffer = buffer;
    CardCmdSector = sector;
    CardCmdCount = count;
    CardCommandStatus = active;
    return result;
}

int _cardCmd5BRangeTx() {
    switch (CardTxCount) {
    case 2: return 0x5B;
    case 3: return 1;
    case 5: return *(u8 *)&CardCmdSector;
    case 6: return D_80114725;
    case 7: return D_80114726;
    case 8: return D_80114727;
    case 9: return CardCmdCount;
    }
    return 0;
}

s32 _cardCmd5BRangeRx(s32 value) {
    s32 phase=CardRxCount;
    s32 result;
    if(phase==9) CardBytesRemaining=CardCmdCount+1U;
    else if(phase>=11) CardCmdBuffer[phase-11]=value;
    result=0;
    return result;
}

s32 _cardCmd5CRangeBegin(s32 request, u8 *buffer, s32 sector, s32 input_count) {
    s32 result;
    s32 active;
    s32 count = input_count;
    if (!_cardValidateSector(1, sector)) return 0;
    if (!_cardValidateSector(1, ((u32)sector + count - 1))) return 0;
    if (!rr_command_begin(request)) return 0;
    CardCurrentCommand = 5;
    CardCommandReader = _cardCmd5CRangeTx;
    CardWriteHandler = _cardCmd5CRangeRx;
    active = 2;
    CardCmdBuffer = buffer;
    CardCmdSector = sector;
    CardCmdCount = count;
    CardCommandStatus = active;
    return result;
}
