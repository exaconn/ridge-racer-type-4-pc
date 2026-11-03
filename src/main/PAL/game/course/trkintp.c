#include "types.h"
#include "rr/track_sample_types.h"
#include "rr/race_types.h"
#include "m2c_macros.h"
#include "game/car/carphys2.h"
#include "game/course/trknode.h"
#include "rr/menu_overlay_runtime.h"

void func_80025EB0(int a, int b);
int func_80025F84(int a0, int a1);
s32 func_80025FBC(s32 arg0, s32 arg1);
void func_80026004(char *vehicle, int direction);

void func_80025EB0(int a, int b)
{
    int s2;
    int s0;
    int x;
    s2 = func_80025D0C(a, b);
    x = a - 2;
    if (x < 0) {
        int d = D_800ABECC - 2;
        x = d + a;
    }
    s0 = func_80025C94(s2, func_80025D0C(x, b), 0x200);
    func_80025C94(s0, func_80025C94(s2, func_80025D0C((a + 2) % D_800ABECC, b), 0x200), 0x200);
}

int func_80025F84(int a0, int a1) {
    int d;
    a0 &= 0xFFF;
    a1 &= 0xFFF;
    if (a0 < a1) d = a1 - a0; else d = a0 - a1;
    if (d > 0x800) d = 0x1000 - d;
    return d;
}

s32 func_80025FBC(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;

    temp_a0 = arg0 & 0xFFF;
    temp_v0 = arg1 & 0xFFF;
    var_v1 = temp_a0 < temp_v0;
    if (var_v1 != 0) {
        var_a0 = temp_v0 - temp_a0;
    } else {
        var_a0 = temp_a0 - temp_v0;
    }
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
        var_v1 = var_v1 == 0;
    }
    var_v0 = var_a0;
    if (var_v1 == 0) {
        var_v0 = -var_v0;
    }
    return var_v0;
}

void func_80026004(char *vehicle, int direction)
{
    int node;
    int target;
    int distance;
    int wrapped;
    int previous;
    *(s32 *)(vehicle + 0x184) = 0;
    node = *D_800ACB8C;
    target = *(s32 *)(vehicle + 0xF0);
    distance = 0;
    if (D_800AD522 == 0) {
        if (direction == 0) {
            while (1) {
                wrapped = node % D_800ABECC;
                distance -= D_800ABEA4[wrapped].segmentLength1C;
                if (target == wrapped) break;
                node++;
            }
        } else {
            while (1) {
                node--;
                previous = node;
                if (node < 0) previous = D_800ABECC + node;
                node = previous;
                if (target == node) break;
                distance += D_800ABEA4[node].segmentLength1C;
            }
        }
    } else {
        if (direction == 0) {
            do {
                node--;
                previous = node;
                if (node < 0) previous = D_800ABECC + node;
                node = previous;
                distance -= D_800ABEA4[node].segmentLength1C;
            } while (target != node);
        } else {
            while (1) {
                if (target == node % D_800ABECC) break;
                node++;
                distance += D_800ABEA4[node % D_800ABECC].segmentLength1C;
            }
        }
    }
    *(s32 *)(vehicle + 0x184) = distance;
}
