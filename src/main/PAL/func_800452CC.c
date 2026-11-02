typedef struct {
    int x, y, z, w;
} V4;

typedef struct {
    short x, y, z, w;
} V8;

typedef struct {
    V4 f0;
    V8 f10;
    int f18;
    int f1C;
    int f20;
    int f24;
} Elem28;

typedef struct {
    char pad0[0x10];
    int f10, f14, f18;
} Arg0Struct;

typedef struct {
    char pad0[0x60];
    V4 f60;
    V8 f70;
    int f78;
    int f7C;
    int f80;
    char pad84[0x20];
    int fA4;
} Context650;

extern void func_800172DC();
extern void func_80044B5C();
extern void func_80044EC4();
extern void func_800451E4();

extern Context650 D_800AC650;
extern int D_800AC6B0;
extern Elem28 (*D_800ACBA4)[16];
extern short D_800F4D44;

int func_800452CC(Arg0Struct *a0, int a1, int a2) {
    int pad[4];
    V4 sp20;
    V4 sp30;
    int s0;
    int s3;
    int i;
    Elem28 *s2;
    int t1;
    int t2;

    s0 = -1;
    s2 = D_800ACBA4[a2];
    for (i = 1; i < 0x10; i++) {
        if (s2[i].f24 * 5 >= a1) {
            s0 = i;
            s3 = s0 - 1;
            break;
        }
    }
    if (s0 < 0) {
        return 1;
    }
    if (D_800AC650.fA4 != s3) {
        func_800172DC();
        D_800AC650.fA4 = s3;
    }
    t1 = (s2[s0].f24 - s2[s3].f24) * 5;
    t2 = a1 - s2[s3].f24 * 5;
    if (s2[s3].f18 == 0) {
        sp20.x = a0->f10;
        sp20.y = a0->f14;
        sp20.z = a0->f18;
    } else {
        sp20 = s2[s3].f0;
    }
    if (s2[s0].f18 == 0) {
        sp30.x = a0->f10;
        sp30.y = a0->f14;
        sp30.z = a0->f18;
    } else {
        sp30 = s2[s0].f0;
    }
    if (t1 > 0) {
        D_800AC650.f60.x = ((sp30.x - sp20.x) * t2) / t1 + sp20.x;
        D_800AC650.f60.y = ((sp30.y - sp20.y) * t2) / t1 + sp20.y;
        D_800AC650.f60.z = ((sp30.z - sp20.z) * t2) / t1 + sp20.z;
        D_800AC650.f70.x = ((s2[s0].f10.x - s2[s3].f10.x) * t2) / t1 + s2[s3].f10.x;
        D_800AC650.f70.y = ((s2[s0].f10.y - s2[s3].f10.y) * t2) / t1 + s2[s3].f10.y;
        D_800AC650.f70.z = ((s2[s0].f10.z - s2[s3].f10.z) * t2) / t1 + s2[s3].f10.z;
        D_800AC650.f7C = ((s2[s0].f1C - s2[s3].f1C) * t2) / t1 + s2[s3].f1C;
        D_800AC650.f80 = ((s2[s0].f20 - s2[s3].f20) * t2) / t1 + s2[s3].f20;
    } else {
        D_800AC650.f60 = sp30;
        D_800AC650.f70 = s2[s0].f10;
        D_800AC650.f7C = s2[s0].f1C;
        D_800AC650.f80 = s2[s0].f20;
    }
    func_800451E4(&D_800AC6B0);
    if (D_800F4D44 == 1) {
        func_80044B5C();
        func_80044EC4();
    }
    return 0;
}
