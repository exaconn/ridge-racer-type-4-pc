#ifndef RR_OVERLAY666_RACE_CONTROLLER_H
#define RR_OVERLAY666_RACE_CONTROLLER_H
#include "rr/race.h"
#include "rr/frame_storage_types.h"
#include "rr/icon_sprites.h"
#include "rr/badge_rows.h"
#include "rr/overlay666_checkpoint.h"
#include "rr/race_setup.h"
#include "rr/tables.h"
#ifndef RR_666_SCRATCH
#define RR_666_SCRATCH ((RrGpuScratchPointers *)0x1F800000U)
#endif

extern s32 D_800F2DE4;
extern s32 D_800FF6B0;
extern RrMenuItemState5C D_800F3A38[2];
extern s16 D_800AC5D0;
extern s8 D_800F4C68;
extern s8 D_800F2FDC;
extern s16 D_800F4D44;


extern RrRaceVehicle D_800ABEF8[2];
extern s32 D_800AC5A4;
extern s32 D_800ABEB0;
extern s32 D_800F4C64;

extern s32 ovl666_D_80116F94;
extern s16 D_800F4C6A;
extern s16 D_800F3A26;
extern s16 D_800F4D8A;
extern RrFrameStorage D_800ADAF0[2];



void func_80014A8C(s32);
void func_800172DC(void);

void func_800207D4(u32 *, s32, u32);
void func_80020B10(s32);
void func_800211A4(s32);
void func_80021350(s32, s32);
void func_80021684(s32, s32);
void func_800219BC(s32, s32);
void func_80021A7C(s32, s32);
void func_80021C00(s32);
void func_80021C94(s32, s32);
void func_80021E2C(s32);
void func_80022098(void *, s32);
void func_800225DC(s32);
void func_80022CD4(RrRaceVehicle *);
void func_8002BD18(s32, s32);
void func_8002E0D8(s32);
void func_80033474(RrRaceVehicle *, s32);
struct w3g2p;
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
void func_8003C96C(void);
void func_8004FA04(void);
void func_8004FA50(void);
void func_8004FC18(s16, s16);
void func_8004FED4(void);
void func_8005007C(s32, s32, s32);
void func_80052A30(s32);
void func_8006E1AC(void);
void func_8006ECE8(s32);
void func_8006EF8C(s32);
void func_80070734(s32);
void func_800735D8(s32);
void func_800747EC(s32);



/* Both complete frame banks and two complete vehicles remain live. Services
 * may replace the context and both scratch software pointers; all subsequent
 * accesses use fresh roots. Timer and mode inputs span their full scalar
 * domains. Frame/pulse/budget arithmetic wraps32. Field18 is rgb24. */
void ovl666_func_80116730(void);
RR_ASSERT_OFFSET(RrRaceVehicle,half1EE,0x1EE);
RR_ASSERT_OFFSET(RrRaceVehicle,half27A,0x27A);
RR_ASSERT_OFFSET(RrFrameEnvironment,rgb24,0x18);
#endif
