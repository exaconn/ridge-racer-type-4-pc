#include "rr/race_types.h"

#include "rr/sequence_queue.h"
#include "rr/lookup.h"
extern int D_800ABEC0;
extern short D_800AD522;
extern int D_800F2EB8;
extern int D_800F3A30;
void func_800396E4(RrRaceVehicle *vehicle)
{
    int distanceLimit;
    int courseLength;
    int flags, node;
    if (!(D_800ABEC0 & 0x100)) {
        if (D_800AD522 != 1) {
            courseLength = D_800F2EB8;
            distanceLimit = (D_800F3A30 - 1) * courseLength;
            if ((vehicle->second + vehicle->first) >= distanceLimit) {
                node = vehicle->trackNodeF0;
                if (func_80055250(6, node) == 1) {
                    flags = D_800ABEC0 & 0x100;
                    D_800ABEC0 = flags;
                    if (flags == 0) {
                        func_8004F2D8(0x12, 0);
                        D_800ABEC0 |= 0x100;
                    }
                }
            }
        }
    }
}
