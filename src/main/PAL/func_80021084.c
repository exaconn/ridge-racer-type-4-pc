/* RODATA 0x800103e0 0x1c */
struct W3G0A { char *p0; char *p4; short s8, sA, sC, sE, s10, s12; char *p14; char *p18; short s1C, s1E, s20, s22, s24, s26; };
extern struct W3G0A D_8010BF88;
extern char D_800ADB60[], D_800D02D8[], D_800AE660[], D_800D0DD8[];
extern short D_80107178;
void func_80021084(int a0) {
    switch (a0) {
    case 0: case 1: case 5: break;
    case 2: case 3: case 4: case 6:
        if (a0 == 4 && D_80107178 == 2) {
        } else if (a0 != 6) {
            goto w3g0b;
        }
    }
    D_8010BF88.p0 = D_800ADB60;
    D_8010BF88.p4 = D_800D02D8;
    D_8010BF88.s8 = 0x106;
    D_8010BF88.sA = 0xE6;
    D_8010BF88.sC = 0x122;
    D_8010BF88.sE = 0xDC;
    D_8010BF88.s10 = 0x112;
    D_8010BF88.s12 = 0xCE;
    return;
w3g0b:
    D_8010BF88.p0 = D_800AE660;
    D_8010BF88.p4 = D_800D0DD8;
    D_8010BF88.s8 = 0xFC;
    D_8010BF88.sA = 0x58;
    D_8010BF88.sC = 0x128;
    D_8010BF88.sE = 0x4A;
    D_8010BF88.s10 = 0xFC;
    D_8010BF88.s12 = 0x68;
    D_8010BF88.p14 = D_800AE660;
    D_8010BF88.p18 = D_800D0DD8;
    D_8010BF88.s1C = 0xFC;
    D_8010BF88.s1E = 0xD8;
    D_8010BF88.s20 = 0x122;
    D_8010BF88.s22 = 0xCA;
    D_8010BF88.s24 = 0xFC;
    D_8010BF88.s26 = 0xE8;
}
