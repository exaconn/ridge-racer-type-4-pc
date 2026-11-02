#include "rr/overlay662_pixel_dispatch.h"

void ovl662_func_80124974(s32 actionCode)
{
    s32 i;
    u32 *clear;

    ovl662_func_8012571C();
    clear = ovl662_D_801276A8.scratch34;
    for (i = 511; i >= 0; i--) {
        *clear++ = 0;
    }
    if (ovl662_D_801276A8.config20 != 2) {
        switch (ovl662_D_801276A8.config1E) {
        case 0: ovl662_func_80124ABC(); break;
        case 1: ovl662_func_80124C94(); break;
        case 2: ovl662_func_80124E4C(); break;
        case 3: ovl662_func_80124FD8(); break;
        case 4: ovl662_func_801251AC(); break;
        }
    }
    if (ovl662_D_801276A8.config20 != 1) {
        switch (ovl662_D_801276A8.config1F) {
        case 0: ovl662_func_80125370(); break;
        case 1: ovl662_func_80125500(); break;
        case 2: ovl662_func_801256D4(); break;
        }
    }
}
