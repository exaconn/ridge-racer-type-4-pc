/* CC1_FLAGS: -mno-split-addresses */
extern int func_80092DB8();
extern char D_80011EF0[];
extern char *D_800ABAB8;
void func_8009305C(int a, int b) {
    func_80092DB8(D_80011EF0, a);
    (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0x1C), a, 8, b);
}
