#ifndef RR_GAME_UI_HUDMAP2_H
#define RR_GAME_UI_HUDMAP2_H

#include "types.h"
#include "rr/audio_state_types.h"

extern s32 BgmNumberToCdTrack[];
extern s16 *OpeningAttemptGlyphLists[];
extern s16 *OpeningModeGlyphLists[];
extern int CourseBgmChoiceTable2[];
extern s32 OpeningFadeTimer;
extern s32 OpeningBannerMode;
extern s32 OpeningAttemptIndex;
extern s32 OpeningBgmSelectState;
extern s32 OpeningSlideTimer;
struct GpuDmaTag;
void DrawMapBitmapIso(struct GpuDmaTag *arg0, u8 *arg1, s32 *arg2);
void SelectReplayBgmTrack(void);
void InitRaceOpeningOverlay(s32 arg0, s32 arg1);
void SelectCourseBgmTrack(void);
void DrawGlyphList(s16 *stream, s32 color, s32 bank);
void DrawGlyphListSloped(s16 *stream, s32 color, s32 bank);
void EmitIconSpriteRow120(s32 index, s32 color);
void EmitIconSpriteRow128(s32 index, s32 color);
void AddBrightnessSprite(unsigned char *a0x, int a1, int a2, int a3);

#endif /* RR_GAME_UI_HUDMAP2_H */
