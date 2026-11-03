#include "rr/overlay662_car_choice.h"

void ovl662_func_801153B0(void) {
    SetDispMask(0);
    ovl662_D_801272E4 = 0;
    func_8002B75C(ovl662_D_801272C8, D_800F3920.prefix.progress.prefix00.fields.name);
    ovl662_func_8011512C();
    func_80037040((s8*)D_800F4D98.bytes, D_800ABE08[1][0].glyph);
    D_800ACBD0[0].first = D_800ABE08[1][ovl662_D_801272E4].first;
    D_800ACBD0[0].second = D_800ABE08[1][ovl662_D_801272E4].second;
    D_800F4C6A = 1;
    D_800F3A26 = 0x40;
    D_800F4D8A = 0;
    func_8001E84C(ovl662_D_80114888);
}
