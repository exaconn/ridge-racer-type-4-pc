/* GCC_VERSION: 2.7.2 */
#include "psyq/libcard/cardc03.h"
extern volatile int D_800ABDE8;
extern unsigned char CardCmdSector;
extern unsigned char *CardCmdBuffer;
extern int CardRxCount;
extern int D_80114730;

int _cardCmd59ParamTx();
int _cardRxStatusByte();

int _cardCmd59ParamTx() {
    switch (D_800ABDE8) {
    case 2: return 0x59;
    case 4: return D_80114725;
    case 5: return CardCmdSector;
    }
    if (D_800ABDE8 < 6) return 0;
    if (D_800ABDE8 >= 10) return 0;
    return CardCmdBuffer[D_800ABDE8 - 6];
}

int _cardRxStatusByte(a0) int a0; { if (CardRxCount == 3) D_80114730 = a0 & 0xFF; return 0; }
