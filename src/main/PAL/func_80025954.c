typedef struct { char pad[0x24]; int s24; char p1[0x74 - 0x28]; int s74; char p2[0x86 - 0x78]; short s86; char p3[0x9C - 0x88]; int s9C; int sA0; int sA4; char p4[0xAC - 0xA8]; short sAC; short sAE; } Q;
typedef struct {
    char p0[0x30]; int s30; char p1[4]; int s38; char p2[0x54 - 0x3C]; int s54;
    char p3[0xE4 - 0x58]; short sE4; short sE6; char p4[0x1D4 - 0xE8]; short s1D4; char p5[2]; unsigned short s1D8;
} P;
extern char D_800ABEF8[];
extern int *D_800F3A08[];
extern short D_800F4D44;
extern int D_800F2DE4;
extern int func_80025FBC();
extern int func_80090424();
extern int func_800904F4();
extern int func_80050E94();
extern int func_8004B7E0();
extern int func_80096464();

void func_80025954(a0) P *a0; {
    Q *q = (Q *)((char *)a0 + 0x1F4);
    int t, a1, a2, a3, v;
    int s0, s2;
    int pad[4];
    t = func_80025FBC(a0->s54, q->s9C);
    v = t / 4;
    a1 = a0->s54 + v;
    a0->s54 = a1;
    t = func_80025FBC(a0->s1D4, a1);
    a0->s1D4 = a0->s1D4 + t / 10;
    a0->s1D8 = (short)a0->s1D8 / 10;
    a0->s30 = ((short)a0->s1D8 * func_80090424(a0->s1D4)) >> 8;
    a0->s38 = ((short)a0->s1D8 * func_800904F4(a0->s1D4)) >> 8;
    a2 = q->s74;
    if (a2 >= 0x3D) a2 = 0x3C;
    else if (a2 < 0) a2 = 0;
    a2 = (a2 << 1) * q->sAC;
    if (a2 < 0) a2 += 0xFF;
    a2 = a2 >> 8;
    if (q->s74 > 0) {
        if ((char *)a0 == D_800ABEF8 || (char *)a0 == D_800ABEF8 + 0x320) {
            a3 = q->s24;
            if (*((unsigned char *)D_800F3A08[a3] + 0xA) == 1) a2 = 0;
            func_80050E94(1, 0x1A80, a2, a3 & 1);
            func_8004B7E0(q->s24 & 1, 2, 1);
        }
        q->s74 = q->s74 - 0x30;
        q->s74 = q->s74 - q->sAE * 2;
        if (q->sAC < 0x80) q->s74 = q->s74 - 0x30;
        if (D_800F4D44 != 4) {
            s2 = q->s74 >> 10;
            s0 = q->s9C;
            if ((func_80096464() & 3) >= 2) q->s9C = s0 + s2;
            else q->s9C = s0 - s2;
            s0 = a0->sE4;
            if (func_80096464() & 1) s0 += 0x10;
            a0->sE4 = s0;
            s0 = a0->sE6;
            if (func_80096464() & 1) s0 += 0x10;
            a0->sE6 = s0;
        } else {
            int c;
            int z, w, u;
            int x;
            int y;
            y = q->s74;
            x = y >> 10;
            c = (unsigned)(D_800F2DE4 & 3) < 2;
            y = q->s9C;
            if (c) y = y - x; else y = y + x;
            z = D_800F2DE4 & 1;
            q->s9C = y;
            u = a0->sE4;
            if (z) u += 0x10;
            w = a0->sE6;
            a0->sE4 = u;
            if (z) w += 0x10;
            a0->sE6 = w;
        }
        if (q->s86 >= 2) {
            a0->s1D8 = 0;
            q->sA0 = 0;
            q->s74 = 0;
        }
    } else {
        q->sA4 = 0;
        if ((char *)a0 == D_800ABEF8 || (char *)a0 == D_800ABEF8 + 0x320) {
            func_80050E94(0, 0, 0, q->s24 & 1);
            func_8004B7E0(q->s24 & 1, 0, 1);
        }
    }
}
