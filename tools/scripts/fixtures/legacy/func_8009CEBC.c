/* GCC_VERSION: 2.7.2 */
extern volatile int D_800ABDE8;
extern int CardCmdSector, D_80114728, D_8011472C;
extern unsigned char *CardCmdBuffer;

int _cardCmd5CVarTx() {
    switch (D_800ABDE8) {
    case 2: return 0x5C;
    case 3: return CardCmdSector;
    }
    if (D_800ABDE8 < 5) return 0;
    if (D_8011472C == 0) {
        if (D_800ABDE8 >= D_80114728 + 5) return 0;
        return CardCmdBuffer[D_800ABDE8 - 5];
    }
    return CardCmdBuffer[D_800ABDE8 - D_80114728 - 6];
}
