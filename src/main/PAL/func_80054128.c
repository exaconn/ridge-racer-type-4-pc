typedef struct { int f0, st, tm; int pad[2]; short s14; short s16; unsigned short u18; unsigned short u1a; short s1c; unsigned short u1e; } E;
extern E D_800F2AE4[];

void func_80054128(int a0) {
    E *e = &D_800F2AE4[a0];
    int st = e->st;
    int t, d;
    switch (st) {
    case 0:
        if (e->s1c < 0xC) {
            e->s1c = e->s1c + 1;
        }
        e->u18 = e->u18 + e->s1c;
        d = e->s14 - (short)e->u18;
        if (e->s1c * 3 < d) {
            break;
        }
        goto L15;
    case 1:
        if (e->s1c > 0) {
            e->s1c = e->s1c - 2;
            e->u18 = e->u18 + e->s1c;
        } else {
            e->st = 2;
            e->tm = 0x32;
        }
        break;
    case 2:
        e->tm = e->tm - 1;
        if (e->tm <= 0) {
    L15:
            e->st = e->st + 1;
        }
        break;
    case 3:
        t = e->u1a;
        t -= 8;
        e->u1a = t;
        if ((t << 16) <= 0) {
            e->f0 = 0;
        }
        break;
    }
    e->u1e = e->u1e + 1;
}
