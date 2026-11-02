extern int func_8004C5A4();
extern unsigned char D_800A017C[];
extern unsigned char D_800A01DC[];
void func_8004C894(int a0, int a1, unsigned char *s, int a3, int a4) {
    int sum = 0;
    unsigned char *s0 = s;
    if (*s0 != 0) {
        do {
            unsigned char c = D_800A017C[*s - 0x20];
            s++;
            if (c != 0xFF) {
                sum += D_800A01DC[c];
            } else {
                sum += 4;
            }
        } while (*s != 0);
    }
    func_8004C5A4(a0 - sum, a1, s0, a3, a4);
}
