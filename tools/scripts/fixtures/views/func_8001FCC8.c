#include "rr/draw_environment.h"

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((void (*)(u32 *,u32 *))func_80095A44)

#undef func_80095A44

typedef struct { int a, b, c; } T;

extern int func_80095A44();

T *func_8001FCC8(u32 *a0, T *a1, unsigned short a2) {
    func_80093C24(a1, 0, 1, a2, 0);
    func_80095A44(a0, (u32 *)a1++);
    return a1;
}
