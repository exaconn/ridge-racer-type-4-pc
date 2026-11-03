/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
extern char D_80011F20[];
extern char *D_800ABAB8;
extern int (*D_800ABABC)();
extern unsigned char D_800ABAC2;
extern char D_800ABB6C[];
extern int D_800ABB80;

unsigned *func_8009323C(unsigned *a, int b);
void func_800932E8(char *a0);

unsigned *func_8009323C(unsigned *a, int b) {
    int *q;
    if (D_800ABAC2 >= 2) D_800ABABC(D_80011F20, a, b);
    (*(int (**)())(D_800ABAB8 + 0x2C))(a, b);
    q = &D_800ABB80;
    *q = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *a = (int)q & 0xFFFFFF;
    return a;
}

void func_800932E8(char *a0)
{
    int t = a0[3];
    (*(int (**)())(D_800ABAB8 + 0x3C))(0);
    (*(int (**)())(D_800ABAB8 + 0x14))(a0 + 4, t);
}
