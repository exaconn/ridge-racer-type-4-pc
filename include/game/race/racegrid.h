#ifndef RR_GAME_RACE_RACEGRID_H
#define RR_GAME_RACE_RACEGRID_H

#include "types.h"
#include "rr/race_types.h"
#include "rr/ui_bar.h"

extern UiUvRect PauseMenuLabelUvs[8];
extern u16 PauseTitleClutIds[2], PauseRowClutIds[4];
extern u8 PauseRowOffsetX[4];

void DrawStartCountdown(u32 *ordering, s32 frame);
void DrawPauseMenu(u32 *ordering, s32 selection);
void ResetLinkSioControl(void);
int IsLinkPeerPresent(void);
void OpenLinkSerial(void);
void CloseLinkSerial(void);
void BeginLinkSync(void);
void SetupMenuCarCamera(int a);
void PlayStartCountdownVoice(int a0);
void InitPlayerRumble(void);
void UpdatePauseMenu(int a0);
void ApplyPauseMenuChoice(int arg0);
int LinkTimeoutCallback(int a0, unsigned int a1);
int ExitOnLinkLost(void);
int UpdateLinkHandshake(int a0);
int PollLinkStatus(void);



void UpdateRaceChaseCamera(RaceVehicle *vehicle);

#endif /* RR_GAME_RACE_RACEGRID_H */
