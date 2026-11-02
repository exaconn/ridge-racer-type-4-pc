typedef struct { int a[8]; } M;
typedef struct { unsigned short h[16]; } MH;
extern int func_8006E030();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090914();
extern int func_80090DB4();

void func_80077A34(int unused, short *p, M *out, int flag) {
    M m10, m30, m50, m70;
    M m90;
    M *q = &m70;
    func_8006E030(q, -p[2]);
    func_8006E108(&m30, -p[0]);
    if (flag == 2) {
        m90 = *(M *)0x1F800028;
        func_8006E09C(&m50, -p[1] - 0x800);
        ((unsigned short *)&m90)[6] = -((unsigned short *)&m90)[6];
        ((unsigned short *)&m90)[7] = -((unsigned short *)&m90)[7];
        ((unsigned short *)&m90)[8] = -((unsigned short *)&m90)[8];
        func_80090DB4(&m90, q);
    } else {
        func_8006E09C(&m50, -p[1]);
        func_80090DB4((int *)0x1F800028, q);
    }
    func_80090914(&m30, &m50, &m10);
    func_80090DB4(&m70, &m10);
    *out = m10;
}
