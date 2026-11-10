#include "psyq/libgte/rotmatyxz.h"
#include "psyq/libgte/gtemat5.h"
#include "rr/frame_storage_types.h"
#include "rr/camera_matrices.h"
#include "rr/matrix_workspace.h"
#include "game/car/car.h"
#include "types.h"
#include "scratchpad.h"
#include "game/camera/viewini.h"
#include "game/car/aipath.h"
#include "game/gfx/poly1.h"
#include "game/math/rotmat.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/gtetrig.h"
#include "rr/hud_number.h"
#include "game/data/globals.h"
#include "rr/object_transform_types.h"
#include "rr/projection_state.h"
#include "game/camera/viewcam.h"
#include "rr/animated_glyph.h"

#define G ((Scratch_8006EF8C *)SCRATCHPAD)


typedef struct {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
} S_800A3C20;

typedef struct {
    s32 f00;
    s32 f04;
    s32 reserved08[23];
    s32 f64;
    s32 f68;
    u16 f6C;
    u16 f6E;
    u16 f70;
    u16 f72;
} Scratch_8006EF8C;


extern int ScaleMatrix();
extern char ActiveCameraModeTable[];
extern short CourseIndex;
extern void SubmitCourseHorizon(void *scratch, VisibleCourseViews *views, s32 course);
extern unsigned short D_8010E280;
extern s16 ViewOffsetX[];
extern s16 ViewOffsetY[];
extern s32 ViewScreenH[];
extern u16 CourseSelectMapCoordsX[];
extern s16 CourseSelectMapCoordsY[];
extern u16 CourseSelectLabelPosX[];
extern s16 CourseSelectLabelPosY[];
extern S_800A3C20 CourseSelectPreviewUvs[][7];
extern void SetGeomOffset(s32, s32);
extern void SetGeomScreen(s32);
extern void SetDrawArea(s32);
extern int ViewUsesCarTrackNode;
extern RaceVehicle PlayerCars[];

void SetupHorizonViewTransform(int *a0, int *a1);
void SetupViewMatrix(void);
void RenderPlayerCourseView(int player);

void SetupHorizonViewTransform(int *a0, int *a1) {
    int v[4], w[4], x[4];
    int *g = (int *)(SCRATCHPAD + 0x7C);
    if (a0) {
        BuildRotMatrixAngles(a0);
        v[0] = (0x8000 - a1[0]) << 2;
        v[1] = (0x60 - a1[1]) << 2;
        v[2] = (0x8000 - a1[2]) << 2;
        ApplyMatrixLV((const MatrixWords32 *)((SCRATCHPAD + 0x108)), (const Vec3i *)(v), (Vec3i *)(w));
        x[2] = 0x4000;
        x[1] = 0x4000;
        x[0] = 0x4000;
        g[0x28] = w[0];
        g[0x29] = w[1];
        g[0x2A] = w[2];
        ScaleMatrix((SCRATCHPAD + 0x108), x);
        ResetViewScratchMask(ActiveVisibleViews, a0, a1);
    }
}

void SetupViewMatrix(void) {
    VectorWords16 v, w, x;
    VectorWords16 *position = &((ViewMatrixScratch48 *)SCRATCHPAD)->projection.pose08.position00;
    v.words[0] = 0x8000 - (position->words[0] >> 2);
    v.words[1] = -(position->words[1] >> 2);
    v.words[2] = 0x8000 - (position->words[2] >> 2);
    ApplyMatrixLV((const MatrixWords32 *)SCRATCH_ROT_MATRIX, &v.xyz, &w.xyz);
    CameraViewMatrix.w[5] = w.words[0];
    CameraViewMatrix.w[6] = w.words[1];
    CameraViewMatrix.w[7] = w.words[2];
    HalfTransViewMatrix = CameraViewMatrix;
    v.words[0] = (0x20000U - position->words[0]);
    v.words[1] = (0U - position->words[1]);
    v.words[2] = (0x20000U - position->words[2]);
    HalfTransViewMatrix.w[5] >>= 1;
    HalfTransViewMatrix.w[6] >>= 1;
    HalfTransViewMatrix.w[7] >>= 1;
    ApplyMatrixLV((const MatrixWords32 *)SCRATCH_ROT_MATRIX, &v.xyz, &w.xyz);
    ((ViewMatrixScratch48 *)SCRATCHPAD)->matrix28.w[5] = w.words[0];
    ((ViewMatrixScratch48 *)SCRATCHPAD)->matrix28.w[6] = w.words[1];
    ((ViewMatrixScratch48 *)SCRATCHPAD)->matrix28.w[7] = w.words[2];
    SetTransMatrix((s32 *)SCRATCH_ROT_MATRIX);
    x.words[2] = 0x4000;
    x.words[1] = 0x4000;
    x.words[0] = 0x4000;
    ScaleMatrix((s32 *)SCRATCH_ROT_MATRIX, x.words);
    SetRotMatrix((s32 *)SCRATCH_ROT_MATRIX);
}


void SetupViewLighting(void) {
    SetColorMatrix(&ColorMatrix);
    BuildLightMatrix();
    SetLightMatrix(&LightMatrix.matrix);
    SetBackColor(BackColorR, BackColorG, BackColorB);
    SetFarColor(FarColorR, FarColorG, FarColorB);
    SetFogNear(0x4E20, 0x122);
}


