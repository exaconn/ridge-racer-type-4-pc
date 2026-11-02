typedef struct {
    short x, y;
} V;

extern int func_8001FEC0();
extern int func_8002071C();
extern int func_8001FCC8();
extern void func_80022694();

extern short D_800F4D44;
extern unsigned char D_8009F71C[][4];

void func_800349F4(int a0, int a1) {
    V v[4];
    unsigned char rgb[4];
    int p;
    int i, j;
    unsigned char (*base)[4];
    unsigned char *t;

    p = func_8001FEC0(a0, *(int *)0x1F800000, 0x90, 0x5C, 0x1C, 8, 0x60, 0xA0, 0x7FF8);
    p = func_8001FEC0(a0, p, 0x90, 0x6A, 0x20, 8, 0xC4, 0xA0, 0x7FF8);
    if (D_800F4D44 == 1) {
        p = func_8001FEC0(a0, p, 0x90, 0x74, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    } else {
        p = func_8001FEC0(a0, p, 0x90, 0x74, 0x24, 8, 0x9C, 0xA0, 0x7FF8);
        p = func_8001FEC0(a0, p, 0x90, 0x7E, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    }
    p = func_8001FEC0(a0, p, 0x88, a1 * 10 + 0x6A, 8, 8, 0xE4, 0xA0, 0x7FF8);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 4; j++) {
            int kx = ((j & 1) << 5) + 0x70;
            v[j].x = ((i % 3) << 5) + kx;
            {
                int ky = ((j >> 1) << 4) + 0x56;
                v[j].y = ((i / 3) << 4) + ky;
            }
        }
        rgb[0] = 0xFF;
        rgb[2] = 0x20;
        rgb[1] = 0x20;
        j = i / 3;
        j = j == 1;
        base = D_8009F71C;
        t = base[j];
        p = func_8002071C(a0, p, v[t[0]], v[t[1]], v[t[2]], rgb, 1);
        rgb[0] = 0xFF;
        rgb[2] = 0xFF;
        rgb[1] = 0xFF;
        p = func_8002071C(a0, p, v[t[1]], v[t[2]], v[t[3]], rgb, 1);
    }
    *(int *)0x1F800000 = func_8001FCC8(a0, p, 5);

    for (j = 0; j < 4; j++) {
        if (j == 1 || j == 2) {
            v[j].x = 0xD0;
        } else {
            v[j].x = 0x70;
        }
        if (j >= 2) {
            v[j].y = 0x86;
        } else {
            v[j].y = 0x56;
        }
    }

    for (i = 0; i < 3; i++) {
        j = 0x40;
        func_80022694(a0, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y, 0x100, 0, j, j, j, -1);
    }
}
