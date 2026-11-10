#ifndef RR_OVERLAY665_RENDER_H
#define RR_OVERLAY665_RENDER_H
#include "rr/overlay665_visual.h"
#include "game/math/rotmat.h"
#include "game/gfx/model.h"
#include "rr/renderer_setup.h"
#include "game/camera/viewcam.h"
#include "game/car/carbound.h"

#include "rr/overlay665_render_types.h"

void SetupViewport(s32);
void SetGeomOffset(s32 x,s32 y);
void SetupObjectLightMatrices(char *view,char *lighting,
    MatrixWords32 *first,MatrixWords32 *second);
u32 *ScaleMatrix(u32 *matrix,s32 scale[3]);
void SetGeomScreen(s32);
void SubmitModelTwoTri(PreviewRenderScratch *scratch,s32 object,s32 blend,s32 depth);
void ovl665_draw_controller_model(void);
#endif
