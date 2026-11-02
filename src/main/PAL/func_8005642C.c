typedef struct {
    int vx, vy, vz, pad;
} VECTOR;

typedef struct {
    short vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    short m[3][3];
    short pad;
    int t[3];
} MATRIX;

typedef struct {
    VECTOR vec;
    SVECTOR rot;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
} Node;

typedef struct {
    char pad0[0x108];
    int f108;
    int f10C;
    char pad1[0x138 - 0x110];
    Node (*nodes)[24];
} Sub24C;

typedef struct {
    char pad0[0x60];
    Node sub60;
    char pad1[0x24C - 0x8C];
    Sub24C *p;
} Context650;

typedef struct {
    char pad0[0x20];
    int f20;
    int f24;
    int f28;
} CarState;

typedef struct {
    int pad0[2];
    VECTOR vec08;
    int r18;
    int r1C;
    int r20;
} Scratch;

extern int func_800451E4();
extern int func_80056258();
extern int func_8006E1AC();
extern int func_80090EC4();
extern int func_800912D4();
extern int func_80091844();

extern VECTOR D_800A0FC0;
extern SVECTOR D_800A0FD0;
extern int D_800A0FD8;
extern CarState D_800ABEF8;
extern Context650 D_800AC650;

int func_8005642C(CarState *arg0, int arg1, int arg2, int *arg3) {
    VECTOR sp10;
    VECTOR sp20;
    VECTOR sp30;
    MATRIX sp40;
    SVECTOR sp60;
    SVECTOR sp68;
    VECTOR sp70;
    Scratch *s5 = (Scratch *)0x1F800000;
    int s0 = -1;
    int s3;
    int i;
    Node *s6;
    int s2;
    int s4;

    s6 = D_800AC650.p->nodes[arg2];
    for (i = 1; i < 0x18; i++) {
        if (s6[i].f24 * 10 >= arg1) {
            s0 = i;
            s3 = i - 1;
            *arg3 = s3;
            break;
        }
    }

    if (s0 < 0) {
        sp60.vx = 0;
        sp60.vy = -0xA00;
        sp60.vz = 0;
        sp68.vx = D_800A0FC0.vx;
        sp68.vy = D_800A0FC0.vy;
        sp68.vz = D_800A0FC0.vz;
        func_80091844(&sp60, &sp40);
        func_80090EC4(&sp40, &sp68, &sp70);
        sp70.vx += D_800ABEF8.f20 * 4;
        sp70.vy += D_800ABEF8.f24 * 4;
        sp70.vz += D_800ABEF8.f28 * 4;
        s5->vec08.vx = sp70.vx;
        s5->vec08.vy = sp70.vy;
        s5->vec08.vz = sp70.vz;
        s5->r18 = D_800A0FD0.vx;
        s5->r1C = D_800A0FD0.vy;
        s5->r20 = D_800A0FD0.vz;
        func_800912D4(D_800A0FD8);
        func_8006E1AC();
        return 1;
    }

    if (s3 != D_800AC650.p->f10C) {
        D_800AC650.p->f10C = s3;
        if (s6[s3].f28 != 0) {
            D_800AC650.p->f108 = s6[s3].f28 * 10;
        }
    }

    func_80056258(D_800AC650.p->f108);

    s2 = (s6[s0].f24 - s6[s3].f24) * 10;
    s4 = arg1 - s6[s3].f24 * 10;

    if (s6[s3].f18 == 0) {
        sp20.vx = arg0->f20 * 4;
        sp20.vy = arg0->f24 * 4;
        sp20.vz = arg0->f28 * 4;
    } else {
        sp20 = s6[s3].vec;
    }

    if (s6[s0].f18 == 0) {
        sp30.vx = arg0->f20 * 4;
        sp30.vy = arg0->f24 * 4;
        sp30.vz = arg0->f28 * 4;
    } else {
        sp30 = s6[s0].vec;
    }

    if (s6[s0].f24 < 0x50) {
        if (s2 > 0) {
            D_800AC650.sub60.vec.vx = ((sp30.vx - sp20.vx) * s4) / s2 + sp20.vx;
            D_800AC650.sub60.vec.vy = ((sp30.vy - sp20.vy) * s4) / s2 + sp20.vy;
            D_800AC650.sub60.vec.vz = ((sp30.vz - sp20.vz) * s4) / s2 + sp20.vz;
            D_800AC650.sub60.rot.vx = ((s6[s0].rot.vx - s6[s3].rot.vx) * s4) / s2 + s6[s3].rot.vx;
            D_800AC650.sub60.rot.vy = ((s6[s0].rot.vy - s6[s3].rot.vy) * s4) / s2 + s6[s3].rot.vy;
            D_800AC650.sub60.rot.vz = ((s6[s0].rot.vz - s6[s3].rot.vz) * s4) / s2 + s6[s3].rot.vz;
            D_800AC650.sub60.f1C = ((s6[s0].f1C - s6[s3].f1C) * s4) / s2 + s6[s3].f1C;
            D_800AC650.sub60.f20 = ((s6[s0].f20 - s6[s3].f20) * s4) / s2 + s6[s3].f20;
        } else {
            D_800AC650.sub60.vec = sp30;
            D_800AC650.sub60.rot = s6[s0].rot;
            D_800AC650.sub60.f1C = s6[s0].f1C;
            D_800AC650.sub60.f20 = s6[s0].f20;
        }
        func_800451E4(&D_800AC650.sub60);
        return 0;
    }

    sp60.vx = 0;
    sp60.vy = -0xA00;
    sp60.vz = 0;
    sp68.vx = D_800A0FC0.vx;
    sp68.vy = D_800A0FC0.vy;
    sp68.vz = D_800A0FC0.vz;
    func_80091844(&sp60, &sp40);
    func_80090EC4(&sp40, &sp68, &sp70);
    sp70.vx += D_800ABEF8.f20 * 4;
    sp70.vy += D_800ABEF8.f24 * 4;
    sp70.vz += D_800ABEF8.f28 * 4;

    if (s2 > 0) {
        D_800AC650.sub60.vec.vx = sp20.vx;
        D_800AC650.sub60.vec.vy = sp20.vy;
        D_800AC650.sub60.vec.vz = sp20.vz;
        D_800AC650.sub60.rot.vx = s6[s3].rot.vx;
        D_800AC650.sub60.rot.vy = s6[s3].rot.vy;
        D_800AC650.sub60.rot.vz = s6[s3].rot.vz;
        D_800AC650.sub60.f1C = s6[s3].f1C;
        D_800AC650.sub60.f20 = s6[s3].f20;
        func_800451E4(&D_800AC650.sub60);
        D_800AC650.sub60.vec = s5->vec08;
        D_800AC650.sub60.rot.vx = s5->r18;
        D_800AC650.sub60.rot.vy = s5->r1C;
        D_800AC650.sub60.rot.vz = s5->r20;
        s5->vec08.vx = ((sp70.vx - D_800AC650.sub60.vec.vx) * s4) / s2 + D_800AC650.sub60.vec.vx;
        s5->vec08.vy = ((sp70.vy - D_800AC650.sub60.vec.vy) * s4) / s2 + D_800AC650.sub60.vec.vy;
        s5->vec08.vz = ((sp70.vz - D_800AC650.sub60.vec.vz) * s4) / s2 + D_800AC650.sub60.vec.vz;
        s5->r18 = ((D_800A0FD0.vx - D_800AC650.sub60.rot.vx) * s4) / s2 + D_800AC650.sub60.rot.vx;
        s5->r1C = ((D_800A0FD0.vy - D_800AC650.sub60.rot.vy) * s4) / s2 + D_800AC650.sub60.rot.vy;
        s5->r20 = ((D_800A0FD0.vz - D_800AC650.sub60.rot.vz) * s4) / s2 + D_800AC650.sub60.rot.vz;
        D_800AC650.sub60.f20 = ((D_800A0FD8 - s6[s3].f20) * s4) / s2 + s6[s3].f20;
        func_800912D4(D_800AC650.sub60.f20);
    } else {
        s5->vec08.vx = sp70.vx;
        s5->vec08.vy = sp70.vy;
        s5->vec08.vz = sp70.vz;
        s5->r18 = D_800A0FD0.vx;
        s5->r1C = D_800A0FD0.vy;
        s5->r20 = D_800A0FD0.vz;
        func_800912D4(D_800A0FD8);
    }
    func_8006E1AC();
    return 0;
}
