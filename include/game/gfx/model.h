#ifndef RR_GAME_GFX_MODEL_H
#define RR_GAME_GFX_MODEL_H

#include "types.h"
#include "rr/emblem_tim_types.h"
#include "rr/model_table.h"
#include "rr/course_view_types.h"
#include "rr/projection_state.h"

int GetViewTrackSegment(const ProjectionView32 *view, s32 player);
s32 YawToOctant(const ProjectionView32 *view);
CourseViewIndices *GetSegmentViewTable(s32 player);
CourseViewIndices *GetWideNeighbourViewTable(s32 player);
void InitTimTemplate(EmblemTimImage840 *image);
void RelocateModelTable(RroModelTableWire *table);
extern RroModelBank *ModelBankTable[];
extern s32 ModelBankFormat;
/* Decode the serialized bank at image (RroModelBankWire) into the host bank
 * for slot, publish it in ModelBankTable[slot] and return it. */
RroModelBank *RegisterModelBank(const void *image, s32 slot);
/* Rewrite the bank image's offsets relative to base (u32 wire words). */
void UnrelocateModelBank(RroModelBank *bank, const u8 *base);
void SelectModelBank(s32 slot);
void SetFogNearFar(s32 a0, s32 a1, s32 a2);
void SetVramImageSlot(const void *pixels, int slot);
void LoadVramImageSlot(int a0);


#endif /* RR_GAME_GFX_MODEL_H */
