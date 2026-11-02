extern int func_80091AD4();
extern int func_800910B4();
extern int func_80090C74();
extern short D_800ABED0[][3];
extern char D_800F38F8[][8];
extern unsigned short D_800AD5BC[];

void func_8006EED8(void) {
    int i;
    int m[8];
    struct { short a, b, c, d; } s;
    struct { short x, p0, y, p1, z, p2, q[2]; } r;
    s.b = 0;
    s.a = 0;
    for (i = 0; i < 3; i++) {
        s.c = -D_800AD5BC[i];
        func_80091AD4(D_800F38F8[i], m);
        func_800910B4(m);
        func_80090C74(&s, &r);
        D_800ABED0[i][0] = r.x;
        D_800ABED0[i][1] = r.y;
        D_800ABED0[i][2] = r.z;
    }
}
