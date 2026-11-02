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
