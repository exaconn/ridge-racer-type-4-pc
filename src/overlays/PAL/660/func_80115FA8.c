#include "rr/menu_overlay_runtime.h"

void func_8002B75C(char *destination, char *source);

extern char ovl660_D_80117920[12];

s32 ovl660_func_80115FA8(s32 mode, s32 time) {
    s32 times[6];
    s16 order[6];
    s32 i;
    s32 j;
    s16 o;

    times[5] = time;
    order[5] = 5;
    for (i = 0; i < 5; i++) {
        order[i] = i;
        if (mode == 1) {
            times[i] = D_800AD5C8[D_800AD522][D_800F3914][i].time;
        } else {
            times[i] = D_800FF700[D_800AD522][D_800F3914][i].time;
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 5; i < j; j--) {
            s32 earlier = times[j - 1];
            s32 later = times[j];
            if (later < earlier) {
                o = order[j];
                times[j] = earlier;
                times[j - 1] = later;
                order[j] = order[j - 1];
                order[j - 1] = o;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        if (order[i] == 5) {
            for (j = 4; i < j; j--) {
                if (mode == 1) {
                    D_800AD5C8[D_800AD522][D_800F3914][j] = D_800AD5C8[D_800AD522][D_800F3914][j - 1];
                } else {
                    D_800FF700[D_800AD522][D_800F3914][j] = D_800FF700[D_800AD522][D_800F3914][j - 1];
                }
            }
            if (mode == 1) {
                D_800AD5C8[D_800AD522][D_800F3914][i].car = func_8003708C((s8 *)&D_800F4D98);
                D_800AD5C8[D_800AD522][D_800F3914][i].time = times[i];
                func_8002B75C(D_800AD5C8[D_800AD522][D_800F3914][i].name, ovl660_D_80117920);
            } else {
                D_800FF700[D_800AD522][D_800F3914][i].car = func_8003708C((s8 *)&D_800F4D98);
                D_800FF700[D_800AD522][D_800F3914][i].time = times[i];
                func_8002B75C(D_800FF700[D_800AD522][D_800F3914][i].name, ovl660_D_80117920);
            }
            return i;
        }
    }
    return i;
}
