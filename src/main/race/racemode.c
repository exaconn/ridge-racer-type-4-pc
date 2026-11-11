#include "game/car/car.h"
#include "rr/player_cars.h"
#include "rr/menu_overlay_runtime.h"
void CopyGhostCarToSlot1(RaceVehicle *a0) {
    PlayerCars[1] = *a0;
    *(short *)&a0->flags1E4 = *(unsigned char *)&a0->flags1E4;
    PlayerCars[1].physicsFlags1E0 = a0->physicsFlags1E0 + 2; /* RacePhysicsModeIndex 0x800AC3F8 */
}

