#include "rr/track_lateral.h"
#include "rr/geometry.h"

static __inline__ s32 extend_track_edge(s32 edge, s32 width, s32 extension)
{
    return edge * (width + extension) / width;
}

/* Interpolate the lateral road position in the horizontal plane. */
void func_80047230(const RrTrackSample3C *first, const RrTrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output)
{
    RrVector firstPoint;
    RrVector secondPoint;
    /* ASM_BARRIER: retain the lateral magnitude in t1. */
    register s32 offset __asm__("$9");
    /* ASM_BARRIER: retain the shared Z edge component in v1. */
    register s32 edgeZ __asm__("$3");
    const RrTrackSample3C *sample;

    offset = lateral;
    if (lateral < 0) offset = -offset;
    sample = first;
    if (lateral < 0) {
        firstPoint.x = extend_track_edge(sample->leftOffsetX04, sample->leftWidth18, sample->leftExtension2E);
        firstPoint.x = (firstPoint.x * offset) / 1024 + sample->x08;
        firstPoint.z = extend_track_edge(sample->leftOffsetZ06, sample->leftWidth18, sample->leftExtension2E);
        firstPoint.z = (firstPoint.z * offset) / 1024 + sample->z0C;
        secondPoint.x = extend_track_edge(second->leftOffsetX04, second->leftWidth18, second->leftExtension2E);
        secondPoint.x = (secondPoint.x * offset) / 1024 + second->x08;
        edgeZ = extend_track_edge(second->leftOffsetZ06, second->leftWidth18, second->leftExtension2E);
    } else {
        /* ASM_BARRIER: retain a separate base for the first sample. */
        __asm__ volatile ("" : "=r"(sample) : "0"(sample));
        firstPoint.x = extend_track_edge(sample->rightOffsetX00, sample->rightWidth1A, sample->rightExtension2C);
        firstPoint.x = (firstPoint.x * offset) / 1024 + sample->x08;
        firstPoint.z = extend_track_edge(sample->rightOffsetZ02, sample->rightWidth1A, sample->rightExtension2C);
        firstPoint.z = (firstPoint.z * offset) / 1024 + sample->z0C;
        secondPoint.x = extend_track_edge(second->rightOffsetX00, second->rightWidth1A, second->rightExtension2C);
        secondPoint.x = (secondPoint.x * offset) / 1024 + second->x08;
        edgeZ = extend_track_edge(second->rightOffsetZ02, second->rightWidth1A, second->rightExtension2C);
    }

    secondPoint.z = edgeZ;
    secondPoint.z = (secondPoint.z * offset) / 1024 + second->z0C;
    /* ASM_BARRIER: keep final firstPoint/secondPoint reloads after the secondPoint.z store */
    __asm__ volatile ("");
    output[0] = (secondPoint.x - firstPoint.x) * fraction / 1024 + firstPoint.x;
    output[2] = (secondPoint.z - firstPoint.z) * fraction / 1024 + firstPoint.z;
}

/* The camera path also needs the height of the centre line. */
void func_80047610(const RrTrackSample3C *first, const RrTrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output)
{
    RrVector firstPoint;
    RrVector secondPoint;
    /* ASM_BARRIER: retain the lateral magnitude in t1. */
    register s32 offset __asm__("$9");
    /* ASM_BARRIER: retain the shared Z edge component in v1. */
    register s32 edgeZ __asm__("$3");
    const RrTrackSample3C *sample;

    offset = lateral;
    if (lateral < 0) offset = -offset;
    sample = first;
    if (lateral < 0) {
        firstPoint.x = extend_track_edge(sample->leftOffsetX04, sample->leftWidth18, sample->leftExtension2E);
        firstPoint.x = (firstPoint.x * offset) / 1024 + sample->x08;
        firstPoint.y = sample->y10;
        firstPoint.z = extend_track_edge(sample->leftOffsetZ06, sample->leftWidth18, sample->leftExtension2E);
        firstPoint.z = (firstPoint.z * offset) / 1024 + sample->z0C;
        secondPoint.x = extend_track_edge(second->leftOffsetX04, second->leftWidth18, second->leftExtension2E);
        secondPoint.x = (secondPoint.x * offset) / 1024 + second->x08;
        secondPoint.y = second->y10;
        edgeZ = extend_track_edge(second->leftOffsetZ06, second->leftWidth18, second->leftExtension2E);
    } else {
        /* ASM_BARRIER: retain a separate base for the first sample. */
        __asm__ volatile ("" : "=r"(sample) : "0"(sample));
        firstPoint.x = extend_track_edge(sample->rightOffsetX00, sample->rightWidth1A, sample->rightExtension2C);
        firstPoint.x = (firstPoint.x * offset) / 1024 + sample->x08;
        firstPoint.y = sample->y10;
        firstPoint.z = extend_track_edge(sample->rightOffsetZ02, sample->rightWidth1A, sample->rightExtension2C);
        firstPoint.z = (firstPoint.z * offset) / 1024 + sample->z0C;
        secondPoint.x = extend_track_edge(second->rightOffsetX00, second->rightWidth1A, second->rightExtension2C);
        secondPoint.x = (secondPoint.x * offset) / 1024 + second->x08;
        secondPoint.y = second->y10;
        edgeZ = extend_track_edge(second->rightOffsetZ02, second->rightWidth1A, second->rightExtension2C);
    }

    secondPoint.z = edgeZ;
    secondPoint.z = (secondPoint.z * offset) / 1024 + second->z0C;
    /* ASM_BARRIER: keep final firstPoint/secondPoint reloads after the secondPoint.z store */
    __asm__ volatile ("");
    output[0] = (secondPoint.x - firstPoint.x) * fraction / 1024 + firstPoint.x;
    output[1] = (secondPoint.y - firstPoint.y) * fraction / 1024 + firstPoint.y;
    output[2] = (secondPoint.z - firstPoint.z) * fraction / 1024 + firstPoint.z;
}

