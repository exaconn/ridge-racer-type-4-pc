#include "game/camera/viewcam.h"
#include "scratchpad.h"
#include "rr/presentation.h"
#include "game/gfx/meshquad.h"
#include "game/gfx/ring2.h"
#include "rr/ring_renderer.h"
#include "rr/overlay665_render.h"
struct RingTransformPose18 { int x, y, z, pad; short rx, ry, rz; };
struct RingScaleVec12 { int x, y, z; };

void DrawRingEffects(void) {
    int i;
    struct {
        struct RingTransformPose18 a;
        struct RingScaleVec12 s;
        int align1C;
        int m[8];
        int v[8];
    } l;
    ScrollTrack40 *scroll = &RaceContext.controller3D8->scroll;
    for (i = 0; i < 2; i++) {
        if (scroll->channel[i].glyph > 0) {
            l.a.y = 0;
            l.a.x = 0;
            l.a.z = (s16)scroll->channel[i].offset;
            l.a.rz = 0;
            l.a.ry = 0;
            l.a.rx = 0;
            SetupObjectLightMatrices(&l.a, &RaceContext.controller3D8->lighting20, l.m, l.v);
            l.s.z = 0x800;
            l.s.y = 0x800;
            l.s.x = 0x800;
            ScaleMatrix(l.m, &l.s);
            if (scroll->channel[i].glyph < 0x18) {
                SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&l.a, l.m);
                DrawMeshPrimList(RaceContext.ringModel564, scroll->channel[i].glyph + 1, 0);
            } else {
                DrawRing(&l.a, 0x300);
            }
        }
    }
}
