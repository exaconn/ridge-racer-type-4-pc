#include "types.h"

typedef struct { short pad[0xC]; short o; short n; short c; short to; short from; } T;

extern struct { char pad[0x18]; s16 a, b, c, d, e, f; } D_800F4DB8;
extern void func_8004FB00();
extern void func_80052B8C(int, short);

void func_8004FBF0(void);
void func_8004FC18(short arg0, short arg1);
void func_8004FCB4(void);

void func_8004FBF0(void) {
    D_800F4DB8.a = 0x80;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0;
    D_800F4DB8.e = 0;
    D_800F4DB8.f = 0;
}

void func_8004FC18(short arg0, short arg1) {
    if (arg1 >= 2) {
        D_800F4DB8.b = arg1;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = arg0;
        D_800F4DB8.e = D_800F4DB8.a;
    } else {
        D_800F4DB8.a = arg0;
        D_800F4DB8.b = 0;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = 0;
        D_800F4DB8.e = 0;
        if (arg0 == 0) {
            func_8004FB00();
        }
    }
    func_80052B8C(arg0 & 0xFFFF, arg1);
}

void func_8004FCB4(void) {
    T *p = &D_800F4DB8;
    int n = p->n;
    int v = 0x80;
    int c;
    short c1;
    if (n > 0) {
        c = (unsigned short)p->c;
        if (p->c < n) {
            n = ((p->to - p->from) << 12) / n;
            c1 = c + 1;
            p->c = c1;
            v = (unsigned short)p->from + n * c1 / 4096;
        } else {
            p->n = 0;
            func_8004FB00(n, p, c);
            return;
        }
    }
    p->o = v;
}
