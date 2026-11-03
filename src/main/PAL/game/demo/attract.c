#include "types.h"
#include "m2c_macros.h"

extern int D_8010E240[];
extern char D_800ACB0C[];
extern int D_800A2124[];
extern int D_8010E270[];
extern unsigned char D_800F4D98[];
extern short D_800F3914;
extern unsigned short D_800F3A3A;
extern unsigned short D_800A2134;
extern unsigned short D_800A2138[];
extern int D_800A19E0[];
extern short D_800F4D44;
extern short D_800FFC10;
extern short D_800F4BE8;
extern short D_800F4C6A;
extern short D_800F3A26;
extern short D_800F4D8A;
extern int D_800F765C;
extern int D_800F2EC8;
extern char D_800AC650[];
extern short D_800AD50A;
extern int func_8005D868();
extern int func_80096434();
extern int func_80096464();
extern int func_80059314();
extern int func_8005C8D8();
extern int func_8005C13C();
extern int func_8005C988();
extern int func_80055300();
extern int func_8004C138();
extern int func_80055330();
extern int func_8005534C();
extern int func_8004F9C8();
extern int func_80077D90();
extern int func_8003A198();
extern int func_80092CB8();
extern s32 D_800ACBF8;

void func_8005D974(int a0);
void func_8005DD8C(void);

void func_8005D974(int a0) {
    int sp[4];
    int i;
    int y;
    int u;
    int *tb;
    int *q;
    int s0;
    int s2;
    int v;
    int t;
    /* ASM_BARRIER: pin rand()%2 result to $v0 like the original shuffle loop */
    register int r __asm__("$2");
    int *w;
    struct { unsigned char a, b, c, d; } *b;
    unsigned short m;
    unsigned short *e;
    int n;
    int *d;
    unsigned short h;
    char *c;
    int x6;
    int *pa, *pb;
    int vq;
    char *c2;
    int x5;

    if (D_8010E240[0] != 0) {
        func_8005D868();
        D_8010E240[0] = 0;
    }
    func_80096434(D_800ACB0C, 0x80);
    *(int *)(D_800ACB0C + 0xC) = a0;
    i = 3;
    q = sp + 3;
    do {
        *q = i;
        i--;
        q--;
    } while (i >= 0);
    i = 0;
    do {
        s0 = func_80096464() % 4;
        r = func_80096464() % 2;
        v = s0 + r;
        t = v;
        if (v < 0) t = v + 3;
        t = (t >> 2) << 2;
        vq = v - t;
        /* ASM_BARRIER: schedule barrier so the sp[s0] address is computed after vq (original order) */
        __asm__ volatile("" : "=r"(vq) : "0"(vq));
        s2 = sp[s0];
        y = sp[vq];
        sp[s0] = y;
        sp[vq] = s2;
        i++;
    } while (i < 0x10);
    i = 0;
    x6 = func_80096464();
    s2 = x6 / 18;
    s2 = x6 - s2 * 18;
    tb = D_800A2124;
    w = D_8010E270;
    b = (void *)D_800F4D98;
    do {
        b[i].a = sp[i];
        b[i].b = tb[sp[i]];
        *w = tb[sp[i]];
        u = s2 + func_80096464() % 4;
        v = u - 2;
        if (v < 0) v = 0;
        if (v > 0x12) v = 0x12;
        b[i].c = v;
        w++;
        i++;
    } while (i < 4);
    D_800F3914 = -1;
    if (a0 == 1) {
        m = D_800F3A3A & D_800A2134;
        if (m != 0) {
            i = 0;
            e = D_800A2138;
            do {
                if (m == e[0])
                    D_800F3914 = e[1];
                i++;
                e += 2;
            } while (i < 8);
        }
    }
    if (D_800F3914 < 0) {
        d = D_8010E240;
        n = d[9];
        h = ((unsigned short *)d)[n * 2 + 2];
        n = n + 1;
        d[9] = n;
        D_800F3914 = h;
        if (n >= 8)
            func_8005D868();
    }
    func_80059314(0, 0);
    D_800F4D44 = 5;
    D_800FFC10 = 0;
    D_800F4BE8 = 0xA;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    if (a0 == 0) {
        func_8005C8D8(-1);
        D_800F765C = D_800A19E0[D_800F3914] + 1;
    } else {
        func_8005C13C(0);
        func_8005C988(2);
        D_800F765C = 1;
    }
    func_80055300();
    c = D_800AC650;
    D_800F2EC8 = 1;
    *(short *)(c + 0x4E0) = 0xD2;
    *(short *)(c + 0x4E2) = 0;
    *(short *)(c + 0x4E4) = 0x8FC;
    func_8004C138(c + 0x4E0, c + 0x4E8);
    func_80055330(0, c[0x4E8], c[0x4E9], c[0x4EA]);
    *(int *)(c + 0x534) = 0;
    if (a0 == 0) {
        x5 = *(int *)(c + 0x3DC);
    } else {
        *(int *)(c + 0x3DC) = 0;
        x5 = 0;
    }
    func_8005534C(x5);
    c2 = D_800AC650;
    *(int *)(c2 + 0x4F0) = -0x20;
    *(int *)(c2 + 0x4EC) = 0;
    *(int *)(c2 + 0x4F4) = 0;
    *(int *)(c2 + 0x4FC) = 0;
    *(int *)(c2 + 0x500) = 0;
    *(int *)(c2 + 0x504) = 0;
    *(int *)(c2 + 0x514) = 0x122;
    func_8004F9C8();
    func_80077D90(0);
    D_800AD50A = 0x209;
}

void func_8005DD8C(void) {
    func_80092CB8(0);
    func_8003A198();
    D_800F4BE8 = 8;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    func_8005C13C(0);
    func_8005C988(1);
    D_800ACBF8 = 0;
}
