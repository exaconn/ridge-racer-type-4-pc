#include "psyq/libgte/gtemat5.h"
#include "rr/frame_storage_types.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat3.h"
#include "scratchpad.h"
#include "rr/race.h"
#include "game/ui/window1.h"
#include "game/ui/hudmap2.h"
#include "game/ui/uipart1.h"
#include "game/data/ldscn1.h"
#include "game/race/raceinfo.h"
#include "game/race/ranking.h"
#include "rr/track_sample_types.h"
#include "rr/overlay662_distance.h"
#include "rr/matrix_workspace.h"
#include "game/race/raceset.h"
#include "game/math/rotmat.h"
#include "game/ui/badge1.h"
#include "rr/badge_rows.h"
#include "rr/menu_label_render.h"
#include "game/data/cdfile2.h"
#include "rr/trig.h"

typedef struct {
    int reserved00[10];
    short reserved28;
    signed char f2A;
    char reserved2B;
} IntroOptionCursor;

typedef struct {
    int reserved00[42];
    int fA8;
} IntroContextCursor;

extern signed char D_8009F688[];
extern int D_8009F654[];
extern int D_8009F66C[];
extern short GameModeType;
extern short CourseIndex;
extern int NumLaps;
extern short D_8009F678[];
extern int CourseBestLapRecords[];
extern int CourseBestLap[][8];
extern short NumCars;
extern short ComDifficultyLevel;
extern int EmitSpinningWheelIcon();
extern TrackSample3C *TrackNodes;
extern int TrackNodeCount;
extern short ReverseCourse;
extern char Options[];
extern short D_8010BFD0[];

void DrawCourseInfoPanel(int arg0, int arg1);
void DrawCourseMapPreview(int unusedSelection,int unusedMode);
void UpdateRaceIntroCamera(RaceVehicle *a0, int a1);

void DrawCourseInfoPanel(int arg0, int arg1) {
    char sp28[8];
    int i;
    int s4;
    int p;
    char *b;

    p = *(int *)SCRATCHPAD;
    for (i = 0; i < 4; i++) {
        int v = D_8009F688[i + (arg1 << 2)];
        int y;
        if (v == -1) {
            break;
        }
        y = 0x128 + i * 0x10;
        p = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], p, 0x16, y, &D_8009F654[v], 0x7FA4, 1);
        if (i == 1) {
            p = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], p, 0x4A, y, &D_8009F654[8], 0x7FA4, 1);
        }
    }

    s4 = arg0 >= 8;
    arg0 &= 7;
    p = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], p, 0x3A, 0x104, &D_8009F66C[s4], s4 + 0x75E4, 0);
    p = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], p, 0x59);
    *(int *)SCRATCHPAD = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], p, 0x10, 0xE0, 2, 0x88, 0x4C, 0x4C, 0x4C);

    if (GameModeType < 2) {
        NumLaps = CourseLapCounts[0][CourseIndex + (GameModeType == 1 ? 8 : 0)];
    }

    i = 7;
    b = sp28 + 7;
    do {
        *b = 0;
        i--;
        b--;
    } while (i >= 0);

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            FormatDecimalDigits(sp28, NumLaps, 0);
        } else {
            FormatDecimalDigits(sp28, D_8009F678[arg0], 3);
        }
        DrawDigitString16(&CurrentDrawBuf->buffer.ordering70.words[0], 0x2E + i * 8, 0x128 + i * 0x10, sp28);
    }

    if (arg1 == 0) {
        for (i = 0; i < 2; i++) {
            int val;
            if (GameModeType == 1) {
                if (i == 0) {
                    val = CourseBestLapRecords[CourseIndex];
                } else {
                    val = CourseBestTotalTimes[CourseIndex];
                }
            } else {
                if (i == 0) {
                    val = CourseBestLap[s4][CourseIndex];
                } else {
                    val = CourseBestTotal[s4][CourseIndex];
                }
            }
            DrawDigitString16(&CurrentDrawBuf->buffer.ordering70.words[0], 0x44 + i * 8, 0x148 + i * 0x10, FormatMillisecondsTime(val));
        }
    } else if (arg1 == 1) {
        i = 7;
        b = sp28 + 7;
        do {
            *b = 0;
            i--;
            b--;
        } while (i >= 0);

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                FormatDecimalDigits(sp28, NumCars - 2, 0);
            } else {
                FormatDecimalDigits(sp28, ComDifficultyLevel + 1, 0);
            }
            DrawDigitString16(&CurrentDrawBuf->buffer.ordering70.words[0], 0x3A - i * 0xC, 0x148 + i * 0x10, sp28);
        }
    }

    *(int *)SCRATCHPAD = EmitSpinningWheelIcon(&CurrentDrawBuf->buffer.ordering70.words[0], *(int *)SCRATCHPAD, 0x16, 0xE4, arg0);
}

