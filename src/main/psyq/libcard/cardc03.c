#include "rr/commands.h"
#include "psyq/libcard/cardc03.h"

int _cardCmd59ParamTx() {
    switch (CardTxCount) {
    case 2: return 0x59;
    case 4: return D_80114725;
    case 5: return *(u8 *)&CardCmdSector;
    }
    if (CardTxCount < 6) return 0;
    if (CardTxCount >= 10) return 0;
    return CardCmdBuffer[CardTxCount - 6];
}

int _cardRxStatusByte(a0) int a0; { if (CardRxCount == 3) CardBytesRemaining = a0 & 0xFF; return 0; }
