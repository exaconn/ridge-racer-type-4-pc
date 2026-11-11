#ifndef RR_GAME_GFX_SUBMODEL_H
#define RR_GAME_GFX_SUBMODEL_H

/* Handwritten GTE renderers of the resident executable (PAL 80058118..800587E0
 * and 8005F0D0..8006E8xx): RRO model faces, relative course models and the
 * subdivided course horizon. Packets go to RR_GPU_SCRATCH->cursor and the
 * ordering table RR_GPU_SCRATCH->order (entries 128 onwards). */

#include "types.h"
#include "rr/rro_model.h"
#include "rr/course_view_types.h"
#include "rr/animated_track_types.h"
#include "rr/overlay665_render_types.h"

/* Model faces (SubmitModel is declared in rr/rro_model.h). The lighting mode
 * selects plain normal-colour lighting (0) or squared light intensities. */
void SubmitModel2(RroSceneScratchPrefix *scene, s32 modelIndex, s32 lightingMode);
void SubmitModelTwoTri(PreviewRenderScratch *scratch, s32 object, s32 blend, s32 depth);

/* Relative course models: scene->modelEntries48 holds decoded RroEffectMesh
 * entries; the second variant depth-cues every face. */
void SubmitCourseModel(RroSceneScratchPrefix *scene, s32 modelIndex);
void SubmitCourseModel2(void *scene, s32 modelIndex);

/* Course horizon tiles of every visible view record, using the course's
 * section pass program (mirror programs when scene->renderFlags64 is set). */
void SubmitCourseHorizon(void *scene, VisibleCourseViews *views, s32 course);

#endif
