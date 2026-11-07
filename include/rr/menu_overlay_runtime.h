#ifndef RR_MENU_OVERLAY_RUNTIME_H
#define RR_MENU_OVERLAY_RUNTIME_H

#include "rr/badge_row_types.h"
#include "rr/course_choice_types.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu.h"
#include "rr/matrix_workspace.h"
#include "rr/menu_arena_types.h"
#include "rr/menu_selection.h"
#include "rr/menu_selection_word.h"
#include "rr/menu_state_types.h"
#include "rr/menu_text_rows.h"
#include "rr/preview_rgb.h"
#include "rr/projection_state.h"
#include "rr/race_checkpoint_types.h"
#include "rr/race_setup_types.h"
#include "rr/race_types.h"
#include "rr/save_progress.h"
#include "rr/selection_types.h"
#include "rr/tables.h"
#include "rr/time_record_types.h"
#include "rr/ui.h"
#include "rr/ui_bar.h"

#include "rr/current_selection_types.h"

/* Shared resident storage: one type per symbol in these overlays. */
extern u8 *CdDirectoryBuffer;
extern RrUiUvRect D_8009FAA0[5];
extern s32 MenuStepDirection;
extern RrCourseChoice4 D_800ABE08[2][9];
extern u32 ScreenFrame;
extern RrRaceVehicle PlayerCars[2];
extern u16 D_800AC1A0;
extern s32 D_800AC5A4;
extern s16 D_800AC5D0;
extern s16 MenuItemCount;
extern s16 D_800AC648;
extern s16 MenuCursor;
extern s32 D_800AC6EC;
extern RrMenuGridPoint D_800ACBD0[4];
extern RrFrameStorage *D_800ACC14;
extern void *LoadCursor;
extern s8 ScreenFadeState;
extern RrMenuSettings2C SelectionState;
extern s16 ReverseCourse;
extern RrTimeRecord16 RaceRecords[2][8][5];
extern RrFrameStorage D_800ADAF0[2];
extern s32 D_800F2A64;
extern s32 RankEntries;
extern u8 *D_800F2AB0;
extern RrRaceSetupPair4 D_800F2AB4;
extern s32 ModeFrame;
extern s32 TrackLength;
extern s8 PauseCursor;
extern s16 D_800F3826;
extern s16 D_800F3914;
extern RrSaveProgressRecord7C SaveProgress;
extern s16 D_800F3A26;
extern s32 NumLaps;
extern RrMenuItemState5C D_800F3A38[2];
extern u16 D_800F3A42;
extern s32 D_800F3A60;
extern RrBadgeRow1C *BadgeRows;
extern u16 MenuScriptId;
extern s32 TotalRaceTime;
extern s8 RacePaused;
extern u16 ModeGroup;
extern RrRaceSetupPair4 D_800F4C70;
extern s32 LapTimes[10];
extern void *D_800F4D40;
extern s16 D_800F4D44;
extern RrMenuArenas D_800F4D70;
extern u8 *D_800F4D74;
extern RrUiRow24 *UiRows;
extern u16 D_800F4D8A;
extern RrMenuGlyphRow1C *GlyphRows;
extern RrCurrentSelection SelectionRows;
extern s32 RaceState;
extern RrTimeRecord16 LapRecords[2][8][5];
extern RrMenuTextRow60 *MenuTextRows;
extern RrRaceVehicle *CarTable[8];
extern RrMenuTextRow60 D_800FFC48[];
extern s32 D_80107190;
extern s16 D_8010C134;

#include "rr/race_frame_scratch.h"
#include "game/data/ldscn5.h"
#include "game/gfx/sprpkt.h"
#include "game/menu/menusave.h"
#include "game/menu/mntext.h"
#include "game/race/raceai2.h"
#include "game/race/racecar.h"
#include "game/race/raceset.h"
#include "game/sound/sndctrl.h"
#include "game/ui/hudctrl.h"
typedef RrRaceFrameScratch RrOvl659Scratch;
#ifndef RR_659_SCRATCH
#define RR_659_SCRATCH ((RrOvl659Scratch *)0x1F800000U)
#endif
#ifndef RR_666_SCRATCH
#define RR_666_SCRATCH ((RrGpuScratchPointers *)0x1F800000U)
#endif
#ifndef RR_MENU_PREVIEW_SCRATCH_WORDS
#define RR_MENU_PREVIEW_SCRATCH_WORDS ((s32 *)0x1F800000U)
#endif
#ifndef RR_PROJECTION_SCRATCH
#define RR_PROJECTION_SCRATCH ((RrProjectionScratchPrefix *)0x1F800000U)
#endif
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((RrGpuScratchPointers *)0x1F800000U)
#endif

/* Resident game and GPU services. */
struct w3g2p;
void SetupRenderer(s32);
void SetModelTexturePage(s32);
void func_800172DC();
void func_800195BC(char *position, volatile u16 *matrix);
void func_80019C90(s32 scale);
void EmitTiledRect(RrGpuRectangleFields *rectangle, s32 intensity, u32 *head);
s32 func_8001E84C(const char *format, ...);
void SetupDrawDispEnvs(s32 r, s32 g, s16 b, u32 flags);
void *AddTextSprites(u32 *ordering, void *cursor, s32 x, s32 y, const char *text, s32 clut,
                    s32 spacing, s32 color);
