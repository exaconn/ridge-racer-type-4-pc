#ifndef RR_GAME_COURSE_TRKRES2_H
#define RR_GAME_COURSE_TRKRES2_H

#include "types.h"
#include "rr/race_types.h"
#include "game/car/grid.h"

/* Align an address or size in the native machine-word domain. */
unsigned long AlignUp4(unsigned long value);
void SetCarSpecTable(RaceSpecEntry30 *arg0);
void SetTrackStartNode(Grid53C *startGrid);
void SetTrackWorkspace(u8 *workspace);


#endif /* RR_GAME_COURSE_TRKRES2_H */
