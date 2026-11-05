#include "types.h"
#include "rr/draw_environment.h"
void AddPrim(u32 *,u32 *);
typedef struct { int a, b, c; } T;

T *func_8001FCC8(u32 *a0, T *a1, unsigned short a2) {
    func_80093C24(a1, 0, 1, a2, 0);
    AddPrim(a0, (u32 *)a1++);
    return a1;
}
