#include "game/car/carbound.h"
#include "rr_asm.h"
#include "rr/gpu.h"

/* Candidate module: mesh quad packet builders (GT4, G4, F4), 80057A44..80058038. */
#include "types.h"
#include "rr/scene_camera_state.h"
#include "rr/player_cars.h"
#include "game/car/bounds.h"
#include "scratchpad.h"
#include "game/camera/camview.h"
#include "game/effect/spark.h"
#include "game/gfx/meshquad.h"
#include "rr/overlay665_render.h"
#include "game/gfx/gfxotag.h"
#include "game/effect/carfx.h"
#include "rr/race_types.h"
#define RTPT() RR_GTE_RTPT_MEM()
#define RTPS() RR_GTE_RTPS_MEM()
#define NCLIP() RR_GTE_NCLIP_MEM()
#define AVSZ4() RR_GTE_AVSZ4_MEM()

MeshQuadGT4 *DrawMeshQuadGT4(MeshQuadGT4 *p, s32 *s);
MeshQuadG4 *DrawMeshQuadG4(MeshQuadG4 *p, s32 *s);
MeshQuadF4 *DrawMeshQuadF4(MeshQuadF4 *p, s32 *s);
void DrawMeshPrimList(int arg0, int arg1, int arg2);

int StepCarShowcaseScene(int a0)
{
  int sp10[2];
  int s1;
  s1 = 4;
  {
    u32 extent = RaceContext.camera24C->extentA0;
    RaceContext.camera24C->distance100 = extent - s1 - (CarBoundsTable[0]->unk08 + PlayerCars[0].halfSpan10E / 3);
  }
  s1 = StepCarShowcasePhysics(&PlayerCars[0], RaceContext.camera24C->frame104, a0, sp10);
  UpdateSparkEffects();
  ScaleMatrixRow1(2, (MatrixWords32 *)SCRATCH_ROT_MATRIX);
  DrawGarageFloorReflection();
  RaceContext.camera24C->frame104 = RaceContext.camera24C->frame104 + 1U;
  RaceContext.camera24C->phase108 = RaceContext.camera24C->phase108 + 1U;
  if (RaceContext.camera24C->frame104 >= 0x322)
  {
    RaceContext.camera24C->frame104 = 0x321;
  }
  if (RaceContext.camera24C->phase108 >= 0x516)
  {
    RaceContext.camera24C->phase108 = 0x515;
  }
  if (Pad0Buttons & 0x800)
  {
    if (RaceContext.camera24C->frame104 >= 0x33)
    {
      s1 = 1;
    }
  }
  return s1;
}

