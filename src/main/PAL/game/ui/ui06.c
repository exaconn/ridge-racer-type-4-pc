typedef struct {
    char f0, f1, f2, f3;
    short x, y;
    unsigned short u, v, cx, cy;
    unsigned char b0, b1;
    char pad[2];
} E;

extern int D_800ACC14;
extern E *D_800F2AB0;

extern unsigned short func_80095A24();
extern int func_80020074();
extern int func_8002039C();

void func_8003FE78(int a0, int a1, int a2, int a3) {
    int i, j, k;
    int tx, ty;
    int x[4], y[4], u[4], v[4];
    unsigned short tpage, clut;
    int s2;

    s2 = *(int *)0x1F800000;
    for (i = 0; i < 18; i++) {
        tx = D_800F2AB0[a0].u + (i % 9) * 24;
        ty = D_800F2AB0[a0].v + (i / 9) * 96;
        tpage = ((ty & 0x100) >> 4) | 0x80 | ((tx & 0x3FF) >> 6);
        clut = func_80095A24(D_800F2AB0[a0].cx, D_800F2AB0[a0].cy + i / 2);
        for (j = 0; j < 4; j++) {
            if (a1 == 0) {
                u[0] = (tx % 64) * 2 + (j & 1) * 24;
                v[0] = (i / 9) * 96;
                if (j >= 2) {
                    v[0] += 48;
                }
                x[0] = D_800F2AB0[a0].x + (i % 6) * 48 + (j & 1) * 24;
                y[0] = D_800F2AB0[a0].y + (i / 6) * 96;
                if (j >= 2) {
                    y[0] += 48;
                }
                s2 = func_80020074(D_800ACC14 + 0xB48, s2, x[0], y[0], 24, 48, u[0], v[0], tpage, clut, a3, a2);
            } else if (a1 == 1) {
                for (k = 0; k < 4; k++) {
                    x[k] = D_800F2AB0[a0].x + (i % 6) * 48 + (k & 1) * 48;
                    y[k] = D_800F2AB0[a0].y + (i / 6) * 96 + j * 24;
                    if (k >= 2) {
                        y[k] += 24;
                    }
                    u[k] = (tx % 64) * 2 + (k & 1) * 48;
                    v[k] = (i / 9) * 96 + j * 24;
                    if (k >= 2) {
                        v[k] += 24;
                    }
                }
                y[0] = -(a2 * 12) / 128 + y[0] + 12;
                y[1] = -(a2 * 12) / 128 + y[1] + 12;
                y[2] = (a2 * 12) / 128 + y[2] - 12;
                y[3] = (a2 * 12) / 128 + y[3] - 12;
                s2 = func_8002039C(D_800ACC14 + 0xB48, s2, &x[0], &y[0], &u[0], &v[0], tpage, clut, 0, a2);
                s2 = func_8002039C(D_800ACC14 + 0xB48, s2, &x[1], &y[1], &u[1], &v[1], tpage, clut, 0, a2);
            }
        }
    }
    *(int *)0x1F800000 = s2;
}
