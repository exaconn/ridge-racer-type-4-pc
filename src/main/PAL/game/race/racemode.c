#include "game/car/car.h"
extern Car D_800ABEF8[];
extern int D_800AC3F8;
void func_800337A8(Car *a0) {
    D_800ABEF8[1] = *a0;
    *(short *)&a0->unk1E4 = *(unsigned char *)&a0->unk1E4;
    D_800AC3F8 = a0->unk1E0 + 2;
}
