#ifndef RR_PROJECTION_STATE_H
#define RR_PROJECTION_STATE_H
#include "rr/animated_sprite_grid.h"
#include "rr/geometry.h"
#include "rr/matrix_words.h"
/* Scratchpad pointer cells precede two complete SDK vectors. Native owners retain
 * complete pointer widths; PAL offsets are checked by the target compiler. */
/* Camera position and full-word Euler angles are one contiguous input view.
 * The fourth word of each vector belongs to the SDK storage. */
typedef struct {
    VectorWords16 position00;
    VectorWords16 angles10;
} ProjectionView32;
typedef struct {
    GpuScratchPointers pointers;
    ProjectionView32 pose08;
} ProjectionScratchPrefix;
typedef struct {
    ProjectionScratchPrefix projection;
    MatrixWords32 matrix28;
} ViewMatrixScratch48;
typedef struct {u8 pad00[0x20];s32 words20[3];} SceneStatePrefix;
extern SceneStatePrefix ComCars;
RR_ASSERT_SIZE(ProjectionView32,0x20);
RR_ASSERT_OFFSET(ProjectionView32,angles10,0x10);
#if defined(mips)
RR_ASSERT_SIZE(ProjectionScratchPrefix,0x28);
RR_ASSERT_OFFSET(ProjectionScratchPrefix,pose08,8);

RR_ASSERT_SIZE(ViewMatrixScratch48,0x48);
RR_ASSERT_OFFSET(ViewMatrixScratch48,matrix28,0x28);
#endif
RR_ASSERT_SIZE(SceneStatePrefix,0x2C);
RR_ASSERT_OFFSET(SceneStatePrefix,words20,0x20);
#endif