void *AddDrawAreaPrim(u32 *, void *, s32, s32, s32, s32);
void *AddDrawModePrim(u32 *ordering, void *cursor, u16 mode);
void *AddTilePrim(u32 *, void *, s32, s32, s32, s32, s32, s32, s32);
void *func_8001FF84(u32 *ordering, void *cursor, s32 x, s32 y, RrUiUvRect *region, u16 clut,
                    s32 flags);
void AddScreenFadeTiles(u32 *, s32, u32);
void func_80020B10(s32);
void func_800211A4(s32);
void DrawHudRotatingQuad(s32, s32);
void DrawHudNumber3(s32, s32);
void func_800219BC(s32, s32);
void func_80021A7C(s32, s32);
void func_80021C00(s32);
void DrawLapCounter(s32, s32);
void DrawSteeringIndicator(void *, s32);
void func_800225DC(s32);
void func_80022CD4(RrRaceVehicle *);
void SaveCarSnapshots(s32);
void UpdateScreenFade(s32);
void ClearScreenOverlayState(void);
void SetMenuFrameOptions(s32, s32, s32);
void DrawMenuStepArrows(s32, s32, s32, s32, s32, s32, s32);
void StepMenuValue(s32, s32, char *, s32, s32);
void FormatDecimalDigits(char *, s32, s32);
void FormatRaceTime(char *, s32);
void DrawLapTimeList(s32);
void func_8002BC7C(s32);
void func_8002BD18(s32, s32);
void func_8002CC88(RrRaceVehicle *, RrPreviewRgb3, s32, s32);
void DrawCars(s32);
void SetCarBounds(RrRaceVehicle *);
void ResetCarState(RrRaceVehicle *);
void func_8002E5A0(RrRaceVehicle *);
s32 DecodeCourseIndex(s32 selection);
void *func_80032EC8(u32 *, void *, s32, s32, s32);
void DrawCourseInfoPanel(s32, s32);
void func_80033368(s32, s32);
void UpdateRaceIntroCamera(RrRaceVehicle *, s32);
void SetupMenuCarCamera(s32);
void UpdateRaceChaseCamera(void *);
void DrawStartCountdown(void *, u32);
void PlayStartCountdownVoice(s32);
void UpdatePauseMenu(s32);
void ApplyPauseMenuChoice(s32);
void DrawPauseMenu(void *, s32);
void StopRaceAudioAndRumble(void);
void FinishRace(void);
void func_8003662C(s32);
void func_80036FD4(s32);
void UnpackChoiceGlyph(s8 *output, s32 glyph);
s32 PackChoiceGlyph(s8 *);
void func_80037110(void);
void func_80037198(s32);
void StepRaceFrame(s32);
void CheckFinalLapEvent(char *);
void ResetScreenCounters(void);
void InitMenuArenas(void);
s32 LoadCarRangeStep(s32, s32);
s32 func_8003B12C(void);
s32 func_8003B47C(void);
s32 LoadTrackNodesStep(void);
/* Original callers use both the legacy no-argument entry and an explicit pause flag. */
void UpdateLapTiming(RrRaceVehicle *);
void InitBadgeRow(s32 index, s32 first, s16 x, s32 y, u16 width, u16 mode, u16 flags);
void UpdateBadgeRowAnim(s32);
void InitRect16Row(s32 index, s16 a, s16 b, s16 c, s16 d, s16 e, s16 f);
void InitGlyphRow();
s32 UpdateGlyphRow(s32 selected);
void InitUiRow(s32 index, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 count, s32 g, s32 h,
                   ...);
void DrawUiRow(s32);
s32 UpdateImageRow(s32);
void SetMenuTextRow(s32 row, s32 position, s32 group, s32 mode, s32 label);
void UpdateMenuTextRow(s32);
void func_800411D4(void);
void func_80041320(s32 row, s32 first, s32 second);
void func_80045868(RrRaceVehicle *);
void PlaySfx(s32);
void *SoundQueueEnqueue(s32, s32);
void SoundSetMasterVolume(s16, s16);
void SlotsStartForRace();
void SlotSetLevelProgram(s32, s32, s32);
void CdAudioInit(void);
void CdAudioSelectTrack();
void CdAudioBeginPlay(s32);
void CdAudioFade(s32, s32);
void func_80053AF4(s32, s32);
void func_80053BB4(s32);
void func_80053CB0(void);
void BuildCameraMatrices(void);
void func_8006ECE8(s32);
void SetupViewport(s32);
void func_80070734(s32);
void UpdateTrackObjects(s32);
void DrawTrackObjects(s32);
s32 rsin(s32);
s32 rcos(s32);
u16 *RotMatrix(s16 *rotation, u16 *matrix);
void SetDispMask(s32);

#endif
