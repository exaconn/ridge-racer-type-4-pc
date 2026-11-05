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
extern u8 *D_800102F0;
extern RrUiUvRect D_8009FAA0[5];
extern s32 D_8009FDA4;
extern RrCourseChoice4 D_800ABE08[2][9];
extern u32 D_800ABEB0;
extern RrRaceVehicle D_800ABEF8[2];
extern u16 D_800AC1A0;
extern s32 D_800AC5A4;
extern s16 D_800AC5D0;
extern s16 D_800AC5E0;
extern s16 D_800AC648;
extern s16 D_800AC64A;
extern s32 D_800AC6EC;
extern RrMenuGridPoint D_800ACBD0[4];
extern RrFrameStorage *D_800ACC14;
extern void *D_800ACC18;
extern s8 D_800ACC68;
extern RrMenuSettings2C D_800AD4D8;
extern s16 D_800AD522;
extern RrTimeRecord16 D_800AD5C8[2][8][5];
extern RrFrameStorage D_800ADAF0[2];
extern s32 D_800F2A64;
extern s32 D_800F2A68;
extern u8 *D_800F2AB0;
extern RrRaceSetupPair4 D_800F2AB4;
extern s32 D_800F2DE4;
extern s32 D_800F2EB8;
extern s8 D_800F2FDC;
extern s16 D_800F3826;
extern s16 D_800F3914;
extern RrSaveProgressRecord7C D_800F3920;
extern s16 D_800F3A26;
extern s32 D_800F3A30;
extern RrMenuItemState5C D_800F3A38[2];
extern u16 D_800F3A42;
extern s32 D_800F3A60;
extern RrBadgeRow1C *D_800F3AFC;
extern u16 D_800F4BE8;
extern s32 D_800F4C64;
extern s8 D_800F4C68;
extern u16 D_800F4C6A;
extern RrRaceSetupPair4 D_800F4C70;
extern s32 D_800F4C80[10];
extern void *D_800F4D40;
extern s16 D_800F4D44;
extern RrMenuArenas D_800F4D70;
extern u8 *D_800F4D74;
extern RrUiRow24 *D_800F4D80;
extern u16 D_800F4D8A;
extern RrMenuGlyphRow1C *D_800F4D8C;
extern RrCurrentSelection D_800F4D98;
extern s32 D_800FF6B0;
extern RrTimeRecord16 D_800FF700[2][8][5];
extern RrMenuTextRow60 *D_800FFC14;
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
void func_80013890(s32);
void func_80014A8C(s32);
void func_800172DC(void);
void func_800195BC(char *position, volatile u16 *matrix);
void func_80019C90(s32 scale);
void func_80019D5C(RrGpuRectangleFields *rectangle, s32 intensity, u32 *head);
s32 func_8001E84C(const char *format, ...);
void func_8001ECE8(s32 r, s32 g, s16 b, u32 flags);
void *func_8001F5B4(u32 *ordering, void *cursor, s32 x, s32 y, const char *text, s32 clut,
                    s32 spacing, s32 color);
void *func_8001FC28(u32 *, void *, s32, s32, s32, s32);
void *func_8001FCC8(u32 *ordering, void *cursor, u16 mode);
void *func_8001FD20(u32 *, void *, s32, s32, s32, s32, s32, s32, s32);
void *func_8001FF84(u32 *ordering, void *cursor, s32 x, s32 y, RrUiUvRect *region, u16 clut,
                    s32 flags);
void func_800207D4(u32 *, s32, u32);
void func_80020B10(s32);
void func_800211A4(s32);
void func_80021350(s32, s32);
void func_80021684(s32, s32);
void func_800219BC(s32, s32);
void func_80021A7C(s32, s32);
void func_80021C00(s32);
void func_80021C94(s32, s32);
void func_80022098(void *, s32);
void func_800225DC(s32);
void func_80022CD4(RrRaceVehicle *);
void func_800295B8(s32);
void func_8002A344(s32);
void func_8002A604(void);
void func_8002AF8C(s32, s32, s32);
void func_8002B12C(s32, s32, s32, s32, s32, s32, s32);
void func_8002B330(s32, s32, char *, s32, s32);
void func_8002B814(char *, s32, s32);
void func_8002B934(char *, s32);
void func_8002BA28(s32);
void func_8002BC7C(s32);
void func_8002BD18(s32, s32);
void func_8002CC88(RrRaceVehicle *, RrPreviewRgb3, s32, s32);
void func_8002E0D8(s32);
void func_8002E394(RrRaceVehicle *);
void func_8002E4F4(RrRaceVehicle *);
void func_8002E5A0(RrRaceVehicle *);
s32 func_80032E9C(s32 selection);
void *func_80032EC8(u32 *, void *, s32, s32, s32);
void func_80032F44(s32, s32);
void func_80033368(s32, s32);
void func_80033474(RrRaceVehicle *, s32);
void func_80033A74(s32);
void func_80033BE4(struct w3g2p *);
void func_80033F3C(void *, u32);
void func_80034630(s32);
void func_80034844(s32);
void func_800348E8(s32);
void func_800349F4(void *, s32);
void func_80036068(void);
void func_8003660C(void);
void func_8003662C(s32);
void func_80036FD4(s32);
void func_80037040(s8 *output, s32 glyph);
s32 func_8003708C(s8 *);
void func_80037110(void);
void func_80037198(s32);
void func_80037D88(s32);
void func_800396E4(char *);
void func_8003A198(void);
void func_8003A1B4(void);
s32 func_8003AE4C(s32, s32);
s32 func_8003B12C(void);
s32 func_8003B47C(void);
s32 func_8003BAB0(void);
/* Original callers use both the legacy no-argument entry and an explicit pause flag. */
void func_8003C254(RrRaceVehicle *);
void func_8003D228(s32 index, s32 first, s16 x, s32 y, u16 width, u16 mode, u16 flags);
void func_8003D6F0(s32);
void func_8003DC9C(s32 index, s16 a, s16 b, s16 c, s16 d, s16 e, s16 f);
void func_8003E040(s32 index, s32 control, s32 glyph, s32 style);
s32 func_8003E13C(s32 selected);
void func_8003E928(s32 index, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 count, s32 g, s32 h,
                   ...);
void func_8003EC88(s32);
s32 func_8003FBA8(s32);
void func_80040338(s32 row, s32 position, s32 group, s32 mode, s32 label);
void func_80040858(s32);
void func_800411D4(void);
void func_80041320(s32 row, s32 first, s32 second);
void func_80045868(RrRaceVehicle *);
void func_8004F24C(s32);
void *func_8004F2D8(s32, s32);
void func_8004FC18(s16, s16);
void func_8004FED4(void);
void func_8005007C(s32, s32, s32);
void func_80052800(void);
void func_800528A8(s32, s32);
void func_80052A30(s32);
void func_80052B8C(s32, s32);
void func_80053AF4(s32, s32);
void func_80053BB4(s32);
void func_80053CB0(void);
void func_8006E1AC(void);
void func_8006ECE8(s32);
void func_8006EF8C(s32);
void func_80070734(s32);
void func_800735D8(s32);
void func_800747EC(s32);
s32 rsin(s32);
s32 rcos(s32);
u16 *func_80091844(s16 *rotation, u16 *matrix);
void SetDispMask(s32);

#endif
