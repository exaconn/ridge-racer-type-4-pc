#include "rr/track_projection.h"
#include "rr/geometry.h"
#include "rr/trig.h"

void func_8004638C(RrRaceVehicle *vehicle)
{
    struct {
        RrTrackOffsetArgument a;
        RrTrackOffsetArgument b;
        RrVector first;
        RrVector second;
        s16 fraction;
        int remaining;
    } line;
    int distance = 0x400 - vehicle->trackFraction170;
    int rounded = distance;
    int firstNode;
    int secondNode;
    int fraction;
    RrTrackLaneOffset8 *offset;
    RrTrackLaneOffset8 *sample;
    RrRaceVehicleControl *control = (RrRaceVehicleControl *)((u8 *)vehicle + RR_RACE_CONTROL_OFFSET);
    if (rounded < 0) {
        rounded += 0x3FF;
    }
    firstNode = vehicle->trackNodeF0 - (rounded >> 10);
    if (firstNode < 0) {
        firstNode += D_800ABECC;
    }
    secondNode = firstNode - 1;
    if (secondNode < 0) {
        secondNode += D_800ABECC;
    }
    offset = D_800ACB9C + firstNode;
    sample = offset;
    line.first.x = sample->x;
    line.first.z = sample->z;
    sample = D_800ACB9C + secondNode;
    line.second.x = sample->x;
    line.second.z = sample->z;
    fraction = distance & 0x3FF;
    {
        int dx = (line.second.x - line.first.x) * fraction;
        int dz;
        if (dx < 0) {
            dx += 0x3FF;
        }
        dz = (line.second.z - line.first.z) * fraction;
        line.a.value = line.first.x + (dx >> 10);
        if (dz < 0) {
            dz += 0x3FF;
        }
        line.b.value = line.first.z + (dz >> 10);
    }
    func_800464E8(line.a, line.b, vehicle->trackNodeF0, &line.fraction, &vehicle->laneCurrent232, &line.remaining);
    if (control->laneTimer38 == 0) {
        control->laneTarget3C = control->laneCurrent3E;
    }
}


#define SIN(x) (((x) & 0x800) ? (((x) & 0x400) ? -D_800A68C0[0x400 - ((x) & 0x3FF)] : -D_800A68C0[(x) & 0x3FF]) : (((x) & 0x400) ? D_800A68C0[0x400 - ((x) & 0x3FF)] : D_800A68C0[(x) & 0x3FF]))

void func_800464E8(RrTrackOffsetArgument offsetX, RrTrackOffsetArgument offsetZ, s32 node, s16 *fraction, s16 *lateral, s32 *remaining) {
    s32 nextNode;
    RrTrackSample3C *sample;
    RrTrackSample3C *nextSample;
    s32 angle;
    s32 cosine;
    s32 along;
    s32 across;
    s32 position;
    s32 width;
    s32 nextWidth;

    nextNode = (node + 1) % D_800ABECC;
    sample = &D_800ABEA4[node];
    nextSample = &D_800ABEA4[nextNode];

    angle = sample->heading12 + 0x800;
    node = sample->heading12 + 0x400;

    across = SIN(angle);
    cosine = SIN(node);

    along = (across * offsetX.value + cosine * offsetZ.value) / 4096;
    across = (-cosine * offsetX.value + across * offsetZ.value) / 4096;

    if (along < 0) {
        along = 0;
    }

    node = nextSample->heading12 - sample->heading12;
    node = sample->segmentLength1C - ((across * SIN(node)) / 4096);
    if (along > node) {
        along = node;
    }

    position = (along << 10) / node;
    *fraction = position;
    *remaining = node - ((node * (s16)position) >> 10);

    if (across < 0) {
        width = sample->leftWidth18 + sample->leftExtension2E;
        nextWidth = nextSample->leftWidth18 + nextSample->leftExtension2E;
        node = width - nextWidth;
        if (node == 0) {
            *lateral = (across << 10) / width;
        } else {
            node = ((node * *fraction) / 1024) + nextWidth;
            *lateral = (across << 10) / node;
        }
    } else {
        width = sample->rightWidth1A + sample->rightExtension2C;
        nextWidth = nextSample->rightWidth1A + nextSample->rightExtension2C;
        node = width - nextWidth;
        if (node == 0) {
            *lateral = (across << 10) / width;
        } else {
            node = ((node * *fraction) / 1024) + nextWidth;
            *lateral = (across << 10) / node;
        }
    }
}

void func_800468F0(RrRaceVehicle *vehicle)
{
    struct { RrTrackOffsetArgument x, z; } offset;
    offset.x.value = vehicle->pos20[0] - D_800ABEA4[vehicle->trackNodeF0].x08;
    offset.z.value = vehicle->pos20[2] - D_800ABEA4[vehicle->trackNodeF0].z0C;
    func_800464E8(offset.x, offset.z, vehicle->trackNodeF0,
        &vehicle->trackFraction170, &vehicle->lateralOffset172, &vehicle->second);
}
