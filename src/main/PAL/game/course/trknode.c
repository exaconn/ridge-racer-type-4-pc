#include "game/car/carphys2.h"
#include "game/course/trknode.h"
#include "rr/overlay668_camera.h"

int GetTrackHeadingAt(int a, int b)
{
  int n = (a + 1) % TrackNodeCount;
  TrackSample3C *t;
  TrackSample3C *p;
  TrackSample3C *q;
  t = TrackNodes;
  p = &t[a];
  q = &t[n];
  return LerpAngle12(p->heading12, q->heading12, b);
}

void InterpTrackPosition(int a, int *r, int c) {
    int n = (a + 1) % TrackNodeCount;
    int c2 = c;
    int t = 0x400 - c2;
    TrackSample3C *p = &TrackNodes[a];
    TrackSample3C *q = &TrackNodes[n];
    r[0] = (t * p->x08 + c2 * q->x08) / 1024;
    r[1] = (t * p->y10 + c2 * q->y10) / 1024;
    r[2] = (t * p->z0C + c2 * q->z0C) / 1024;
}
