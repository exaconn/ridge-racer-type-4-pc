#ifndef RR_GAME_GFX_MODEL_H
#define RR_GAME_GFX_MODEL_H

#include "types.h"
#include "rr/model_table.h"
#include "rr/course_view_types.h"
#include "rr/projection_state.h"

int GetViewTrackSegment(const ProjectionView32 *view, s32 player);
s32 YawToOctant(const ProjectionView32 *view);
CourseViewIndices *GetSegmentViewTable(s32 player);
void InitTimTemplate(int *a0);
void RelocateModelTable(RroModelTableWire *table);
extern RroModelBank *ModelBankTable[];
extern s32 ModelBankFormat;
void RegisterModelBank(RroModelBank *bank, s32 slot);
void UnrelocateModelBank(RroModelBank *bank, const u8 *base);
void SelectModelBank(s32 slot);
void SetFogNearFar(s32 a0, s32 a1, s32 a2);
void SetVramImageSlot(int a0, int a1);
void LoadVramImageSlot(int a0);


#endif /* RR_GAME_GFX_MODEL_H */
