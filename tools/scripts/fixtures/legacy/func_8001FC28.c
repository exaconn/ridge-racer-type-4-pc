#include "rr/badge_rows.h"
struct P12 { int a, b, c; };
extern int SetDrawArea();
extern int func_80095A44();
extern char D_800ADAF0[];
extern int D_800F4BF0;
void *func_8001FC28(u32 *a0, void *cursor, int a2, int a3, int a4, int a5) {
    struct P12 *a1=cursor;
    short sp[4];
    char *p = D_800ADAF0;
    int t;
    sp[0] = a2;
    if (*(short *)(p + 0x5E) == *(short *)(p + 0x227D6)) {
        sp[1] = a3;
    } else {
        t = D_800F4BF0;
        sp[1] = a3 + (t << 8);
    }
    sp[2] = a4;
    sp[3] = a5;
    SetDrawArea(a1, sp);
    func_80095A44(a0, a1++);
    return a1;
}
