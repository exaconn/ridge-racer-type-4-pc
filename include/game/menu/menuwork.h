#ifndef RR_GAME_MENU_MENUWORK_H
#define RR_GAME_MENU_MENUWORK_H

#include "types.h"

void SelectTeamCarGroup(void);
void UpdateUnlockedCarMask(void);
void InitCarPreviewScene(void);
void CarPreviewModeUpdate(void);
void GarageCarPreviewModeUpdate(void);
void GetNameChar(unsigned char *a0, int a1);
void StepNameChar(unsigned char *a0);
void CarPreviewLoadModeEnter(void);
void CarPreviewLoadModeUpdate(void);


#endif /* RR_GAME_MENU_MENUWORK_H */
