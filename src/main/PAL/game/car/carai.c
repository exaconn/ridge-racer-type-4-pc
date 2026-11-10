#include "scratchpad.h"
#include "rr/track_projection.h"
#include "rr/race.h"
#include "game/car/carspec3.h"
#include "game/race/racedist.h"
#include "game/car/carai.h"
#include "game/course/trkproj.h"
#include "rr/track_lateral.h"
#include "rr/vehicle_drive.h"
#include "game/car/cardrive.h"
#include "psyq/libc/rand.h"

extern RaceStandingsTable *RaceStandings;
extern RaceVehicle ComCars[];
extern int D_800F4DEC;
extern int RaceState;
extern struct { u8 reserved00[0x34]; int s34; } SoundState;

extern void UpdateComCarLaneChange();

void UpdateComCars(void) {
    int i;
    RaceVehicle *s0;
    RaceVehicleControl *s1;
    int r;

    RaceStandings = (RaceStandingsTable *)(SCRATCHPAD + 0x7C);
    UpdateRaceStandings();
    switch (RaceState) {
    case 0:
        D_800F4DEC = 0;
        break;
    case 1:
        SoundState.s34 = SoundState.s34 + 1;
        break;
    }
    for (i = 0; i < 0xB; i++) {
        s0 = &ComCars[i];
        s1 = &s0->control;
        if ((short)s0->flags1E4 > 0) {
            CalcDistanceToPlayer(s0);
            if (RaceState < 2) {
                if (RaceState == 0) {
                    s1->slideTimer5C = PsyqRand() % 1000 + 0x1D4C;
                    r = PsyqRand();
                    s1->engine54.halves.accelerating = 1;
                    s1->engine54.halves.rpm = 0x400;
                    s1->latch60 = 0;
                    s1->slideDivisor5E = r % 100 + 0x190;
                } else if ((u16)s1->engine54.halves.accelerating == 1) {
                    s1->engine54.halves.rpm = s1->engine54.halves.rpm + (u16)s1->slideDivisor5E;
                    if (s1->engine54.halves.rpm > s1->slideTimer5C) {
                        s1->engine54.halves.accelerating = 0;
                        s1->slideTimer5C = PsyqRand() % 1000 + 0x157C;
                        s1->slideDivisor5E = PsyqRand() % 100 + 0xC8;
                    }
                } else {
                    s1->engine54.halves.rpm = s1->engine54.halves.rpm - (u16)s1->slideDivisor5E;
                    if (s1->engine54.halves.rpm < s1->slideTimer5C) {
                        s1->engine54.halves.accelerating = 1;
                        s1->slideTimer5C = PsyqRand() % 1000 + 0x1D4C;
                        s1->slideDivisor5E = PsyqRand() % 100 + 0x190;
                    }
                }
            } else {
                ApplyRubberBandSpec(s0);
                ComputeCarTrackSurfaceProj(s0);
                UpdateComCarOvertake(s0);
                UpdateComCarTargetPoint(s0);
                UpdateComCarDrive(s0);
                UpdateComCarLaneChange(s0);
                UpdateComCarSpeed(s0);
            }
        }
    }
}
