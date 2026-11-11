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
#include "rr/rro_model.h"
#include "psyq/libgpu/gpudisp.h"
#include "game/camera/viewcam.h"
#include "rr/animated_glyph.h"

extern int ScaleMatrix();
extern char ActiveCameraModeTable[];
extern short CourseIndex;
extern void SubmitCourseHorizon(void *scratch, VisibleCourseViews *views, s32 course);
extern unsigned short CameraTargetTrackNode;
extern s16 ViewOffsetX[];
extern s16 ViewOffsetY[];
extern s32 ViewScreenH[];
extern u16 CourseSelectMapCoordsX[];
extern s16 CourseSelectMapCoordsY[];
extern u16 CourseSelectLabelPosX[];
extern s16 CourseSelectLabelPosY[];
extern void SetGeomOffset(s32, s32);
extern void SetGeomScreen(s32);
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
    CameraTargetTrackNode = (unsigned short)g[0x2E];
    g[0x2E] = -1;
    BuildViewSegmentObjectList(player);
    v = LookupOctantVisibility();
    if (v < PrimaryVisibleViews.objects.count) {
        PrimaryVisibleViews.objects.count = v;
    }
    SetupViewLighting();
    SetupViewMatrix();
    SubmitCourseHorizon(g, ActiveVisibleViews, CourseIndex);
    g[0x2E] = (short)CameraTargetTrackNode;
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


void SetupViewport(s32 viewport) {
    RroSceneScratchPrefix *scene = (RroSceneScratchPrefix *)SCRATCHPAD;
    u8 *packet;
    s32 isMirror;
    s32 coordinate;
    GpuRectangleFields *rectangle;
    s32 top;
    s32 height;

    packet = scene->view.projection.pointers.cursor;
    ActiveViewportIndex = viewport;
    isMirror = (viewport == 1);
    SetGeomOffset(ViewOffsetX[viewport], ViewOffsetY[viewport]);
    SetGeomScreen(ViewScreenH[viewport]);
    scene->viewport6C.rect.x = CourseSelectMapCoordsX[viewport];
    coordinate = CourseSelectMapCoordsY[viewport];
    if (isMirror) {
        coordinate = (s32)((u32)coordinate + (u32)RearViewMirrorY);
    }
    scene->viewport6C.rect.y = coordinate;
    scene->viewport6C.rect.w = CourseSelectLabelPosX[viewport];
    coordinate = CourseSelectLabelPosY[viewport];
    if (isMirror) {
        coordinate = (s32)((u32)coordinate + (u32)RearViewMirrorY);
    }
    scene->viewport6C.rect.h = coordinate;
    rectangle = &CourseSelectPreviewUvs[DrawBufIndex][viewport];
    if (!isMirror) {
        scene->renderFlags64 = 0;
    } else {
        scene->renderFlags64 = 1;
        top = RearViewMirrorY;
        if (top < 0) {
            top = 0;
        }
        rectangle->y = top;
        height = (u16)scene->viewport6C.rect.h;
        if ((s16)top < (s16)height) {
            top = height - top;
        } else {
            top = 0;
        }
        rectangle->h = top;
        coordinate = rectangle->y;
        if (DrawBufIndex != 0) {
            coordinate += 0x100;
        }
        rectangle->y = coordinate;
    }
    if (viewport & 1) {
        scene->view.projection.pointers.order = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
    } else {
        scene->view.projection.pointers.order = &CurrentDrawBuf->buffer.ordering70.words[0];
    }
    SetDrawArea(packet, rectangle);
    AddPrim(scene->view.projection.pointers.order + 703, (u32 *)packet);
    packet += 0xC;
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = packet;
}

int GetViewTrackNode(const s32 *position, s32 player) {
    if (ViewUsesCarTrackNode == 0) {
        return FindTrackNodeAtXZ(position[0], position[2]);
    }
    return PlayerCars[player].trackNodeF0;
}
