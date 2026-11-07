#include "types.h"
#include "rr/track_sample_types.h"
#include "rr/race_types.h"
#include "m2c_macros.h"
#include "game/car/carphys2.h"
#include "game/course/trknode.h"
#include "rr/menu_overlay_runtime.h"

void GetSmoothedTrackHeading(int a, int b);
int AngleDiffAbs(int a0, int a1);
s32 AngleDiffSigned(s32 arg0, s32 arg1);
void ComputeStartDistance(char *vehicle, int direction);

void GetSmoothedTrackHeading(int a, int b)
{
    int s2;
    int s0;
    int x;
    s2 = GetTrackHeadingAt(a, b);
    x = a - 2;
    if (x < 0) {
        int d = TrackNodeCount - 2;
        x = d + a;
    }
    s0 = LerpAngle12(s2, GetTrackHeadingAt(x, b), 0x200);
    LerpAngle12(s0, LerpAngle12(s2, GetTrackHeadingAt((a + 2) % TrackNodeCount, b), 0x200), 0x200);
}

int AngleDiffAbs(int a0, int a1) {
    int d;
    a0 &= 0xFFF;
    a1 &= 0xFFF;
    if (a0 < a1) d = a1 - a0; else d = a0 - a1;
    if (d > 0x800) d = 0x1000 - d;
    return d;
}

s32 AngleDiffSigned(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;

    temp_a0 = arg0 & 0xFFF;
    temp_v0 = arg1 & 0xFFF;
    var_v1 = temp_a0 < temp_v0;
    if (var_v1 != 0) {
        var_a0 = temp_v0 - temp_a0;
    } else {
        var_a0 = temp_a0 - temp_v0;
    }
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
        var_v1 = var_v1 == 0;
    }
    var_v0 = var_a0;
    if (var_v1 == 0) {
        var_v0 = -var_v0;
    }
    return var_v0;
}

void ComputeStartDistance(char *vehicle, int direction)
{
    int node;
    int target;
    int distance;
    int wrapped;
    int previous;
    *(s32 *)(vehicle + 0x184) = 0;
    node = *TrackStartNode;
    target = *(s32 *)(vehicle + 0xF0);
    distance = 0;
    if (ReverseCourse == 0) {
        if (direction == 0) {
            while (1) {
                wrapped = node % TrackNodeCount;
                distance -= TrackNodes[wrapped].segmentLength1C;
                if (target == wrapped) break;
                node++;
            }
        } else {
            while (1) {
                node--;
                previous = node;
                if (node < 0) previous = TrackNodeCount + node;
                node = previous;
                if (target == node) break;
                distance += TrackNodes[node].segmentLength1C;
            }
        }
    } else {
        if (direction == 0) {
            do {
                node--;
                previous = node;
                if (node < 0) previous = TrackNodeCount + node;
                node = previous;
                distance -= TrackNodes[node].segmentLength1C;
            } while (target != node);
        } else {
            while (1) {
                if (target == node % TrackNodeCount) break;
                node++;
                distance += TrackNodes[node % TrackNodeCount].segmentLength1C;
            }
        }
    }
    *(s32 *)(vehicle + 0x184) = distance;
}
