#include "rr/rro_model.h"
#include "game/gfx/model.h"
#include "psyq/libgte/gtemat5.h"
#include "rr/frame_storage_types.h"
#include "game/camera/viewcam.h"
#include "types.h"
#include "scratchpad.h"
#include "rr/projection_state.h"
#include "game/course/trkintp2.h"
#include "game/demo/opening.h"
#include "game/gfx/gfxdraw.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndslot.h"
#include "psyq/libgpu/gpuenv.h"
#include "game/gfx/gfxotag.h"
#include "game/ui/hudmap2.h"
#include "rr/hud_number.h"
#include "rr/bios_devices.h"
#include "rr/race_setup.h"
#include "rr/race_types.h"
#include "rr/gpu.h"
#include "rr/frame_storage_types.h"
#include "rr/audio_loop.h"
#include "rr/screen_tiles.h"
#include "game/sound/cdstart.h"
#include "rr/menu_transition.h"
#include "psyq/libc/rand.h"
#include "game/sound/cdplay.h"


typedef struct {
    int x, y, z, w;
} V4;

typedef struct {
    short x, y, z, w;
} V8;

typedef struct {
    V4 f0;
    V8 f10;
    int f18;
    int f1C;
    int f20;
    int f24;
} Elem28;


typedef struct {
    s32 unk00[12];
    short f30, f32, f34, f36;
    s32 unk38[6];
    int f50[2];
    int f58[2];
    V4 f60;
    V8 f70;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
    int fA0;
    int fA4;
    int fA8[2];
    s32 unkB0[(0x554 - 0xB0) / 4];
    Elem28 (*f554)[16];
    int f558;
    char *f55C;
} Context650;

extern Context650 RaceContext;
extern short SelectedTeamIndex;
extern short CourseIndex;
extern void SetGeomScreen();
extern void RotMatrixZYX();
extern SceneCameraView24 SecondaryViewportCam;
extern Elem28 (*CameraTrackTriggerTable)[16];
extern s32 SceneRenderScratchPad;
extern int RaceIntroCameraState;
extern signed char SelectionState[];
extern int ViewUsesCarTrackNode;

void RenderSubOtObject(void);
int FindCameraTrackTrigger(RaceVehicle *a0, int a1, int a2);
int DrawLensFlareAndSun(RaceVehicle *arg0);

void RenderSubOtObject(void) {
    int sp10[8];
    int *g = (int *)SCRATCHPAD;
    int s5;
    char *s2;
    int s1;

    s5 = g[0x18];
    s2 = RaceContext.f55C + (SelectedTeamIndex << 7);
    BuildObjectLightAndRotMatrices(s2, sp10);
    SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)s2, sp10);
    s1 = RotTransPersOrigin(sp10, s2);
    if (s1 > 0) {
        ClearOTagR(RaceContext.f50[DrawBufIndex], 0x15F);
        g[1] = RaceContext.f50[DrawBufIndex];
        g[0x18] = s1 >> 7;
        if (s1 >> 7 >= 6) {
            g[0x18] = 5;
        }
        SelectModelBank(8);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)s2, sp10);
        g[0x1E] = 0;
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, ModelBankFormat < 1, 0, 0);
        s1 >>= s5;
        s1 += 0x6E;
        if (s1 > 0) {
            EmitSpotlightConePolys(s1 + 1);
            ((GpuDmaTag *)g[1])->bits.address = (CurrentDrawBuf->buffer.ordering70.tags + s1)->bits.address;
            (CurrentDrawBuf->buffer.ordering70.tags + s1)->bits.address = g[1] + 0x578;
        }
    }
    if (CourseSkyboxTypeTable[CourseIndex] == 7) {
        s2 = RaceContext.f55C + (SelectedTeamIndex << 7) + 0x40;
        BuildObjectLightAndRotMatrices(s2, sp10);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)s2, sp10);
        s1 = RotTransPersOrigin(sp10, s2);
        if (s1 > 0) {
            ClearOTagR(RaceContext.f58[DrawBufIndex], 0x15F);
            g[1] = RaceContext.f58[DrawBufIndex];
            g[0x18] = s1 >> 7;
            if (s1 >> 7 >= 6) {
                g[0x18] = 5;
            }
            SelectModelBank(9);
            SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)s2, sp10);
            ((int *)SCRATCHPAD)[0x78 / 4] = 0;
            SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, ModelBankFormat < 1, 0, 0);
            s1 >>= s5;
            s1 += 0x6E;
            if (s1 > 0) {
                EmitSpotlightConePolys(s1 + 1);
                ((GpuDmaTag *)g[1])->bits.address = (CurrentDrawBuf->buffer.ordering70.tags + s1)->bits.address;
                (CurrentDrawBuf->buffer.ordering70.tags + s1)->bits.address = g[1] + 0x578;
            }
        }
    }
    g[0x18] = s5;
    g[1] = &CurrentDrawBuf->buffer.ordering70.words[0];
}

