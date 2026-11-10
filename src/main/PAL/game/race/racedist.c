#include "rr/vehicle_table.h"
#include "types.h"
#include "game/car/car.h"
#include "game/course/control.h"
#include "rr/trig.h"
#include "rr/track_interpolation.h"
#include "rr/race.h"
#include "game/race/racedist.h"
#include "game/data/globals.h"

#define SIN(x) (((x) & 0x800) ? (((x) & 0x400) ? -SinTable[0x400 - ((x) & 0x3FF)] : -SinTable[(x) & 0x3FF]) : (((x) & 0x400) ? SinTable[0x400 - ((x) & 0x3FF)] : SinTable[(x) & 0x3FF]))

extern RaceVehicle ComCars[];
extern RaceStandingsTable *RaceStandings;
extern s16 NumCars;
extern int abs(int);


void UpdateRaceStandings(void) {
    int i, j;
    RaceVehicle *v;
    RaceVehicleControl *c;
    RaceStandingsTable *t;
    int a0;
    RaceStandingEntry0C tmp;

    t = RaceStandings;
    for (i = 0; i < 0xB; i++) {
        v = &ComCars[i];
        c = &v->control;
        if ((s16)v->flags1E4 > 0) {
            a0 = -v->heading1D4 + 0x400;
            a0 -= TrackNodes[v->trackNodeF0].heading12;
            c->lateralSpeed58 = -((SIN(a0) * v->input) / 4096);
        }
    }

    if (NumPlayers > 0) {
        v = &PlayerCars[0];
        a0 = -v->heading1D4 + 0x400;
        a0 -= TrackNodes[v->trackNodeF0].heading12;
        t->unk00 = -((SIN(a0) * v->input) / 4096);
    }

    if (NumPlayers == 2) {
        v = &PlayerCars[1];
        a0 = -v->heading1D4 + 0x400;
        a0 -= TrackNodes[v->trackNodeF0].heading12;
        t->unk04 = -((SIN(a0) * v->input) / 4096);
    }

    for (i = 0; i < NumCars; i++) {
        v = CarTable[i];
        t->entries18[i].unk00 = i;
        t->entries18[i].totalDist04 = v->first + v->second;
        t->entries18[i].vehicle08 = v;
    }

    for (i = 1; i < NumCars; i++) {
        tmp = t->entries18[i];
        for (j = i - 1; j >= 0 && t->entries18[j].totalDist04 < tmp.totalDist04; j--) {
            t->entries18[j + 1] = t->entries18[j];
        }
        t->entries18[j + 1] = tmp;
    }
}

static __inline__ s32 WrappingMagnitude(s32 value) {
    return value < 0 ? (s32)(0U - value) : value;
}

void CalcDistanceToPlayer(RaceVehicle *input) {
    RaceDistanceResult *out = RaceContext.result;
    if (RaceContext.mode == 1) {
        out->first = (PlayerCars[0].first + 0U + PlayerCars[0].second) - (input->first + 0U + input->second);
        out->second = 0x7FFFFFFF;
        out->selected = 0;
    } else if (RaceContext.mode == 2) {
        out->first = (PlayerCars[0].first + 0U + PlayerCars[0].second) - (input->first + 0U + input->second);
        out->second = (PlayerCars[1].first + 0U + PlayerCars[1].second) - (input->first + 0U + input->second);
        if (WrappingMagnitude(out->first) <= WrappingMagnitude(out->second)) {
            out->selected = 0;
        } else {
            out->selected = 1;
        }
    } else {
        out->first = 0;
        out->second = 0;
        out->selected = 0;
    }
}
