#include "rr/track_lateral.h"

extern s32 func_800924D4(s32 z, s32 x);

s32 func_8004C454(s32 node, s32 lane)
{
    int delta[6];
    int previous;
    RrTrackLaneOffset8 *currentOffset, *previousOffset;
    int previousOffsetBytes, currentOffsetBytes;
    RrTrackSample3C *previousSample, *currentSample;
    previous = (node - 1) % D_800ABECC;
    if (previous < 0) previous += D_800ABECC;
    switch (lane) {
    case 0:
        currentOffset = D_800ACB9C + node;
        previousOffset = D_800ACB9C + previous;
        previousOffsetBytes = previous * (s32)sizeof(RrTrackSample3C);
        currentOffsetBytes = node * (s32)sizeof(RrTrackSample3C);
        previousSample = (RrTrackSample3C *)(previousOffsetBytes + (unsigned long)D_800ABEA4);
        currentSample = (RrTrackSample3C *)(currentOffsetBytes + (unsigned long)D_800ABEA4);
        delta[2] = previousSample->x08 + previousOffset->x - (currentSample->x08 + currentOffset->x);
        delta[3] = previousSample->z0C + previousOffset->z - (currentSample->z0C + currentOffset->z);
        break;
    case 10:
        delta[2] = D_800ABEA4[previous].x08 - D_800ABEA4[node].x08;
        delta[3] = D_800ABEA4[previous].z0C - D_800ABEA4[node].z0C;
        break;
    }
    return (s16)func_800924D4(delta[3], delta[2]);
}