void DrawCourseMapPreview(int unusedSelection,int unusedMode) {
    struct { int x, y, z; } v;
    int i, r;
    TrackSample3C *t;
    int b;
    if (ReverseCourse != 0) {
        i = (TrackPreviewTick >> 2) % TrackNodeCount;
    } else {
        int n = TrackNodeCount;
        r = (TrackPreviewTick >> 2) % n;
        r++;
        i = n - r;
    }
    b = TrackNodes;
    t = (TrackSample3C *)(i * 0x3C + b);
    v.x = t->x08;
    v.z = t->z0C;
    TrackPreviewTick += 1;
    DrawMapBitmapIso(&CurrentDrawBuf->buffer.ordering70.words[0], TrackNodesEnd, &v);
}

void UpdateRaceIntroCamera(RaceVehicle *a0, int a1)
{
  int sp10[3];
  MatrixWords32 m20;
  MatrixWords32 m40;
  int *g = (int *) SCRATCHPAD;
  int s0;
  int v0;
  int v1;
  int i;
  int k;
  IntroOptionCursor **new_var;
  IntroOptionCursor *p;
  IntroContextCursor *q;
  short *s5;
  if (a1 >= 0x4B)
  {
    return;
  }
  if (a1 < 2)
  {
    ResetCarSlotBuffers();
    D_8010BFD0[0] = 0;
    D_8010BFD0[2] = -0x100;
    D_8010BFD0[1] = -0x100;
    if ((a1 == 1) && (a0 == PlayerCars))
    {
      i = 0;
      k = 2;
      p = (IntroOptionCursor *)Options;
      q = (IntroContextCursor *)&RaceContext;
      do
      {
        q->fA8 = p->f2A;
        (*(new_var = &p))->f2A = k;
        p = (IntroOptionCursor *)((char *)p + 4);
        q = (IntroContextCursor *)((char *)q + 4);
        i++;
      }
      while (i < 2);
    }
  }
  else
  {
    s0 = (a1 << 11) / 75;
    v0 = rsin(s0);
    s5 = D_8010BFD0;
    if (v0 < 0)
    {
      v0 += 7;
    }
    s5[0] = v0 >> 3;
    v0 = -(rcos(s0) << 9);
    if (v0 < 0)
    {
      v0 += 0xFFF;
    }
    s5[2] = v0 >> 12;
    s5[1] = ((a1 << 8) / 75) - 0x200;
  }
  BuildRotMatrixY(&m20, 0x800 - a0->rot54);
  BuildRotMatrixX(&m40, a0->rot50);
  MulMatrix(&m20, &m40);
  BuildRotMatrixZ(&m40, a0->rot58);
  MulMatrix2(&m20, &m40);
  ApplyMatrix(&m40, (const Vec3s8 *)(D_8010BFD0), (Vec3i *)(sp10));
  g[2] = a0->pos10[0] + sp10[0];
  g[3] = a0->pos10[1] + sp10[1];
  g[4] = a0->pos10[2] + sp10[2];
  v1 = rsin(i = a0->rot54) * 0x60;
  if (v1 < 0)
  {
    v1 += 0xFFF;
  }
  sp10[0] = (a0->pos10[0] + (v1 >> 12)) - g[2];
  sp10[1] = a0->pos10[1] - g[3];
  v1 = rcos(a0->rot54) * 0x60;
  if (v1 < 0)
  {
    v1 += 0xFFF;
  }
  v1 >>= 12;
  sp10[2] = (a0->pos10[2] + v1) - g[4];
  v0 = Atan2Table(sp10[0], sp10[2]);
  g[7] = 0x400 - v0;
  v0 = CordicSqrt((sp10[0] * sp10[0]) + (sp10[2] * sp10[2]));
  v0 = Atan2Table(sp10[1], v0 >> 6);
  g[6] = 0x400 - v0;
  g[8] = 0;
  if (a1 < 0x49)
  {
    return;
  }
  a0->flags1E4 |= 0x100;
  if ((a1 == 0x49) && (a0 == PlayerCars))
  {
    i = 0;
    q = (IntroContextCursor *)&RaceContext;
    p = (IntroOptionCursor *)Options;
    do
    {
      *(unsigned char *)&p->f2A = *(unsigned char *)&q->fA8;
      q = (IntroContextCursor *)((char *)q + 4);
      i++;
      p = (IntroOptionCursor *)((char *)p + 4);
    }
    while (i < 2);
  }
}
