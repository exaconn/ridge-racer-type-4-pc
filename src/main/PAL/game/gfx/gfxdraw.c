/* RODATA 0x80010f00 0x1c */
typedef struct {
    int x, y, z, f0C;
    int f10, f14, f18, f1C;
    int f20, f24, f28, f2C;
    unsigned short f30;
    char pad32[0x12];
} CamNode;

typedef struct {
    char pad0[0x1C];
    int f1C;
    int f20, f24, f28, f2C;
    short f30, f32, f34, f36;
    char pad38[0x10];
    int f48, f4C;
    char pad50[0x500];
    CamNode *f550;
} Context650;

typedef struct { int x, y, z, w; } V4;

typedef struct { V4 a, b; } V8;

typedef struct { unsigned char a, b, c, d; } C4;

typedef struct { unsigned short a, b, c, d; } S4;

typedef struct { char p0[0x18]; C4 c[3]; S4 v[3]; unsigned char b3C, b3D, b3E; } A0;

extern Context650 D_800AC650;
extern int D_800AC698;
extern short D_800F4D44;
extern int D_8010718C;
extern void func_800172DC(int, int);
extern void func_80033BE4(void *);
extern int func_80043730(void *);
extern void func_800439BC(void *, short *, int *);
extern int func_80043C14(void *, CamNode *);
extern void func_80043DE4(void *, int *, CamNode *);
extern void func_80043ED4(unsigned short *, int, int *);
extern void func_80043F5C(unsigned short *, int, int *);
extern short func_80043FC0(CamNode *, int);
extern void func_8006E1AC(void);
extern int func_8006E368(int, int);
extern int func_80090758(int);
extern void func_800912D4(int);
extern int *D_800ABEA4;
extern int D_800AC548[];
extern int func_8006E030();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090914();
extern int func_80090CA4();
extern int func_80090DB4();
extern int func_80090F74();
extern int func_800910E4();
extern int func_80091114();
extern int func_80091274();
extern V4 D_80010F1C;
extern int func_800916D4();

void func_8004417C(int arg0, void *arg1);
void func_800448EC(void);
void func_80044920();
void func_80044B24(void);

void func_8004417C(int arg0, void *arg1) {
    int sp10[4];
    int sp20[4];
    unsigned short sp30[4];
    unsigned short sp38[4];
    CamNode *s2;
    int a0;
    int a1;
    int a2;
    int t1;
    int *scratch;

    s2 = D_800AC650.f550;
    a0 = func_80043730(arg1);
    scratch = (int *)0x1F800000;
    if (arg0 >= 2) {
        s2 = D_800AC650.f550;
        s2 += a0;
        D_8010718C = 0;
        arg0 = s2->f30 & 0xF;
    }
    D_800AC650.f1C = 0;
    a1 = D_800AC650.f48 + 1;
    D_800AC650.f48 = a1;
    if (a0 != D_800AC650.f4C) {
        if (D_800F4D44 == 5 || a1 >= 0x1A) {
            D_800AC650.f1C = 1;
            D_800AC650.f4C = a0;
            D_800AC650.f48 = 0;
        }
        func_800172DC(a0, a1);
    }
    switch (arg0) {
    case 0:
        func_80033BE4(arg1);
        break;
    case 1:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        func_800912D4(s2->f28);
        break;
    case 2:
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        a2 = func_80043C14(arg1, s2) >> 4;
        t1 = a2 / 4;
        if (a2 - t1 > (0x400000 / s2->f28) * 8) {
            t1 = a2 - (0x400000 / s2->f28) * 8;
        }
        sp10[0] = (D_800AC650.f20 / 4) - s2->x;
        sp10[1] = (D_800AC650.f24 / 4) - s2->y;
        sp10[2] = (D_800AC650.f28 / 4) - s2->z;
        scratch[2] = D_800AC650.f20 - ((sp10[0] * t1) / a2) * 4;
        scratch[3] = D_800AC650.f24 - ((sp10[1] * t1) / a2) * 4;
        scratch[4] = D_800AC650.f28 - ((sp10[2] * t1) / a2) * 4;
        a0 = (t1 * (s2->f28 << 4)) / 4096;
        if (a0 >= 0x401) {
            a0 = 0x400;
        } else if (a0 < 0x118) {
            a0 = 0x118;
        }
        func_800912D4(a0);
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        break;
    case 3:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        scratch[6] = s2->f10;
        scratch[7] = s2->f14;
        scratch[8] = func_80043FC0(s2, D_800AC698);
        func_800912D4(s2->f28);
        break;
    case 4:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        sp30[0] = s2->f10;
        sp30[1] = s2->f14;
        sp30[2] = 0;
        func_80043F5C(sp30, s2->f18, sp20);
        sp20[0] += s2->x;
        sp20[1] += s2->y;
        sp20[2] += s2->z;
        D_800AC650.f20 = ((((D_800AC650.f20 / 4) - sp20[0]) * s2->f2C) / 1024 + sp20[0]) * 4;
        D_800AC650.f24 = ((((D_800AC650.f24 / 4) - sp20[1]) * s2->f2C) / 1024 + sp20[1]) * 4;
        D_800AC650.f28 = ((((D_800AC650.f28 / 4) - sp20[2]) * s2->f2C) / 1024 + sp20[2]) * 4;
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        func_800912D4(s2->f28);
        break;
    case 5:
        func_80043DE4(arg1, &D_800AC650.f20, s2);
        sp38[0] = s2->f10;
        sp38[1] = s2->f14;
        sp38[2] = s2->f18;
        func_80043ED4(sp38, s2->f2C, &scratch[2]);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        scratch[2] += D_800AC650.f20;
        scratch[3] += D_800AC650.f24;
        scratch[4] += D_800AC650.f28;
        func_800912D4(s2->f28);
        break;
    case 6:
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        func_80043DE4(arg1, &scratch[2], s2);
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        func_800912D4(s2->f28);
        break;
    }
    func_8006E1AC();
}

