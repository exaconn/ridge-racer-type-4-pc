#ifndef RR_GAME_RACE_RACEGRID_H
#define RR_GAME_RACE_RACEGRID_H

#include "types.h"
#include "rr/ui_bar.h"

extern UiUvRect D_8009F6AC[8];
extern u16 D_8009F6CC[2], D_8009F6D0[4];
extern u8 D_8009F6D8[4];

void DrawStartCountdown(u32 *ordering, s32 frame);
void DrawPauseMenu(u32 *ordering, s32 mode);
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


#endif /* RR_GAME_RACE_RACEGRID_H */
