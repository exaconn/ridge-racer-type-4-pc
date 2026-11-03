#include "rr/lookup.h"
#include "rr/race_types.h"

extern void func_8004EE60(s32 value);

void func_800394B8(RrRaceVehicle *vehicle)
{
    func_8004EE60(func_80055250(2, vehicle->trackNodeF0));
}