void func_800448EC(void) {
    int *s = D_800ABEA4;
    D_800AC548[0] = s[2];
    D_800AC548[1] = 0;
    D_800AC548[2] = s[3];
    D_800AC548[4] = 0;
    D_800AC548[5] = 0;
    D_800AC548[6] = 0;
}

void func_80044920(a0, a1) A0 *a0; V8 *a1;
{
    struct { V8 m10; V8 m30; V8 m50; } s;
    struct { short a[3][3]; short pad[7]; } t70;
    struct { short a[3][3]; short pad[7]; } t90;
    V4 b0;
    int a3;
    b0 = D_80010F1C;
    func_8006E09C(&s.m50, -*(short *)((char *)a0 + 0x12));
    func_80090DB4((int *)0x1F800028, &s.m50);
    func_8006E108(&s.m10, -*(short *)((char *)a0 + 0x10));
    func_8006E030(&s.m30, -*(short *)((char *)a0 + 0x14));
    func_80090CA4(&s.m10, &s.m30);
    func_80090DB4(&s.m50, &s.m10);
    func_8006E09C(&s.m50, *(short *)((char *)a0 + 0x12) - 0x400);
    func_8006E108(&s.m30, *(short *)((char *)a0 + 0x10));
    func_80090CA4(&s.m50, &s.m30);
    func_8006E030(&s.m30, *(short *)((char *)a0 + 0x14));
    func_80090DB4(&s.m50, &s.m30);
    a3 = 0;
    do {
        t70.a[a3][0] = a0->v[a3].a;
        t70.a[a3][1] = -a0->v[a3].b;
        t70.a[a3][2] = a0->v[a3].c;
        t90.a[0][a3] = a0->c[a3].a * 16;
        t90.a[1][a3] = a0->c[a3].b * 16;
        t90.a[2][a3] = a0->c[a3].c * 16;
        a3++;
    } while (a3 < 3);
    func_80090914(t70.a, &s.m30, &s.m50, a3);
    func_800910E4(&s.m50);
    func_80091114(t90.a);
    func_80091274(a0->b3C, a0->b3D, a0->b3E);
    func_80090F74(&s.m10, &b0);
    *a1 = s.m10;
}

void func_80044B24(void) {
    short a[3];
    int b, c, d;
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    func_800916D4(a, &b, &c, &d);
}
