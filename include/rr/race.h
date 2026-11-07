#ifndef RR_RACE_H
#define RR_RACE_H

#include "rr/layout.h"

#include "rr/race_types.h"

extern RrRaceVehicle PlayerCars[2];
extern RrRaceDistanceContextPrefix RaceContext;
void CalcDistanceToPlayer(RrRaceVehicle *current);
/* The interpolation path requires span != 0; positive blend requires divisor != 0.
 * The target retains the original DIV exception checks for invalid records. */
/* Return a full signed word whose value is normalized to signed16. */
s32 CalcSteerLimit(RrRaceVehicle *vehicle);

RR_ASSERT_SIZE(RrRaceVehicle, 0x320);
RR_ASSERT_OFFSET(RrRaceVehicle, pos10, 0x10);
RR_ASSERT_OFFSET(RrRaceVehicle, pos20, 0x20);
RR_ASSERT_OFFSET(RrRaceVehicle, vel30, 0x30);
RR_ASSERT_OFFSET(RrRaceVehicle, rot50, 0x50);
RR_ASSERT_OFFSET(RrRaceVehicle, ang80, 0x80);
RR_ASSERT_OFFSET(RrRaceVehicle, savedRotC8, 0xC8);
RR_ASSERT_OFFSET(RrRaceVehicle, trackNodeF0, 0xF0);
RR_ASSERT_OFFSET(RrRaceVehicle, height108, 0x108);
RR_ASSERT_OFFSET(RrRaceVehicle, halfSpan10E, 0x10E);
RR_ASSERT_OFFSET(RrRaceVehicle, lateralOffset172, 0x172);
RR_ASSERT_OFFSET(RrRaceVehicle, first, 0x184);
RR_ASSERT_OFFSET(RrRaceVehicle, second, 0x188);
RR_ASSERT_OFFSET(RrRaceVehicle, shakeX198, 0x198);
RR_ASSERT_OFFSET(RrRaceVehicle, steerAngle1A2, 0x1A2);
RR_ASSERT_OFFSET(RrRaceVehicle, steerInput1BA, 0x1BA);
RR_ASSERT_OFFSET(RrRaceVehicle, heading1D4, 0x1D4);
RR_ASSERT_OFFSET(RrRaceVehicle, input, 0x1D8);
RR_ASSERT_OFFSET(RrRaceVehicle, flags1E4, 0x1E4);
RR_ASSERT_OFFSET(RrRaceVehicle, trackAngle1EC, 0x1EC);
RR_ASSERT_OFFSET(RrRaceVehicle, playerFlags218, 0x218);
RR_ASSERT_OFFSET(RrRaceVehicle, laneTimer22C, 0x22C);
RR_ASSERT_OFFSET(RrRaceVehicle, blend, 0x238);
RR_ASSERT_OFFSET(RrRaceVehicle, speed240, 0x240);
RR_ASSERT_OFFSET(RrRaceVehicle, shiftDir256, 0x256);
RR_ASSERT_OFFSET(RrRaceVehicle, relDist268, 0x268);
RR_ASSERT_OFFSET(RrRaceVehicle, laneMode27C, 0x27C);
RR_ASSERT_OFFSET(RrRaceVehicle, steering288, 0x288);
RR_ASSERT_OFFSET(RrSteeringState, duration04, 4);
RR_ASSERT_OFFSET(RrRaceVehicle, maxSpeed2EC, 0x2EC);
RR_ASSERT_OFFSET(RrRaceVehicle, maximum, 0x30C);
RR_ASSERT_OFFSET(RrRaceVehicle, span, 0x30E);
RR_ASSERT_OFFSET(RrRaceVehicle, divisor, 0x310);
RR_ASSERT_OFFSET(RrRaceVehicle, metric31E, 0x31E);
RR_ASSERT_SIZE(RrRaceVehicleControl, 0x12C);
RR_ASSERT_OFFSET(RrRaceVehicleControl, laneTimer38, 0x38);
RR_ASSERT_OFFSET(RrRaceVehicleControl, boostTimer44, 0x44);
RR_ASSERT_OFFSET(RrRaceVehicleControl, speed4C, 0x4C);
RR_ASSERT_OFFSET(RrRaceVehicleControl, shiftDir62, 0x62);
RR_ASSERT_OFFSET(RrRaceVehicleControl, relDist74, 0x74);
RR_ASSERT_OFFSET(RrRaceVehicleControl, laneMode88, 0x88);
RR_ASSERT_OFFSET(RrRaceVehicleControl, maxSpeedF8, 0xF8);
RR_ASSERT_OFFSET(RrRaceVehicleControl, maximum118, 0x118);
RR_ASSERT_OFFSET(RrRaceVehicleControl, metric12A, 0x12A);
RR_ASSERT_SIZE(RrRaceSpecEntry30, 0x30);
RR_ASSERT_OFFSET(RrRaceSpecEntry30, thresholds14, 0x14);
RR_ASSERT_OFFSET(RrRaceSpecEntry30, maximum20, 0x20);
RR_ASSERT_OFFSET(RrRaceSpecEntry30, altSpeed28, 0x28);
RR_ASSERT_SIZE(RrRaceStandingEntry0C, 0xC);
RR_ASSERT_OFFSET(RrRaceStandingsTable, entries18, 0x18);
RR_ASSERT_SIZE(RrRaceDistanceResult, 0x14);
RR_ASSERT_OFFSET(RrRaceDistanceResult, first, 8);
RR_ASSERT_OFFSET(RrRaceDistanceResult, second, 0xC);
RR_ASSERT_OFFSET(RrRaceDistanceResult, selected, 0x10);
RR_ASSERT_SIZE(RrRaceDistanceContextPrefix, 0xEC);
RR_ASSERT_OFFSET(RrRaceDistanceContextPrefix, mode, 0x14);
RR_ASSERT_OFFSET(RrRaceDistanceContextPrefix, result, 0xE8);

#endif
