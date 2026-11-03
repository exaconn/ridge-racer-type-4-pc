typedef struct {
    short s0;
    short s2;
    short s4;
    short s6;
    char pad[0x14];
} S_800F3AFC;

extern S_800F3AFC *D_800F3AFC;
extern int D_800AD564;
extern int D_8009FE10[];
extern int func_8001FF84();
extern int func_8001FCC8();

void func_80042F58(int a0, int a1, int a2) {
    char sp[8];
    int s4;
    int s2;
    int i;
    int off;
    int rem;

    s4 = D_800F3AFC[a2].s4 + 0x10;
    s2 = D_800F3AFC[a2].s6 + 0x20;
    rem = D_800AD564 % 5;
    a1 = func_8001FF84(a0, a1, s4, s2, &D_8009FE10[rem], 0x4424, 0);
    for (i = 0; i < 2; i++) {
        off = i * 0x28 - 8;
        a1 = func_8001FF84(a0, a1, s4 + off, s2, &D_8009FE10[i + 5], 0x4424, 0);
    }
    func_8001FCC8(a0, a1, 0x39);
}
