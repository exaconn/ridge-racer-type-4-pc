#ifndef RR_RACE_SETUP_H
#define RR_RACE_SETUP_H
#include "rr/race_types.h"
#include "rr/standings.h"
void SetupRenderer(s32);
void SetModelTexturePage(s32);
void func_800209C4(s32);
void func_80021084(s32);
void func_80045830(void);
void func_8004A678(void);
void PadRumbleStart(s32);
void func_8004F7E0(void);
void func_80059168(void);
void BuildCameraMatrices(void);
void func_80070440(void);
void ClearTrackObjectStates(s32);
void DrawSync(s32);
s32 func_80114988(void);
void func_80114AF8(unsigned long);
extern s32 ScreenFrame;
extern s32 D_800ABEC0;
extern s16 D_800ABEF0;
extern s32 D_800AC538;
extern s32 D_800AC5A4;
extern s16 D_800AC5D0;
extern s32 VSyncThreshold;
extern s32 D_800ACBF8;
extern s16 D_800AD50C;
extern s16 D_800AD522;
extern s32 SavedRaceState;
extern s32 D_800F2DE4;
extern s32 TrackLength;
extern s8 D_800F2FDC;
extern s16 D_800F3914;
extern s32 NumLaps;
extern s8 D_800F4C68;
extern s32 BestLapTime;
extern s16 D_800F4D44;
extern s32 RaceState;
extern s32 D_800FF6F8;
extern s16 D_80107178;
extern s32 D_80107190;
extern s32 D_8010C12C;
extern s16 D_8010C134;
extern s32 D_8010C13C;
#include "rr/race_setup_types.h"
#include "game/camera/viewini.h"
#include "game/race/raceinit.h"
extern RrRaceSetupPair4 D_800F4C70,D_800F2AB4;
extern RrRaceVehicle PlayerCars[2];
extern s32 D_8010C0F8[3],D_8010C120[3],D_8010C140[3];
extern s32 D_800F3838[48],D_800ACC28[16],D_800F3B00[8];
extern u8 D_8009F900[16];
extern s32 D_800F4BA8[10];

/* The vector owner is flat48 words, logically [2][8][3]. The scalar and
 * participant tables are complete16-element owners, logically [2][8].
 * Course/group must remain0..7/0..1 whenever tables are selected. All signed16
 * mode values are supported. Helpers may change later-read globals, tables,
 * current/future vehicle flags and output state; both whole vehicles stay live.
 * The 8011 hooks are the overlay entry points used when entering mode4. */
void SetupRace();
RR_ASSERT_SIZE(RrRaceSetupPair4,4);
RR_ASSERT_OFFSET(RrRaceSetupPair4,second,2);
#endif