void ApplyCameraState(const SceneCameraView24 *state) {
    MatrixWords32 matrix;
    Vec3s8 angles;
    Vec3s8 offset;
    Vec3i transformed;
    ProjectionScratchPrefix *scratch = (ProjectionScratchPrefix *)SCRATCHPAD;
    scratch->pose08.angles10.xyz.x = -state->pose00.rotation10.x;
    scratch->pose08.angles10.xyz.y = state->pose00.rotation10.y;
    scratch->pose08.angles10.xyz.z = state->pose00.rotation10.z;
    angles.fields.x = state->pose00.rotation10.x;
    angles.fields.y = state->pose00.rotation10.y;
    angles.fields.z = 0;
    offset.fields.x = 0;
    offset.fields.y = 0;
    offset.fields.z = -(u16)state->distance1C;
    RotMatrixZYX(angles.halves, matrix.w);
    ApplyMatrix(&matrix, &offset, &transformed);
    {
        s32 origin = state->pose00.position00.xyz.x;
        u32 displacement = transformed.x;
        scratch->pose08.position00.xyz.x = origin + displacement;
    }
    {
        s32 origin = state->pose00.position00.xyz.y;
        u32 displacement = transformed.y;
        scratch->pose08.position00.xyz.y = origin + displacement;
    }
    {
        s32 origin = state->pose00.position00.xyz.z;
        u32 displacement = transformed.z;
        scratch->pose08.position00.xyz.z = origin + displacement;
    }
    SetGeomScreen(state->projection20);
    BuildCameraMatrices();
}
int FindCameraTrackTrigger(RaceVehicle *a0, int a1, int a2) {
    int unused10[4];
    V4 sp20;
    V4 sp30;
    int s0;
    int s3;
    int i;
    Elem28 *s2;
    int t1;
    int t2;

    s0 = -1;
    s2 = CameraTrackTriggerTable[a2];
    for (i = 1; i < 0x10; i++) {
        if (s2[i].f24 * 5 >= a1) {
            s0 = i;
            s3 = s0 - 1;
            break;
        }
    }
    if (s0 < 0) {
        return 1;
    }
    if (RaceContext.fA4 != s3) {
        ResetCarSlotBuffers();
        RaceContext.fA4 = s3;
    }
    t1 = (s2[s0].f24 - s2[s3].f24) * 5;
    t2 = a1 - s2[s3].f24 * 5;
    if (s2[s3].f18 == 0) {
        sp20.x = a0->pos10[0];
        sp20.y = a0->pos10[1];
        sp20.z = a0->pos10[2];
    } else {
        sp20 = s2[s3].f0;
    }
    if (s2[s0].f18 == 0) {
        sp30.x = a0->pos10[0];
        sp30.y = a0->pos10[1];
        sp30.z = a0->pos10[2];
    } else {
        sp30 = s2[s0].f0;
    }
    if (t1 > 0) {
        RaceContext.f60.x = ((sp30.x - sp20.x) * t2) / t1 + sp20.x;
        RaceContext.f60.y = ((sp30.y - sp20.y) * t2) / t1 + sp20.y;
        RaceContext.f60.z = ((sp30.z - sp20.z) * t2) / t1 + sp20.z;
        RaceContext.f70.x = ((s2[s0].f10.x - s2[s3].f10.x) * t2) / t1 + s2[s3].f10.x;
        RaceContext.f70.y = ((s2[s0].f10.y - s2[s3].f10.y) * t2) / t1 + s2[s3].f10.y;
        RaceContext.f70.z = ((s2[s0].f10.z - s2[s3].f10.z) * t2) / t1 + s2[s3].f10.z;
        RaceContext.f7C = ((s2[s0].f1C - s2[s3].f1C) * t2) / t1 + s2[s3].f1C;
        RaceContext.f80 = ((s2[s0].f20 - s2[s3].f20) * t2) / t1 + s2[s3].f20;
    } else {
        RaceContext.f60 = sp30;
        RaceContext.f70 = s2[s0].f10;
        RaceContext.f7C = s2[s0].f1C;
        RaceContext.f80 = s2[s0].f20;
    }
    ApplyCameraState(&SecondaryViewportCam);
    if (GameModeType == 1) {
        InitRaceContextBuffers();
        RenderSubOtObject();
    }
    return 0;
}

