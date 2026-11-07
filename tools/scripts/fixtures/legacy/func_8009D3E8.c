/* GCC_VERSION: 2.7.2 */
extern int D_800ABDE8, CardCmdSector;
int _cardCmd5FTx() {
    switch (D_800ABDE8) {
    case 2: return 0x5F;
    case 4: return CardCmdSector;
    }
    return 0;
}
