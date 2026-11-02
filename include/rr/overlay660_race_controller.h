#ifndef RR_OVERLAY660_RACE_CONTROLLER_H
#define RR_OVERLAY660_RACE_CONTROLLER_H
#include "rr/race.h"
#include "rr/frame_storage_types.h"
#include "rr/badge_rows.h"
#include "rr/race_setup.h"
#include "rr/tables.h"
#include "rr/overlay_race_state.h"

/* Complete scratch prefix: pointer fields grow naturally on native builds. */
typedef struct {
 RrGpuScratchPointers pointers;
 u8 reserved08[0x78 - 8];
 s32 word78;
} Rr660FrameScratch;
#ifndef RR_660_FRAME_SCRATCH
#define RR_660_FRAME_SCRATCH ((Rr660FrameScratch *)0x1F800000U)
#endif
extern RrFrameStorage D_800ADAF0[2];
extern RrMenuItemState5C D_800F3A38[2];
extern s32 D_800F2A64;
extern s16 D_800F4C6A,D_800F3A26,D_800F4D8A;
struct w3g2p;
void func_80014A8C(s32);
void func_800172DC(void);
void func_800207D4(u32 *, s32, u32);
void func_80020B10(s32);
void func_800211A4(s32);
void func_80021350(s32, s32);
void func_80021684(s32, s32);
void func_800219BC(s32, s32);
void func_80021E2C(s32);
void func_80022098(u32 *, s32);
void func_800225DC(s32);
void func_80022CD4(RrRaceVehicle *);
void func_8002BA28(s32);
void func_8002E0D8(s32);
void func_80033474(RrRaceVehicle *, s32);
void func_80033BE4(struct w3g2p *);
void func_80033F3C(void *, u32);
void func_80034630(s32);
void func_80034844(s32);
void func_800348E8(s32);
void func_800349F4(void *, s32);
void func_80036030(void);
void func_80036068(void);
void func_8003660C(void);
void func_80036FD4(s32);
void func_80037D88(s32);
void func_800396E4(char *);
void func_8003BFC8(s32);
void func_8003C254(RrRaceVehicle *);
void func_8003C96C(void);
void func_8004FA04(void);
void func_8004FA50(void);
void func_8004FED4(void);
void func_8005007C(s32, s32, s32);
void func_80052A30(s32);
void func_8006E1AC(void);
void func_8006ECE8(s32);
void func_8006EF8C(s32);
void func_80070734(s32);
void func_800735D8(s32);
void func_800747EC(s32);

/* Uses both complete frame banks and vehicle owners, the full input[2] and
 * first halfword of each race-setup pair. The frame counter/tick wrap32.
 * Every pointer remains complete; helpers may change roots and later-read
 * globals. Paused rendering publishes fresh F2A64 to scratch.word78 too. */
void ovl660_func_8011718C(void);
RR_ASSERT_SIZE(Rr660FrameScratch,0x7C);
RR_ASSERT_OFFSET(Rr660FrameScratch,pointers,0);
RR_ASSERT_OFFSET(Rr660FrameScratch,word78,0x78);
#endif
