#include "rr/diagnostics.h"
#include "rr/overlay662_garage.h"
#include "rr/menu_setup.h"
extern const char ovl662_D_80114900[];
extern s16 D_800F4C6A,D_800F3A26,D_800F4D8A;
void InitUiRow(s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,...);
void ovl662_func_8011A988(void) {
    func_8001E84C(ovl662_D_80114900);
    ovl662_func_8011A788(1);
    ovl662_func_8011D278(-1);
    ovl662_func_8011D120();
    ovl662_D_801276A8.dirtyD3 = 0;
    ClearScreenOverlayState();
    InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
    InitUiRow(0, 140, 136, 28, 32, 0, 0, 1, 0, 104);
    InitUiRow(1, 140, 176, 28, 32, 0, 0, 1, 0, 105);
    D_800F4C6A = 3;
    D_800F3A26 = 4;
    D_800F4D8A = 0;
}
