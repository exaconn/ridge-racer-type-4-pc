#include "rr/ui.h"
extern int func_8003D228();
extern int func_8003DC9C();
extern int func_8003E040();
extern int D_800ACC18, D_800F2DE4, D_800F4D40;
typedef struct { short a, b; } X;
typedef struct { char p[4]; char a; char b; char c; char d; } Y;
extern X *D_800F3AFC;
extern Y *D_800FF68C;
extern signed char D_800F4D7C;
extern int D_800F4D8C;
void func_8003687C(int a0) {
    int i = 0;
    int a;
    int t;
    int p;
    do {
        a = a0 + i;
        t = a * 0x1C;
        *(int *)(t + D_800F4D8C + 0x18) = *(int *)(i * 0x1C + D_800F4D8C + 0x18) + 0x30000;
        func_8003E040(a, -2, 0, 0);
        i += 1;
        p = t + D_800F4D8C;
        func_8003DC9C(*(unsigned char *)(p + 7), *(short *)(p + 8), *(short *)(p + 0xA), *(short *)(p + 0xC), *(short *)(p + 0xE), 0, 0);
    } while (i < 4);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    func_8003E928(0, 0x28, 0x48, 0xC0, 0x20, 0, 0, 4, 3, 4);
    D_800F3AFC->b = 1;
    D_800FF68C->a = 1;
    D_800FF68C->d = 0;
    D_800F2DE4 = 0;
    D_800F4D7C = 0;
    D_800ACC18 = D_800F4D40;
}
