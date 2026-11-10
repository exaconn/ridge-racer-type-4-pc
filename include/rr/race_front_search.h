#ifndef RR_RACE_FRONT_SEARCH_H
#define RR_RACE_FRONT_SEARCH_H
#include "rr/race.h"

extern s16 NumCars;
extern s32 TrackLength;

/* Returns the identifier of the closest positive wrapped gap strictly below
 * (input+1)*16, or -1; a successful update also writes table.gap14.
 * Requires at most eight active rows, valid row identifiers/reference index,
 * and positive track length when rows are active. The context result pointer
 * names the complete canonical standings table. Ties retain the first match.
 * Inactive counts allow a null table and do not access the track length. */
s32 FindNearestCarAhead(RaceVehicle *vehicle);
#endif
