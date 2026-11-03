#include "rr/track_lateral.h"

/* Interpolate the lateral road position in the horizontal plane. */
void func_80047230(const RrTrackSample3C *first, const RrTrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output)
{
    int firstPoint[4];
    int secondPoint[4];
    /* ASM_BARRIER: retain the lateral magnitude in t1. */
    register int offset __asm__("$9");
    /* ASM_BARRIER: retain the intermediate component in v1. */
    register int component __asm__("$3");
    const RrTrackSample3C *sample;
    int *result;

    result = output;
    offset = lateral;
    if (lateral < 0) offset = -offset;
    sample = first;
    if (lateral < 0) {
        component = sample->leftOffsetX04 * (sample->leftWidth18 + sample->leftExtension2E) / sample->leftWidth18;
        firstPoint[0] = component;
        firstPoint[0] = (component * offset) / 1024 + sample->x08;
        component = sample->leftOffsetZ06 * (sample->leftWidth18 + sample->leftExtension2E) / sample->leftWidth18;
        firstPoint[2] = component;
        firstPoint[2] = (component * offset) / 1024 + sample->z0C;
        component = second->leftOffsetX04 * (second->leftWidth18 + second->leftExtension2E) / second->leftWidth18;
        secondPoint[0] = component;
        secondPoint[0] = (component * offset) / 1024 + second->x08;
        component = second->leftOffsetZ06 * (second->leftWidth18 + second->leftExtension2E) / second->leftWidth18;
    } else {
        /* ASM_BARRIER: retain a separate base for the first sample. */
        __asm__ volatile ("" : "=r"(sample) : "0"(sample));
        component = sample->rightOffsetX00 * (sample->rightWidth1A + sample->rightExtension2C) / sample->rightWidth1A;
        firstPoint[0] = component;
        firstPoint[0] = (component * offset) / 1024 + sample->x08;
        component = sample->rightOffsetZ02 * (sample->rightWidth1A + sample->rightExtension2C) / sample->rightWidth1A;
        firstPoint[2] = component;
        firstPoint[2] = (component * offset) / 1024 + sample->z0C;
        component = second->rightOffsetX00 * (second->rightWidth1A + second->rightExtension2C) / second->rightWidth1A;
        secondPoint[0] = component;
        secondPoint[0] = (component * offset) / 1024 + second->x08;
        component = second->rightOffsetZ02 * (second->rightWidth1A + second->rightExtension2C) / second->rightWidth1A;
    }

    secondPoint[2] = component;
    secondPoint[2] = (component * offset) / 1024 + second->z0C;
    /* ASM_BARRIER: keep final firstPoint/secondPoint reloads after the secondPoint[2] store */
    __asm__ volatile ("");
    result[0] = (secondPoint[0] - firstPoint[0]) * fraction / 1024 + firstPoint[0];
    result[2] = (secondPoint[2] - firstPoint[2]) * fraction / 1024 + firstPoint[2];
}

/* The camera path also needs the height of the centre line. */
void func_80047610(const RrTrackSample3C *first, const RrTrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output)
{
    int firstPoint[4];
    int secondPoint[4];
    /* ASM_BARRIER: retain the lateral magnitude in t1. */
    register int offset __asm__("$9");
    register int component __asm__("$3");
    const RrTrackSample3C *sample;
    int *result;

    result = output;
    offset = lateral;
    if (lateral < 0) offset = -offset;
    sample = first;
    if (lateral < 0) {
        component = sample->leftOffsetX04 * (sample->leftWidth18 + sample->leftExtension2E) / sample->leftWidth18;
        firstPoint[0] = component;
        firstPoint[0] = (component * offset) / 1024 + sample->x08;
        firstPoint[1] = sample->y10;
        component = sample->leftOffsetZ06 * (sample->leftWidth18 + sample->leftExtension2E) / sample->leftWidth18;
        firstPoint[2] = component;
        firstPoint[2] = (component * offset) / 1024 + sample->z0C;
        component = second->leftOffsetX04 * (second->leftWidth18 + second->leftExtension2E) / second->leftWidth18;
        secondPoint[0] = component;
        secondPoint[0] = (component * offset) / 1024 + second->x08;
        secondPoint[1] = second->y10;
        component = second->leftOffsetZ06 * (second->leftWidth18 + second->leftExtension2E) / second->leftWidth18;
    } else {
        /* ASM_BARRIER: retain a separate base for the first sample. */
        __asm__ volatile ("" : "=r"(sample) : "0"(sample));
        component = sample->rightOffsetX00 * (sample->rightWidth1A + sample->rightExtension2C) / sample->rightWidth1A;
        firstPoint[0] = component;
        firstPoint[0] = (component * offset) / 1024 + sample->x08;
        firstPoint[1] = sample->y10;
        component = sample->rightOffsetZ02 * (sample->rightWidth1A + sample->rightExtension2C) / sample->rightWidth1A;
        firstPoint[2] = component;
        firstPoint[2] = (component * offset) / 1024 + sample->z0C;
        component = second->rightOffsetX00 * (second->rightWidth1A + second->rightExtension2C) / second->rightWidth1A;
        secondPoint[0] = component;
        secondPoint[0] = (component * offset) / 1024 + second->x08;
        secondPoint[1] = second->y10;
        component = second->rightOffsetZ02 * (second->rightWidth1A + second->rightExtension2C) / second->rightWidth1A;
    }

    secondPoint[2] = component;
    secondPoint[2] = (component * offset) / 1024 + second->z0C;
    /* ASM_BARRIER: keep final firstPoint/secondPoint reloads after the secondPoint[2] store */
    __asm__ volatile ("");
    result[0] = (secondPoint[0] - firstPoint[0]) * fraction / 1024 + firstPoint[0];
    result[1] = (secondPoint[1] - firstPoint[1]) * fraction / 1024 + firstPoint[1];
    result[2] = (secondPoint[2] - firstPoint[2]) * fraction / 1024 + firstPoint[2];
}

typedef struct {
    short x, y;
    int z;
} TrackLaneOffset;

extern TrackLaneOffset *D_800ACB9C;


void func_80047A50(RrRaceVehicle *vehicle) {
    int firstPoint[4];
    int secondPoint[4];
    int target[4];
    RrRaceVehicleControl *control;
    int fraction;
    int firstNode;
    int secondNode;
    RrTrackSample3C *firstSample;
    RrTrackSample3C *secondSample;
    TrackLaneOffset *firstOffset;
    TrackLaneOffset *secondOffset;
    int delta;

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
            firstPoint[0] = firstSample->x08 + firstOffset->x;
            firstPoint[2] = firstSample->z0C + firstOffset->y;
            secondOffset = &D_800ACB9C[secondNode];
            secondPoint[0] = secondSample->x08 + secondOffset->x;
            secondPoint[2] = secondSample->z0C + secondOffset->y;
            target[0] = (secondPoint[0] - firstPoint[0]) * fraction / 1024 + firstPoint[0];
            target[2] = (secondPoint[2] - firstPoint[2]) * fraction / 1024 + firstPoint[2];
            break;
        case 10:
            func_80047230(firstSample, secondSample, control->laneStart3A, fraction, target);
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
        func_80047230(firstSample, secondSample, control->laneStart3A, fraction, target);
    }
    vehicle->pathDeltaX224 = target[0] - vehicle->pos20[0];
    vehicle->pathDeltaZ228 = target[2] - vehicle->pos20[2];
}
