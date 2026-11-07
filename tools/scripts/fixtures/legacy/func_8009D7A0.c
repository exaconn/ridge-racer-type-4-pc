/* CC1_FLAGS: -mno-split-addresses */
extern volatile int D_800ABDE8;
int _cardCmd5ATx() {
 register int r asm("$2") = 0x5A; int d; d = D_800ABDE8; if (d != 2) r = 0; return r;
}
