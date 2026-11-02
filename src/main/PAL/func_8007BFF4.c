/* RODATA 0x8001160C 0x20 */
struct E { char pad[4]; unsigned char a; char b; unsigned char c; signed char d; };
extern struct E D_800A4258[];
extern unsigned char D_800A4524[][4];
extern short D_800AC594;
extern short D_800F3A1C;
extern unsigned char D_800F3A24[];
extern char D_800F3A38[];
extern unsigned short D_800F3AF8;
extern unsigned short D_800F4D8A;
extern unsigned short D_800FFC18;
extern short D_800FFC00;
extern short D_80109478[];
extern short D_8010948E;
extern short D_801094A0;
extern int func_8002B330();
extern int func_8004F24C();
extern int func_80078E40();

void func_8007BFF4()
{
    int arr[4];
    int i, c, flag, n;
    struct E *base, *p;
    unsigned char *q;
    char *r, *b3;
    signed char sb;

    D_801094A0 = 0;
    D_800F3AF8 = D_800FFC18;
    switch ((short)D_800F3AF8) {
    case 0: D_800F3A1C = 0; break;
    case 1: case 5: D_800F3A1C = 0x20; break;
    case 2: D_800F3A1C = 0x21; break;
    case 3: D_800F3A1C = 0x18; break;
    case 6: D_800F3A1C = 0x2A; D_800F3AF8 = 0x10; break;
    case 7: D_800F3A1C = 0x2B; D_800F3AF8 = 0x10; break;
    }
    base = D_800A4258;
    p = base + D_800F3A1C;
    i = p->a;
    c = p->c;
    D_8010948E = c;
    if (D_800F4D8A == 0) {
        sb = p->d;
        if (sb >= 0) {
            D_80109478[c] = sb;
        }
        D_800F4D8A = 1;
    }
    flag = 0;
    q = D_800A4524[i];
    i = 0;
    arr[0] = q[0];
    arr[1] = q[1];
    arr[2] = q[2];
    arr[3] = q[3];
    n = 4;
    for (; i < 4; i++) {
        if (arr[i] == 0xFF) { n = i; break; }
        if (i == 1 && (arr[1] & 0x80)) {
            flag = 1;
            arr[1] ^= 0x80;
        }
    }
    if (flag) {
        func_8002B330(((unsigned short)D_800AC594 & 1) << 2, 2, D_80109478 + D_8010948E, n, 0);
    } else {
        func_8002B330((((unsigned short)D_800AC594 & 1) << 2) | 1, 2, D_80109478 + D_8010948E, n, 0);
    }
    b3 = D_800F3A38;
    r = b3 + D_800AC594 * 0x5C;
    if (*(int *)(r + 0x28) != 0) {
        func_8004F24C(1);
        D_800F4D8A = 0;
        D_800F3A1C = arr[D_80109478[D_8010948E]];
        if (D_800F3A1C != 0x43) {
            D_800F3A24[D_800FFC00] = 0x5A;
            func_80078E40(D_800FFC00, 0, 1);
        }
    } else if (*(int *)(r + 0x2C) != 0) {
        func_8004F24C(2);
        D_800F4D8A = 0;
        D_800F3A1C = arr[n - 1];
    }
}
