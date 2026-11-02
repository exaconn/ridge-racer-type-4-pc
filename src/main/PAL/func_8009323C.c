/* CC1_FLAGS: -mno-split-addresses */
extern char D_80011F20[];
extern char *D_800ABAB8;
extern int (*D_800ABABC)();
extern unsigned char D_800ABAC2;
extern char D_800ABB6C[];
extern int D_800ABB80;
unsigned *func_8009323C(unsigned *a, int b) {
    int *q;
    if (D_800ABAC2 >= 2) D_800ABABC(D_80011F20, a, b);
    (*(int (**)())(D_800ABAB8 + 0x2C))(a, b);
    q = &D_800ABB80;
    *q = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *a = (int)q & 0xFFFFFF;
    return a;
}