MeshQuadGT4 *DrawMeshQuadGT4(MeshQuadGT4 *p, s32 *s) {
    s32 *g = (s32 *)SCRATCHPAD;
    PolyGT4WordPacket *out = (PolyGT4WordPacket *)g[0];
    s32 sh = g[0x18];
    u8 *verts = (u8 *)s[0];
    s32 n = s[1];
    s32 nv;
    /* ASM_BARRIER: Keep paired packet word copies in $v1/$a0 around GTE pipeline gaps. */
    register s32 x __asm__("$3");
    /* ASM_BARRIER: Keep second word of paired packet copy in $a0. */
    register s32 y __asm__("$4");
    PolyGT4WordPacket *t1 = out;
    if (n-- > 0) {
        s32 c3c = 0x3C;
        do {
            u8 *v0 = verts + (p->v0 << 3);
            u8 *v1 = verts + (p->v1 << 3);
            u8 *v2 = verts + (p->v2 << 3);
            RR_GTE_LWC2_V012_3P(v0, v1, v2);
            t1->len = 0xC;
            RTPT();
            ((u8 *)t1)[7] = c3c;
            {
                x = p->color08; y = p->color0C;
                t1->color04 = x;
                t1->color10 = y;
            }
            NCLIP();
            RR_GTE14_SWC2_MAC0(s + 6);
            RR_GTE14_SWC2_SXYZ(s + 2);
            if (s[6] > 0) {
                u8 *v3 = verts + (p->v3 << 3);
                s32 x2, y2;
                RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, v3);
                x = s[2]; y = s[3];
                RTPS();
                t1->xy0 = x;
                t1->xy1 = y;
                RR_GTE_SWC2_AT(14, 0, s + 5);
                x2 = s[4]; y2 = s[5];
                AVSZ4();
                RR_GTE14_SWC2_OTZ(s + 8);
                s[8] = s[8] >> sh;
                if (s[8] < 0x1C1) {
                    t1->xy2 = x2;
                    t1->xy3 = y2;
                    {
                        x = p->uvClut18; y = p->uvTpage1C;
                        t1->uvClut0C = x;
                        t1->uvTpage18 = y;
                    }
                    {
                        x = p->uv2; y = p->uv3;
                        t1->uv2 = x;
                        t1->uv3 = y;
                    }
                    {
                        x = p->color10; y = p->color14;
                        t1->color1C = x;
                        t1->color28 = y;
                    }
                    {
                        s32 *ot = (s32 *)((char *)(s[0x15] * 4 + ((s8)p->otBias25 * 4 + (s[8] * 4 + s[0x16]))) + 0x200);
                        nv = 0xFFFFFF;
                        *(s32 *)t1 = (*(s32 *)t1 & 0xFF000000) | (*ot & nv);
                    }
                    {
                        s32 *ot = (s32 *)((char *)(s[0x15] * 4 + ((s8)p->otBias25 * 4 + (s[8] * 4 + s[0x16]))) + 0x200);
                        *ot = (*ot & 0xFF000000) | ((s32)t1 & 0xFFFFFF);
                    }
                    t1++;
                }
            }
            p++;
        } while (n-- > 0);
    }
    *(s32 *)SCRATCHPAD = t1;
    return p;
}

MeshQuadG4 *DrawMeshQuadG4(MeshQuadG4 *p, s32 *s) {
    s32 *g = (s32 *)SCRATCHPAD;
    PolyG4WordPacket *out = (PolyG4WordPacket *)g[0];
    s32 sh = g[0x18];
    u8 *verts = (u8 *)s[0];
    s32 n = s[1];
    s32 nv;
    /* ASM_BARRIER: Keep paired packet word copies in $v1/$a0 around GTE pipeline gaps. */
    register s32 x __asm__("$3");
    /* ASM_BARRIER: Keep second word of paired packet copy in $a0. */
    register s32 y __asm__("$4");
    PolyG4WordPacket *t1 = out;
    if (n-- > 0) {
        s32 c3c = 0x38;
        do {
            u8 *v0 = verts + (p->v0 << 3);
            u8 *v1 = verts + (p->v1 << 3);
            u8 *v2 = verts + (p->v2 << 3);
            RR_GTE_LWC2_V012_3P(v0, v1, v2);
            t1->len = 8;
            RTPT();
            ((u8 *)t1)[7] = c3c;
            {
                x = p->color08; y = p->color0C;
                t1->color04 = x;
                t1->color0C = y;
            }
            NCLIP();
            RR_GTE14_SWC2_MAC0(s + 6);
            RR_GTE14_SWC2_SXYZ(s + 2);
            if (s[6] > 0) {
                u8 *v3 = verts + (p->v3 << 3);
                s32 x2, y2;
                RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, v3);
                x = s[2]; y = s[3];
                RTPS();
                t1->xy0 = x;
                t1->xy1 = y;
                RR_GTE_SWC2_AT(14, 0, s + 5);
                x2 = s[4]; y2 = s[5];
                AVSZ4();
                RR_GTE14_SWC2_OTZ(s + 8);
                s[8] = s[8] >> sh;
                if (s[8] < 0x1C1) {
                    t1->xy2 = x2;
                    t1->xy3 = y2;
                    {
                        x = p->color10; y = p->color14;
                        t1->color14 = x;
                        t1->color1C = y;
                    }
                    {
                        s32 *ot = (s32 *)((char *)(s[0x15] * 4 + ((s8)p->otBias19 * 4 + (s[8] * 4 + s[0x16]))) + 0x200);
                        nv = 0xFFFFFF;
                        *(s32 *)t1 = (*(s32 *)t1 & 0xFF000000) | (*ot & nv);
                    }
                    {
                        s32 *ot = (s32 *)((char *)(s[0x15] * 4 + ((s8)p->otBias19 * 4 + (s[8] * 4 + s[0x16]))) + 0x200);
                        *ot = (*ot & 0xFF000000) | ((s32)t1 & 0xFFFFFF);
                    }
                    t1++;
                }
            }
            p++;
        } while (n-- > 0);
    }
    *(s32 *)SCRATCHPAD = t1;
    return p;
}

