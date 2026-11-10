#include "rr/rro_model.h"
#include "game/gfx/model.h"
#include "psyq/libgte/gtemat4.h"
#include "game/car/carwheel.h"
#include "rr/frame_storage_types.h"
#include "game/camera/viewcam.h"
#include "types.h"
#include "rr/scene_camera_state.h"
#include "rr/player_cars.h"
#include "scratchpad.h"
#include "rr/geometry.h"
#include "game/gfx/gfxdraw.h"
#include "game/gfx/meshquad.h"
#include "game/gfx/scene1.h"
#include "game/camera/camview.h"
#include "rr/race_types.h"
#include "rr/matrix_workspace.h"
#include "rr/frame_storage_types.h"
#include "game/math/rotmat.h"


typedef struct {
    u8 reserved00[0x70];
    u32 ot70;
} DrawBufOtAt;

extern void UpdateCarWorldTransform();

void DrawGarageFloorReflection(void)
{
    struct { MatrixWords32 m18; MatrixWords32 m38; char m58[0x28]; VectorWords16 v80; PreviewColor color90; u8 reserved94[24]; } L;
    int *g = (int *)SCRATCHPAD;
    int s6;
    int a1;
    int *a3;
    register DrawBufOtAt *a0 __asm__("$4");
    register unsigned m __asm__("$6");
    unsigned hm;
    int d;
    int t;

    s6 = g[0x18];
    g[0x18] = 3;
    SetupObjectLightMatrices(&RaceContext.camera24C->pose00, &RaceContext.camera24C->lighting78, &L.m18, &L.m38);
    SelectModelBank(8);
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&RaceContext.camera24C->pose00.position00, (s32 *)&L.m18);
    g[0x1E] = 0;
    SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, ModelBankFormat < 1, 0, 0);
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&RaceContext.camera24C->pose00.position00, (s32 *)&L.m18);
    DrawMeshPrimList(RaceContext.model560, 0, 0);
    TransformPoint(&L.m38, &RaceContext.camera24C->pose00.position00.xyz, &RaceContext.camera24C->parts18[0].position00.xyz, &L.v80.xyz, (MatrixWords32 *)L.m58);
    BuildRotMatrixY(L.m58, -RaceContext.camera24C->parts18[0].rotation10.y);
    MulMatrix2((const MatrixWords32 *)(&L.m18), (MatrixWords32 *)(L.m58));
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&L.v80, (s32 *)L.m58);
    g[0x1E] = 0;
    SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, 1, 0, 0);
    TransformPoint(&L.m38, &RaceContext.camera24C->pose00.position00.xyz, &RaceContext.camera24C->parts18[1].position00.xyz, &L.v80.xyz, (MatrixWords32 *)L.m58);
    BuildRotMatrixY(L.m58, -RaceContext.camera24C->parts18[1].rotation10.y);
    MulMatrix2((const MatrixWords32 *)(&L.m18), (MatrixWords32 *)(L.m58));
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&L.v80, (s32 *)L.m58);
    a1 = 1;
    g[0x1E] = 0;
    if (ModelBankFormat >= 3)
        a1 = 2;
    SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, 0);
    TransformPoint(&L.m38, &RaceContext.camera24C->pose00.position00.xyz, &RaceContext.camera24C->parts18[3].position00.xyz, &L.v80.xyz, (MatrixWords32 *)L.m58);
    BuildRotMatrixX(L.m58, -RaceContext.camera24C->parts18[3].rotation10.x);
    MulMatrix2((const MatrixWords32 *)(&L.m18), (MatrixWords32 *)(L.m58));
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&L.v80, (s32 *)L.m58);
    DrawMeshPrimList(RaceContext.model560, 2, 0);
    TransformPoint(&L.m38, &RaceContext.camera24C->pose00.position00.xyz, &RaceContext.camera24C->parts18[2].position00.xyz, &L.v80.xyz, (MatrixWords32 *)L.m58);
    BuildRotMatrixY(L.m58, -RaceContext.camera24C->parts18[2].rotation10.y);
    MulMatrix2((const MatrixWords32 *)(&L.m18), (MatrixWords32 *)(L.m58));
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&L.v80, (s32 *)L.m58);
    DrawMeshPrimList(RaceContext.model560, RaceContext.camera24C->mirrorA4 * 2 | 1, 0);
    g[0x18] = s6;
    L.color90.channels.r = 0x80;
    L.color90.channels.g = 0x80;
    L.color90.channels.b = 0x80;
    g[1] = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
    UpdateCarWorldTransform(PlayerCars);
    DrawPreviewCar(PlayerCars, L.color90.argument, 0x80, 1);
    t = RotTransPersOrigin(&L.m18, &PlayerCars[0].position20);
    t = (t >> 3) + 0x64;
    m = 0xFFFFFF;
    t = t << 2;
    a0 = (DrawBufOtAt *)t;
    hm = 0xFF000000;
    d = CurrentDrawBuf;
    a3 = (int *)g[1];
    a0 = (DrawBufOtAt *)((char *)a0 + d);
    *a3 = (*a3 & hm) | (a0->ot70 & m);
    a0->ot70 = (a0->ot70 & hm) | ((g[1] + 0xAFC) & m);
    g[1] = d + 0x70;
}

s32 FindCurveSegment(PairS16 *p, s32 a1) {
    int t = a1 / 10;
    int i = 0;
    while (p[i].b >= 0) {
        if (t < p[i].b) break;
        i++;
    }
    return i;
}

s32 InterpCurveLinear(PairS16 *t, s32 idx, s32 x) {
    PairS16 *p;
    if (idx == 0) return t[0].a;
    p = (PairS16 *)(idx * 4 + (s32)t);
    if (p->b < 0) return p->a;
    return (p->a - p[-1].a) * (x - p[-1].b * 10) / ((p->b - p[-1].b) * 10) + p[-1].a;
}

s32 InterpCurveEase(s32 a0)
{
  s32 unused[2];
  int i;
  int r;
  int q;
  short *p;
  int v1;
  int a1;
  int a2;
  int a3;
  int d;
  int h;
  i = FindCurveSegment(D_800A0D64, a0);
  if (i == 0)
  {
    return D_800A0D64[0].a;
  }
  p = (short *) ((i * 4) + ((int) D_800A0D64));
  if (p[1] < 0)
  {
    return p[0];
  }
  v1 = p[-1];
  a3 = p[0];
  a1 = p[1] - v1;
  a1 = a1 * 10;
  d = a0 - (v1 * 10);
  a2 = a3 - p[-2];
  h = a1 >> 1;
  if (d < h)
  {
    r = (((d * d) * a2) / 2) / (h * h);
  }
  else
  {
    d = a1 - d;
    q = (((d * d) * a2) / 2) / (h * h);
    r = a3 - q;
  }
  q = r;
  return q;
}
