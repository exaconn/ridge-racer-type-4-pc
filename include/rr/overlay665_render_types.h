#ifndef RR_OVERLAY665_RENDER_TYPES_H
#define RR_OVERLAY665_RENDER_TYPES_H
#include "rr/matrix_workspace.h"
#include "rr/projection_state.h"
#include "rr/renderer_setup.h"
#include "rr/object_transform_types.h"
#include "rr/rro_model.h"

/* Geometry and renderer state occupy the same scratch tail at different
 * stages. These are actual scratchpad records, including the full pointer
 * slots used by the mesh dispatcher. */
typedef union {
    ObjectTransform38 projection;
    struct {
        u32 blend;
        GpuColorCommand first,second;
        u8 tail0C[44];
    } render;
} PreviewScratchTail;
typedef struct {
    RroSceneScratchPrefix scene;
    PreviewScratchTail tail7C;
} PreviewRenderScratch;
#if defined(mips)
RR_ASSERT_SIZE(PreviewRenderScratch,0xB4);
RR_ASSERT_OFFSET(PreviewRenderScratch,scene,0);
RR_ASSERT_OFFSET(PreviewRenderScratch,tail7C,0x7C);
#endif

#endif
