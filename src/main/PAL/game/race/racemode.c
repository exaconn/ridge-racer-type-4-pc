#include "game/car/car.h"
#include "rr/menu_overlay_runtime.h"
extern int D_800AC3F8;
void CopyGhostCarToSlot1(RaceVehicle *a0) {
    PlayerCars[1] = *a0;
    *(short *)&a0->flags1E4 = *(unsigned char *)&a0->flags1E4;
    D_800AC3F8 = a0->physicsFlags1E0 + 2;
}