void func_80047A50(RrRaceVehicle *vehicle) {
    RrVector firstPoint;
    RrVector secondPoint;
    RrVector target;
    RrRaceVehicleControl *control;
    s32 fraction;
    s32 firstNode;
    s32 secondNode;
    RrTrackSample3C *firstSample;
    RrTrackSample3C *secondSample;
    RrTrackLaneOffset8 *firstOffset;
    RrTrackLaneOffset8 *secondOffset;
    s32 delta;

    control = (RrRaceVehicleControl *)((u8 *)vehicle + RR_RACE_CONTROL_OFFSET);
    fraction = 0x400 - vehicle->trackFraction170;
    firstNode = vehicle->trackNodeF0 - fraction / 1024;
    if (firstNode < 0) {
        firstNode += D_800ABECC;
    }
    secondNode = firstNode - 1;
    if (secondNode < 0) {
        secondNode += D_800ABECC;
    }
    firstSample = &D_800ABEA4[firstNode];
    secondSample = &D_800ABEA4[secondNode];
    /* ASM_BARRIER: form both sample pointers before selecting the lane state. */
    __asm__ volatile("");
    fraction &= 0x3FF;
    if (control->laneActive6A == 0) {
        switch (control->laneTimer38) {
        case 0:
            firstOffset = &D_800ACB9C[firstNode];
            firstPoint.x = firstSample->x08 + firstOffset->x;
            firstPoint.z = firstSample->z0C + firstOffset->z;
            secondOffset = &D_800ACB9C[secondNode];
            secondPoint.x = secondSample->x08 + secondOffset->x;
            secondPoint.z = secondSample->z0C + secondOffset->z;
            target.x = (secondPoint.x - firstPoint.x) * fraction / 1024 + firstPoint.x;
            target.z = (secondPoint.z - firstPoint.z) * fraction / 1024 + firstPoint.z;
            break;
        case 10:
            func_80047230(firstSample, secondSample, control->laneStart3A, fraction, (s32 *)&target);
            break;
        }
    } else {
        delta = control->laneDelta6C >= 0 ? control->laneDelta6C : -control->laneDelta6C;
        if (delta < 0x10) {
            control->laneActive6A = 0;
            control->laneStart3A = control->laneTarget3C;
            if (control->laneMode88 == 3) {
                control->laneMode88 = 0;
            }
        } else {
            if (control->laneDelta6C > 0) {
                control->laneDelta6C -= 14;
                if (control->laneDelta6C < 0) {
                    control->laneDelta6C = 0;
                }
            } else {
                control->laneDelta6C += 14;
                if (control->laneDelta6C > 0) {
                    control->laneDelta6C = 0;
                }
            }
            control->laneStart3A = control->laneTarget3C - control->laneDelta6C;
            if (control->laneStart3A < -800) {
                control->laneStart3A = -800;
            }
            if (control->laneStart3A > 800) {
                control->laneStart3A = 800;
            }
            control->laneDelta6C = control->laneTarget3C - control->laneStart3A;
        }
        func_80047230(firstSample, secondSample, control->laneStart3A, fraction, (s32 *)&target);
    }
    vehicle->pathDeltaX224 = target.x - vehicle->pos20[0];
    vehicle->pathDeltaZ228 = target.z - vehicle->pos20[2];
}
