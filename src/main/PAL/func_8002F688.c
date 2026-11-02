typedef struct { int a; short b; char c; char d; } S8;
typedef struct { int x; } __attribute__((packed)) U4;
extern int func_80096434(), func_80032E9C();
extern unsigned char D_8009F06C[];
extern short D_800ABEC8, D_800AC648, D_800AD520, D_800F3826, D_800F3914, D_800F4C28;
extern signed char D_800AD4D8[], D_800AD4EF;
extern unsigned char D_800AD4F0, D_800F3981;
extern S8 D_800F2A68[];
extern unsigned char D_800F4D98[];
extern int D_800F3920[];
void func_8002F688() {
    int x, c0, i0, j;
    int a0;
    register int v1;
    int a2;
    char *s0, *p;
    a2 = (unsigned char)D_800AD4D8[0x17];
    i0 = (signed char)a2;
    if (i0 != -1) {
        if (D_800AD4D8[0x19] <= D_8009F06C[i0 / 2]) {
            if (D_800AD4D8[i0] != -1) {
                D_800AD4D8[0x17] = a2 + 1;
            }
        }
    }
    a0 = D_800AD4EF;
    if (a0 == -1) {
        func_80096434(D_800F2A68, 0x40);
        j = 0;
        do {
            D_800F2A68[j].c = j;
            D_800F2A68[j].b = j;
            j++;
        } while (j < 8);
        v1 = 0;
        func_80096434(D_800F4D98, 0x20);
        func_80096434(D_800AD4D8, 0x2C);
        a0 = 0;
        v1 = 0;
        a2 = -1;
        *(U4 *)&D_800AD4D8[0x1C] = *(U4 *)&D_800F4D98[0];
        D_800ABEC8 = 0;
        do {
            p = (char *)D_800AD4D8 + v1;
            v1++;
            p[0] = a2;
            p[8] = 3;
        } while (v1 < 8);
        D_800AD4F0 = D_800F3981;
    }
    x = (unsigned char)D_800AD4D8[0x17];
    x = (x << 24) >> 25;
    D_800AD4D8[0x15] = x;
    if ((unsigned)(x & 0xFF) >= 3) {
        D_800AD4D8[0x15] = 2;
    }
    *(U4 *)&D_800F4D98[0] = *(U4 *)&D_800AD4D8[0x1C];
    D_800AC648 = a0;
    c0 = D_800F4D98[0];
    D_800AD520 = c0;
    D_800F3826 = D_800F4D98[1];
    D_800F4C28 = D_800F4D98[2];
    D_800F3914 = func_80032E9C(a0, c0, a2);
    D_800ABEC8 = 0;
    D_800F3920[3] |= 1 << D_800AC648;
    if (D_800F4D98[2] >= 5) {
        D_800ABEC8 = (D_800F4D98[2] - 5) % 7;
        return;
    }
    if (D_800F4D98[2] != 0) {
        D_800ABEC8 = D_800F4D98[2] - 1;
    }
}
