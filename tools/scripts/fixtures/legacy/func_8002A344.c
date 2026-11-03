typedef struct { unsigned char c[4]; } B4;
typedef struct { unsigned char a, b, c, d; } S;
extern S D_800ACC68;
extern B4 D_8009EFD0[];
extern char *D_800ACC14;
extern short D_800F4CB0;
extern short D_800F4D2C;
extern int func_8001FDD4();
extern int func_8002A63C();
extern int func_8005B2D4();
extern int func_8005B304();
extern int SetDispMask();

void func_8002A344(a0) int a0; {
    B4 sp;
    int a, t, a1;
    S *st = &D_800ACC68;
    *(D_800ACC14 + 0x18) = 0;
    a = (a0 >> 1) & 1;
    st->c = a;
    if (a != st->d) D_800ACC68.a = 0;
    st->d = a;
    t = st->b;
    switch (D_800ACC68.a) {
    case 0:
        sp = D_8009EFD0[a ^ 1];
        st->b = 0;
        D_800ACC68.a = 1;
        break;
    case 1:
        {
            B4 *p = &D_8009EFD0[st->c];
            B4 *q = &D_8009EFD0[st->c ^ 1];
            a1 = 0x1E - t;
            sp.c[0] = (p->c[0] * t + q->c[0] * a1) / 30;
            sp.c[1] = (p->c[1] * t + q->c[1] * a1) / 30;
            sp.c[2] = (p->c[2] * t + q->c[2] * a1) / 30;
        }
        if (t >= 0x1E) {
            D_800ACC68.a = 2;
            st->b = 0;
        } else {
            if (t == 2) {
                if (D_800F4CB0 == 0) SetDispMask(1);
            }
            D_800ACC68.b = D_800ACC68.b + 1;
        }
        break;
    case 2:
        sp = D_8009EFD0[st->c];
        break;
    }
    {
        int t = *(int *)0x1F800000;
        *(int *)0x1F800000 = func_8001FDD4(D_800ACC14 + 0xB6C, t, 0, 0, 0x140, 0x200, sp.c[0], sp.c[1], sp.c[2]);
    }
    func_8002A63C(a0 >> 2);
    if (D_800F4D2C != 6 && D_800F4D2C != 8) {
        func_8005B2D4(a0 & 1);
        func_8005B304();
    }
}