void ResetLensFlareState(void) {
    PsyqBzero(&SceneRenderScratchPad, 0x20);
    ((s32 *)(&SceneRenderScratchPad))[7] = -1;
}

int DrawLensFlareAndSun(RaceVehicle *arg0)
{
    int i;
    int s0;
    int *p;
    signed char *q;
    Elem28 *r;
    GpuTilePacket *pk;
    char *ot;
    char *ot2;
    int v;
    int v1;
    int v2;
    Context650 *g;
    Context650 *g2;
    Context650 *g1;
    Context650 *gB;
    Context650 *gC;
    Context650 *gL;
    Context650 *gF;
    Context650 *gD;
    Context650 *gS;
    Context650 *gT;
    s0 = 0;
    do {
        SlotSetPause(s0, 1);
        s0++;
    } while (s0 < 4);
    g1 = &RaceContext;
    if (g1->f90 != 0) {
        AddScreenFadeTiles((char *)&CurrentDrawBuf->buffer.ordering70.words[0], g1->f8C * 10, 0x25);
        v1 = g1->fA0 - 8;
        g1->fA0 = v1;
        if (v1 < 0) {
            g1->fA0 = 0;
        }
        SetSoundChannelVolLR(0, (short)g1->fA0, (short)g1->fA0);
        g1->f8C = g1->f8C + 1;
    } else {
        v2 = g1->fA0 + 8;
        g1->fA0 = v2;
        if (v2 >= 0x61) {
            g1->fA0 = 0x60;
        }
        SetSoundChannelVolLR(0, (short)g1->fA0, (short)g1->fA0);
    }
    gB = &RaceContext;
    s0 = gB->f88;
    if (s0 < 0xC) {
        if (s0 == 1) {
            StartOpeningFadeOut();
            if (gB->f94 == s0) {
                CdAudioBeginPlay(0);
            }
            for (s0 = 0; s0 < 2; s0++) {
                gB->fA8[s0] = Options.config.w[s0].rumble02;
                Options.config.w[s0].rumble02 = 2;
            }
        }
        {
            int x, d;
            x = SceneRenderScratchPad << 8;
            d = x / 12;
            s0 = 0x100;
            /* ASM_BARRIER: keep 0x100 in s0 and prioritize x in v0 */
            __asm__("" : "+r"(s0) : "r"(x));
            s0 -= d;
        }
        UpdateRaceOpeningOverlay(0x1C2);
        pk = (GpuTilePacket *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
        pk->tag.bytes.length = 3;
        pk->code = 0x62;
        pk->r = s0;
        pk->g = s0;
        pk->b = s0;
        pk->x = 0;
        pk->y = 0;
        pk->w = 0x140;
        pk->h = 0xF0;
        pk->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
        CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = (unsigned long)pk;
        pk++;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)pk;
        pk->tag.bytes.length = 1;
        ((u32 *)pk)[1] = 0xE1000245;
        pk->tag.bits.address = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
        CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = (unsigned long)pk;
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)pk + 8;
    }
    gC = &RaceContext;
    if (gC->f94 == 0) {
        if ((GameModeType == 1) && (SelectionState[SelectionState[0x17] + 8] >= 3)) {
            s0 = 1;
            gL = gC;
            gL->f98 = 0;
            r = &gL->f554[SelectedTeamIndex][1];
            do {
                v = r->f24;
                if (gL->f98 < v) {
                    gL->f98 = v;
                }
                s0++;
                r++;
            } while (s0 < 0x10);
            gF = &RaceContext;
            gF->f94 = 1;
            gF->f98 = gF->f98 * 5;
        } else {
            gC->f94 = 2;
        }
    }
    gD = &RaceContext;
    switch (gD->f94) {
    case 1:
        FindCameraTrackTrigger(arg0, gD->f88, SelectedTeamIndex);
        if (Pad0Buttons & 0x800) {
            if (gD->f88 >= 0x1A) {
                if (gD->f90 == 0) {
                    gD->f98 = gD->f88 + 0x19;
                }
            }
        }
        gS = &RaceContext;
        if ((gS->f98 - gS->f88) < 0x1A) {
            if (gS->f90 == 0) {
                gS->f90 = 1;
                CdAudioFade(0, 8);
            }
        }
        gT = &RaceContext;
        gT->f88++;
        if (gT->f88 >= gT->f98) {
            gT->f88 = gT->f98;
            gT->f9C = 1;
        }
        ViewUsesCarTrackNode = 0;
        break;

    case 2:
        gD->f88++;
        if (gD->f88 >= 0x49) {
            gD->f9C = 1;
            arg0->flags1E4 = arg0->flags1E4 | 0x100;
        }
        if (gD->f9C == 0) {
            RaceContext.f30 = 0;
            RaceContext.f32 = 0;
            RaceContext.f34 = -0x100;
            CarLocalToWorld(arg0, &RaceContext.f30, &RaceContext.f60);
            RaceContext.f70.z = 0;
            RaceContext.f80 = 0x1D6;
            RaceContext.f70.x = ((RaceContext.f88 * 0x190) / 73) - 0x280;
            v = (RaceContext.f70.y = (-(RaceContext.f88 * 0xB00) / 73) + 0x600);
            RaceContext.f7C = (-(RaceContext.f88 << 9) / 73) + 0x400;
            if (((short)v) < (-0x400)) {
                RaceContext.f70.y = -0x400;
            }
        } else {
            RaceContext.f30 = 0;
            RaceContext.f32 = 0;
            RaceContext.f34 = -0x100;
            CarLocalToWorld(arg0, &RaceContext.f30, &RaceContext.f60);
            RaceContext.f70.x = -0xF0;
            RaceContext.f70.y = -0x400;
            RaceContext.f7C = 0x200;
            RaceContext.f70.z = 0;
            RaceContext.f80 = 0x1D6;
        }
        ApplyCameraState(&SecondaryViewportCam);
        g2 = (Context650 *)((char *)&SecondaryViewportCam - 0x60);
        if (g2->f88 >= 0x33) {
            g2->f90 = 1;
        }
        break;
    }

    g = &RaceContext;
    if (g->f9C != 0) {
        SlotsStartForRace();
        v = g->f9C;
        DrawSubOtFlag = 1;
        if (v == 1) {
            if (g->f94 == v) {
                if (RaceBgmListIndex == 0) {
                    CdAudioSelectTrack(0, (PsyqRand() % 14) + 0xB);
                } else {
                    CdAudioSelectTrack(0, BgmNumberToCdTrack[RaceBgmListIndex - 1]);
                }
            }
            RaceIntroCameraState = 2;
        }
        s0 = 0;
        p = (int *)&RaceContext;
        q = (signed char *)&Options;
        do {
            q[0x2A] = *(((unsigned char *)p) + 0xA8);
            p++;
            q += 4;
            s0++;
        } while (s0 < 2);
    } else {
        DrawSubOtFlag = 0;
    }
    return RaceIntroCameraState;
}
