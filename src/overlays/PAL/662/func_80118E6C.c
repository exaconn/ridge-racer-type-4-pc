#include "rr/overlay662_car_choice.h"

void ovl662_func_80118E6C(void) {
    s16 v0 = D_800ABE08[1][ovl662_D_801272E4].first;
    s16 v1;
    ovl662_D_801272BC = v0;
    v1 = D_800ABE08[1][ovl662_D_801272E4].second;
    D_800ACBD0[0].first = v0;
    ovl662_D_801272E2 = v1;
    D_800ACBD0[0].second = v1;
    ovl662_func_80118ED8();
}
