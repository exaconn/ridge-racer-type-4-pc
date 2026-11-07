/* GCC_VERSION: 2.7.2 */
extern volatile int D_800ABDE8;
extern unsigned char CardCmdSector, D_80114725;
extern unsigned char *CardCmdBuffer;

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
