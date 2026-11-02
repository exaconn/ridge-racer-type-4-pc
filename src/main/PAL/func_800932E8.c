/* CC1_FLAGS: -mno-split-addresses */
extern char *D_800ABAB8;
void func_800932E8(char *a0)
{
    int t = a0[3];
    (*(int (**)())(D_800ABAB8 + 0x3C))(0);
    (*(int (**)())(D_800ABAB8 + 0x14))(a0 + 4, t);
}