void SetupPlayerView(int player) {
    char *p = ActiveCameraModeTable;
    BuildViewSegmentObjectList(player);
    ApplySegmentVisibilityMask();
    SetupViewLighting();
    SetupHorizonViewTransform(p, p - 0x30);
    SetupViewMatrix();
    SubmitCourseHorizon(SCRATCHPAD, ActiveVisibleViews, CourseIndex);
}

void RenderPlayerCourseView(int player) {
    BuildViewSegmentObjectList(player);
    UpdateVisibleViewRecords();
    ApplySegmentVisibilityMask();
    SetupViewLighting();
    SetupViewMatrix();
    SubmitCourseHorizon(SCRATCHPAD, ActiveVisibleViews, CourseIndex);
}


void RenderMirrorCourseView(int player) {
    short *g = (short *)SCRATCHPAD;
    unsigned int v; int unused[12];
    D_8010E280 = (unsigned short)g[0x2E];
    g[0x2E] = -1;
    BuildViewSegmentObjectList(player);
    v = LookupOctantVisibility();
    if (v < PrimaryVisibleViews.objects.count) {
        PrimaryVisibleViews.objects.count = v;
    }
    SetupViewLighting();
    SetupViewMatrix();
    SubmitCourseHorizon(g, ActiveVisibleViews, CourseIndex);
    g[0x2E] = (short)D_8010E280;
}


void SetGteObjectTransform(ObjectTransform38 *output, const s16 *position, s32 *rotation) {
    ProjectionScratchPrefix *view = (ProjectionScratchPrefix *)SCRATCHPAD;
    output->fields.relative00.fields.x = position[0] - (s16)view->pose08.position00.words[0];
    output->fields.relative00.fields.y = position[2] - (s16)view->pose08.position00.words[1];
    output->fields.relative00.fields.z = position[4] - (s16)view->pose08.position00.words[2];
    ApplyMatrix((const MatrixWords32 *)SCRATCH_ROT_MATRIX, &output->fields.relative00, &output->fields.transformed08);
    output->fields.matrix18.fields.translation14[0] = output->fields.transformed08.x;
    output->fields.matrix18.fields.translation14[1] = output->fields.transformed08.y;
    output->fields.matrix18.fields.translation14[2] = output->fields.transformed08.z;
    SetRotMatrix(rotation);
    SetTransMatrix(output->fields.matrix18.w);
}

void BuildLightMatrix(void) {
    int i;
    MatrixWords32 rotation;
    Vec3s8 direction;
    VectorWords16 rotated;
    direction.fields.y = 0;
    direction.fields.x = 0;
    for (i = 0; i < 3; i++) {
        direction.fields.z = -LightSourceIntensities[i];
        RotMatrixYXZ(&LightSourceAngles[i], &rotation);
        SetRotMatrix(rotation.w);
        ApplyRotMatrix(&direction, &rotated.xyz);
        LightMatrix.matrix.fields.rotation00[i][0] = rotated.xyz.x;
        LightMatrix.matrix.fields.rotation00[i][1] = rotated.xyz.y;
        LightMatrix.matrix.fields.rotation00[i][2] = rotated.xyz.z;
    }
}


void SetupViewport(s32 a0) {
    Scratch_8006EF8C *s2 = G;
    s32 s3;
    s32 s1;
    s32 s5;
    s32 v1;
    S_800A3C20 *p;
    /* ASM_BARRIER: Keep a0_val in $a0 in the s5 != 0 branch so p is allocated to $a1. */
    register s32 a0_val asm("$4");
    /* ASM_BARRIER: Keep a2_val in $a2 in the s5 != 0 branch so p is allocated to $a1. */
    register s32 a2_val asm("$6");

    s3 = s2->f00;
    ActiveViewportIndex = a0;
    s1 = (a0 == 1);
    SetGeomOffset(ViewOffsetX[a0], ViewOffsetY[a0]);
    s5 = (a0 == 1);
    SetGeomScreen(ViewScreenH[a0]);
    s2->f6C = CourseSelectMapCoordsX[a0];
    v1 = CourseSelectMapCoordsY[a0];
    if (s1) {
        v1 += RearViewMirrorY;
    }
    s2->f6E = v1;
    s2->f70 = CourseSelectLabelPosX[a0];
    v1 = CourseSelectLabelPosY[a0];
    if (s1) {
        v1 += RearViewMirrorY;
    }
    s2->f72 = v1;
    p = &CourseSelectPreviewUvs[DrawBufIndex][a0];
    if (!s5) {
        s2->f64 = 0;
    } else {
        s2->f64 = 1;
        a0_val = RearViewMirrorY;
        if (a0_val < 0) {
            a0_val = 0;
        }
        p->f2 = a0_val;
        a2_val = s2->f72;
        if ((s16)a0_val < (s16)a2_val) {
            a0_val = a2_val - a0_val;
        } else {
            a0_val = 0;
        }
        p->f6 = a0_val;
        v1 = p->f2;
        if (DrawBufIndex != 0) {
            v1 += 0x100;
        }
        p->f2 = v1;
    }
    if (a0 & 1) {
        s2->f04 = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
    } else {
        s2->f04 = &CurrentDrawBuf->buffer.ordering70.words[0];
    }
    SetDrawArea(s3);
    AddPrim(s2->f04 + 0xAFC, s3);
    s3 += 0xC;
    G->f00 = s3;
}

int GetViewTrackNode(const s32 *position, s32 player) {
    if (ViewUsesCarTrackNode == 0) {
        return FindTrackNodeAtXZ(position[0], position[2]);
    }
    return PlayerCars[player].trackNodeF0;
}
