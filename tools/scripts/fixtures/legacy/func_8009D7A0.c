/* CC1_FLAGS: -mno-split-addresses */
extern volatile int D_800ABDE8;
int func_8009D7A0() {
 register int r asm("$2") = 0x5A; int d; d = D_800ABDE8; if (d != 2) r = 0; return r;
}
