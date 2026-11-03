#include "rr/track_sample_types.h"
#include "rr/race_types.h"

extern s16 D_800AD522;

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
