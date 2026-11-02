extern int func_8001E84C(), func_8008E780(), func_80096414(), func_80096444();
extern char D_80011BC4[], D_80011BF0[], D_80011BF8[], D_80011C28[], D_80011C4C[], D_80011C6C[], D_80011C80[];
extern int D_800A6490, D_800A67A0;
struct N { int a; int b; int c; char name[32]; };
extern struct N D_80110950[];
extern unsigned char D_80111F50[];
int func_8008E17C() {
    int sp18;
    unsigned char *s1;
    int a3;
    int v;
    int s0, s3;
    char *s2;
    unsigned char *end;
    v = func_8008E780(1, 0x10, D_80111F50);
    if (v != 1) {
        if (D_800A6490 > 0) { func_8001E84C(D_80011BC4); return 0; }
        return 0;
    }
    if (func_80096414(D_80111F50 + 1, D_80011BF0, 5) != 0) {
        if (D_800A6490 > 0) { func_8001E84C(D_80011BF8); return 0; }
        return 0;
    }
    sp18 = *(int *)(D_80111F50 + 0x8C);
    if (func_8008E780(1, sp18, D_80111F50) != v) {
        if (D_800A6490 > 0) { func_8001E84C(D_80011C28, sp18); return 0; }
        return 0;
    }
    s1 = D_80111F50;
    if (D_800A6490 >= 2) func_8001E84C(D_80011C4C);
    end = D_80111F50 + 0x800;
    a3 = 0;
    if (s1 < end) {
        for (;;) {
            if (s1[0] == 0) break;
            s0 = a3 * 0x2C;
            *(int *)((char *)D_80110950 + s0 + 8) = *(int *)(s1 + 2);
            s2 = (char *)D_80110950 + s0 + 12;
            s3 = a3 + 1;
            *(int *)((char *)D_80110950 + s0) = s3;
            *(int *)((char *)D_80110950 + s0 + 4) = s1[6];
            func_80096444(s2, s1 + 8, s1[0], a3);
            s2[s1[0]] = 0;
            s1 += s1[0] + ((s1[0] & 1) + 8);
            if (D_800A6490 >= 2)
                func_8001E84C(D_80011C6C, *(int *)((char *)D_80110950 + s0 + 8), *(int *)((char *)D_80110950 + s0), *(int *)((char *)D_80110950 + s0 + 4), s2);
            a3 = s3;
            if (a3 >= 0x80) goto done;
            if (s1 >= end) break;
        }
    }
    if (a3 < 0x80) *(int *)((char *)D_80110950 + a3 * 0x2C + 4) = 0;
done:
    D_800A67A0 = 0;
    if (D_800A6490 >= 2) func_8001E84C(D_80011C80, a3);
    return 1;
}
