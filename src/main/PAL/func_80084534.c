/* CC1_FLAGS: -mno-split-addresses */
extern int func_80084674();
extern void func_80088DA4(int unused_mode);
extern int D_800A48F8;
extern char D_8010F90A[];
extern char D_8010F90E[];
extern char D_8010F918[];
extern char D_8010F91A[];
extern char D_8010F91C[];
extern char D_8010F91E[];
extern signed char D_801100FC;
extern short D_80110118;

void func_80084534(void) {
    struct {
        int s10;
        int s14;
        short s18;
        short s1A;
        char pad1[0x24 - 0x1C];
        short s24;
        char pad2[0x2C - 0x26];
        int s2C;
        char pad3[0x4A - 0x30];
        unsigned short s4A;
        short s4C;
    } L;
    short s0;
    int v0, v1, a1;
    int c1, c18, cff;
    L.s14 = 0x60093;
    s0 = 0;
    L.s24 = 0x1000;
    L.s2C = 0x1000;
    L.s4A = 0x80FF;
    L.s18 = 0;
    L.s1A = 0;
    L.s4C = 0x4000;
    if (D_801100FC > 0) {
        c1 = 1;
        c18 = 0x18;
        cff = 0xFF;
        do {
            v1 = s0;
            a1 = c1 << v1;
            if ((D_800A48F8 & a1) == 0) {
                v0 = v1 * 0x36;
                *(short *)(D_8010F90A + v0) = c18;
                *(short *)(D_8010F90E + v0) = 0;
                *(short *)(D_8010F918 + v0) = cff;
                *(short *)(D_8010F91A + v0) = 0;
                *(short *)(D_8010F91C + v0) = 0;
                *(short *)(D_8010F91E + v0) = cff;
                L.s10 = a1;
                func_80084674(&L, a1);
                D_80110118 = s0;
                func_80088DA4(1);
            }
            s0++;
        } while (s0 < D_801100FC);
    }
}
