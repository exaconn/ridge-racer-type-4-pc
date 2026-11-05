#include "rr/draw_environment.h"

/* legacy typing of shared globals in this part of the unit */
#define AddPrim ((void (*)(u32 *,u32 *))AddPrim)

#undef AddPrim

typedef struct { int a, b, c; } T;

extern int AddPrim();

T *func_8001FCC8(u32 *a0, T *a1, unsigned short a2) {
    func_80093C24(a1, 0, 1, a2, 0);
    AddPrim(a0, (u32 *)a1++);
    return a1;
}
