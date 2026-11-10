#ifndef RR_GAME_MENU_MENUTRA21_H
#define RR_GAME_MENU_MENUTRA21_H

#include "types.h"

int LoadSaveFromCardStep();
void DrawCardMenuStatusText();
void RunCardSaveOrLoadStep(void);
void StepCardConfirmPrompt(void);
void WaitCardDialogueComplete(void);
void RunCardFormatStep(void);
void InitCardMenuSession(void);
void CloseCardMenuUiRows(void);
void StepCardSlotHeaderScan(void);
void CheckCardPollStatusTransition(void);
void CheckCardFileExistsTransition(void);
void StepCardDataTransfer(void);
int ScanBothCardPortsStep(int a0);
int ReadCardSlotHeaderStep(int a0, int a1);


#endif /* RR_GAME_MENU_MENUTRA21_H */
