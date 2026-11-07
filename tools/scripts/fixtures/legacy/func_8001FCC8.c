#include "types.h"
#include "rr/draw_environment.h"
void AddPrim(u32 *,u32 *);
typedef struct { int a, b, c; } T;

T *AddDrawModePrim(u32 *a0, T *a1, unsigned short a2) {
    SetDrawMode(a1, 0, 1, a2, 0);
    AddPrim(a0, (u32 *)a1++);
    return a1;
}
