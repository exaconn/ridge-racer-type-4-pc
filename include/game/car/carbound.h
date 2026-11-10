#ifndef RR_GAME_CAR_CARBOUND_H
#define RR_GAME_CAR_CARBOUND_H

#include "types.h"
#include "rr/race_types.h"
#include "rr/matrix_words.h"

#include "rr/vehicle_table.h"
void DrawCars(s32 viewMode);

void AssignCarModelSlot(RaceVehicle *p);

/* Scale the second rotation row; its halfwords retain the low16 product. */
void ScaleMatrixRow1(s32 scale, MatrixWords32 *matrix);

#endif /* RR_GAME_CAR_CARBOUND_H */