MeshQuadF4 *DrawMeshQuadF4(MeshQuadF4 *p, s32 *s) {
    s32 *g = (s32 *)SCRATCHPAD;
    PolyF4Packet *out = (PolyF4Packet *)g[0];
    s32 sh = g[0x18];
    u8 *verts = (u8 *)s[0];
    s32 n = s[1];
    s32 nv;
    s32 x;
    s32 y;
    PolyF4Packet *t1 = out;
    if (n-- > 0) {
        s32 c3c = 0x28;
        do {
            u8 *v0 = verts + (p->v0 << 3);
            u8 *v1 = verts + (p->v1 << 3);
            u8 *v2 = verts + (p->v2 << 3);
            RR_GTE_LWC2_V012_3P(v0, v1, v2);
            t1->len = 5;
            ((u8 *)t1)[7] = c3c;
            RTPT();
            {
                s32 w = p->color08;
                t1->colorCode04 = w;
            }
            NCLIP();
            RR_GTE14_SWC2_MAC0(s + 6);
            RR_GTE14_SWC2_SXYZ(s + 2);
            {
                u8 *v3 = verts + (p->v3 << 3);
                RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, v3);
                x = s[2]; y = s[3];
                RTPS();
                t1->xy0 = x;
                t1->xy1 = y;
                RR_GTE_CMD_2NOP(0x4B400006);
                RR_GTE_SWC2_AT(14, 0, s + 5);
                RR_GTE_SWC2_AT(24, 0, s + 7);
                if (s[6] > 0 || s[7] < 0) {
                    x = s[4]; y = s[5];
                    AVSZ4();
                    RR_GTE14_SWC2_OTZ(s + 8);
                    s[8] = s[8] >> sh;
                    if (s[8] < 0x1C1) {
                        t1->xy2 = x;
                        t1->xy3 = y;
                        {
                            s32 *ot = (s32 *)((char *)(s[0x15] * 4 + ((s8)p->otBias0D * 4 + (s[8] * 4 + s[0x16]))) + 0x200);
                            nv = 0xFFFFFF;
                            *(s32 *)t1 = (*(s32 *)t1 & 0xFF000000) | (*ot & nv);
                        }
                        {
                            s32 *ot = (s32 *)((char *)(s[0x15] * 4 + ((s8)p->otBias0D * 4 + (s[8] * 4 + s[0x16]))) + 0x200);
                            *ot = (*ot & 0xFF000000) | ((s32)t1 & 0xFFFFFF);
                        }
                        t1++;
                    }
                }
            }
            p++;
        } while (n-- > 0);
    }
    *(s32 *)SCRATCHPAD = t1;
    return p;
}

void DrawMeshPrimList(int arg0, int arg1, int arg2) {
    int *g = (int *)(SCRATCHPAD + 0x7C);
    int *a;
    int v;
    int t;
    g[0x54 / 4] = arg2;
    g[0x58 / 4] = ((int *)SCRATCHPAD)[1];
    t = arg0 + arg1 * 0xC;
    g[0] = ((int *)t)[1];
    a = *(int **)(t + RR_FIELD_OFFSET(RroModelTable, entries[0].commands));
    v = *a;
    a++;
    if (v != 0) {
        do {
            g[1] = v >> 16;
            switch (v & 0xFFFF) {
            case 0:
                a = DrawMeshQuadF4(a, g);
                break;
            case 12:
                a = DrawMeshQuadGT4(a, g);
                break;
            case 8:
                a = DrawMeshQuadG4(a, g);
                break;
            }
            v = *a;
            a++;
        } while (v != 0);
    }
}
