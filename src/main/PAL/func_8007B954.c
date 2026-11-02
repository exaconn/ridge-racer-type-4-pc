struct E { char pad[4]; unsigned char a; char b; unsigned char c; signed char d; };
extern struct E D_800A4258[];
extern unsigned char D_800A44A8[][4];
extern short D_800AC594;
extern short D_800F3A1C;
extern unsigned char D_800F3A24[];
extern char D_800F3A38[];
extern char *D_800F4D80;
extern unsigned short D_800F4D8A;
extern short D_800FFC00;
extern unsigned short D_80109458;
extern short D_80109478[];
extern short D_8010948E;
extern int func_8002B330();
extern int func_8004F24C();

void func_8007B954()
{
    int a, s4, s3;
    /* ASM_BARRIER: pin q to a0 ($4) to reproduce original register allocation */
    register unsigned char *q asm("$4");
    unsigned char *r, *r2;
    char *b3;
    unsigned char *b2;
    unsigned char (*tb)[4];
    int t, c;
    int ia;
    struct E *base;
    struct E *p;
    signed char sb;

    base = D_800A4258;
    p = base + D_800F3A1C;
    q = (unsigned char *)(int)p->a;
    c = p->c;
    D_8010948E = c;
    if (D_800F4D8A == 0) {
        sb = p->d;
        if (sb >= 0) {
            D_80109478[c] = sb;
        }
        D_800F4D8A = 1;
    }
    tb = D_800A44A8;
    ia = (int)q * 4;
    b2 = D_800F3A24;
    r = b2 + D_800FFC00;
    q = (unsigned char *)tb + ia;
    a = q[0];
    s4 = q[1];
    s3 = q[2];
    if (*r < 0x10 || a == *r) {
        if (D_800F4D80[0x56C] == 2) {
            func_8002B330(((unsigned short)D_800AC594 & 1) << 2, 2, D_8010948E * 2 + (int)D_80109478, 2, 0);
            b3 = D_800F3A38;
        r2 = (unsigned char *)b3 + D_800AC594 * 0x5C;
            if (*(int *)(r2 + 0x28) != 0) {
                func_8004F24C(1);
                if (D_80109478[D_8010948E] == 0) {
                    D_800F3A1C = s4;
                } else {
                    D_800F3A1C = s3;
                }
                D_800F4D8A = 0;
            } else if (*(int *)(r2 + 0x2C) != 0) {
                func_8004F24C(2);
                D_800F3A1C = s3;
                D_800F4D8A = 0;
            }
        }
    } else {
        if (*r == 0x32) return;
        if (*r != 0x14) return;
        t = D_800F3A1C;
        if (t == 2 || t == 0x1A || t == 0x2D || t == 0x3C) {
            D_80109458 = D_800F3A1C;
            D_800F3A1C = 8;
        } else if (t == 0x10 || t == 0x23) {
            D_800F3A1C = 0x12;
        } else if (t == 0x34) {
            D_800F3A1C = 0x36;
        }
        D_800F4D8A = 0;
    }
}
