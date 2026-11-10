#include "rr/projection_state.h"
#include "rr/camera_matrices.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat.h"
#include "types.h"
#include "scratchpad.h"
#include "psyq/libgte/gtemat7.h"
#include "rr/matrix_workspace.h"
#include "rr/menu_overlay_runtime.h"
#include "game/math/rotmat.h"
#include "game/data/globals.h"

extern short AtanTable[];

s32 Atan2Table(int a0, int a1);

void BuildRotMatrixZ(MatrixWords32 *matrix, s32 angle) {
    short sine = rsin(angle);
    short cosine = rcos(angle);
    matrix->fields.rotation00[0][1] = -sine;
    matrix->fields.rotation00[0][0] = cosine;
    matrix->fields.rotation00[1][1] = cosine;
    matrix->fields.rotation00[0][2] = 0;
    matrix->fields.rotation00[1][0] = sine;
    matrix->fields.rotation00[1][2] = 0;
    matrix->fields.rotation00[2][0] = 0;
    matrix->fields.rotation00[2][1] = 0;
    matrix->fields.rotation00[2][2] = 0x1000;
}

void BuildRotMatrixY(MatrixWords32 *matrix, s32 angle) {
    s16 sine;
    s16 cosine;

    sine = rsin(angle);
    cosine = rcos(angle);
    matrix->fields.rotation00[0][2] = (s16) -sine;
    matrix->fields.rotation00[0][0] = cosine;
    matrix->fields.rotation00[0][1] = 0;
    matrix->fields.rotation00[1][0] = 0;
    matrix->fields.rotation00[1][1] = 0x1000;
    matrix->fields.rotation00[1][2] = 0;
    matrix->fields.rotation00[2][0] = sine;
    matrix->fields.rotation00[2][1] = 0;
    matrix->fields.rotation00[2][2] = cosine;
}

void BuildRotMatrixX(MatrixWords32 *matrix, s32 angle) {
    s16 sine;
    s16 cosine;

    sine = rsin(angle);
    cosine = rcos(angle);
    matrix->fields.rotation00[0][0] = 0x1000;
    matrix->fields.rotation00[0][1] = 0;
    matrix->fields.rotation00[0][2] = 0;
    matrix->fields.rotation00[1][0] = 0;
    matrix->fields.rotation00[1][1] = cosine;
    matrix->fields.rotation00[1][2] = (s16) -sine;
    matrix->fields.rotation00[2][0] = 0;
    matrix->fields.rotation00[2][1] = sine;
    matrix->fields.rotation00[2][2] = cosine;
}

void SetIdentityMatrix(MatrixWords32 *matrix) {
    matrix->fields.rotation00[0][0] = 0x1000;
    matrix->fields.rotation00[0][1] = 0;
    matrix->fields.rotation00[0][2] = 0;
    matrix->fields.rotation00[1][0] = 0;
    matrix->fields.rotation00[1][1] = 0x1000;
    matrix->fields.rotation00[1][2] = 0;
    matrix->fields.rotation00[2][0] = 0;
    matrix->fields.rotation00[2][1] = 0;
    matrix->fields.rotation00[2][2] = 0x1000;
    matrix->fields.translation14[0] = 0;
    matrix->fields.translation14[1] = 0;
    matrix->fields.translation14[2] = 0;
}

void BuildCameraMatrices(void) {
    MatrixWords32 rotation;
    ProjectionScratchPrefix *view = (ProjectionScratchPrefix *)SCRATCHPAD;
    BuildRotMatrixY((MatrixWords32 *)SCRATCH_ROT_MATRIX, view->pose08.angles10.xyz.y);
    BuildRotMatrixY(&CameraViewMatrix, view->pose08.angles10.xyz.y);
    SetIdentityMatrix(&CameraPitchRollMatrix);
    BuildRotMatrixY(&D_800FF6D8, view->pose08.angles10.xyz.y);
    BuildRotMatrixX(&rotation, view->pose08.angles10.xyz.x);
    MulMatrix2(&rotation, (MatrixWords32 *)(SCRATCH_ROT_MATRIX));
    MulMatrix2(&rotation, &CameraViewMatrix);
    MulMatrix2(&rotation, &CameraPitchRollMatrix);
    BuildRotMatrixZ(&rotation, view->pose08.angles10.xyz.z);
    MulMatrix2(&rotation, (MatrixWords32 *)(SCRATCH_ROT_MATRIX));
    MulMatrix2(&rotation, &CameraViewMatrix);
    MulMatrix2(&rotation, &CameraPitchRollMatrix);
    BuildRotMatrixY(&rotation, 0x800);
    MulMatrix0(
        &rotation,
        (const MatrixWords32 *)(SCRATCH_ROT_MATRIX),
        &RearViewMirrorMatrix);
    BuildRotMatrixX(&rotation, D_800AC0A4 * 2);
    MulMatrix2(&rotation, &RearViewMirrorMatrix);
    RearViewMirrorMatrix.halves[0] = -RearViewMirrorMatrix.halves[0];
    RearViewMirrorMatrix.halves[1] = -RearViewMirrorMatrix.halves[1];
    RearViewMirrorMatrix.halves[2] = -RearViewMirrorMatrix.halves[2];
    SetRotMatrix(SCRATCH_ROT_MATRIX);
}

void BuildRotMatrixAngles(int *a0) {
    MatrixWords32 rotation;
    BuildRotMatrixY((MatrixWords32 *)(SCRATCHPAD + 0x108), a0[1]);
    BuildRotMatrixX(&rotation, a0[0]);
    MulMatrix2(&rotation, (MatrixWords32 *)((SCRATCHPAD + 0x108)));
    BuildRotMatrixZ(&rotation, a0[2]);
    MulMatrix2(&rotation, (MatrixWords32 *)((SCRATCHPAD + 0x108)));
}

int Atan2Table(int a0, int a1)
{
    int r;
    if (a0 == 0) {
        r = 0;
        if (a1 != 0) {
            r = -0x400;
            if (a1 > 0) {
                return 0x400;
            }
            goto ret;
        }
        goto ret;
    }
    if (a0 > 0) {
        if (a1 >= 0) {
            if (a0 < a1) {
                return 0x400 - AtanTable[(a0 << 10) / a1];
            }
            r = AtanTable[(a1 << 10) / a0];
ret:
            return r;
        }
        a1 = -a1;
        if (a0 < a1) {
            return AtanTable[(a0 << 10) / a1] - 0x400;
        }
        return -AtanTable[(a1 << 10) / a0];
    }
    a0 = -a0;
    if (a1 >= 0) {
        if (a0 < a1) {
            return AtanTable[(a0 << 10) / a1] + 0x400;
        }
        return 0x800 - AtanTable[(a1 << 10) / a0];
    }
    a1 = -a1;
    if (a0 < a1) {
        return 0xC00 - AtanTable[(a0 << 10) / a1];
    }
    return AtanTable[(a1 << 10) / a0] + 0x800;
}
