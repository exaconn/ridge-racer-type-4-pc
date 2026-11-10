#ifndef RR_MODE_MENU_STATE_H
#define RR_MODE_MENU_STATE_H

#include "rr/menu_setup.h"
#include "rr/frame_environment.h"
#include "rr/driver_control.h"

extern char CardSaveFileName[], CardTargetFileName[24];
extern s32 ActivePlayerIndex, LoadMenuStatusPromptId, DialogueSceneMode, GarageExtraDefaultTimes[3];
extern s16 MenuActiveControllerIndex, MenuStateIndex, CardTransferMode, ModeGroup;
extern s16 GpSaveSlotCursor, GameModeType, LinkState, CardActivePort, CardMenuOperation;
extern s16 CardReplayLoadedFlag, CardPromptIndex, CardPromptChoices[3];
extern u16 ModeIndex, SubmodeState;
extern MenuSettings2C SaveSlotSelections[3], SaveSlotSnapshots[3];

void CopyStringClearTail(char *destination, char *source);
s32 UpdateLinkHandshake(s32);
void SeqStartTrack(s32 mode);
s32 LoadMenuCarModelsStep(void);
void UpdateScreenFade(s32 layout);

/* The initial choice index is0..2. The name terminates within23 bytes and
 * the text destination has24 bytes. Badge helpers may replace the live root;
 * each replacement covers two complete BadgeRow1C records. Both settings
 * arrays contain three complete records. Only the full word result1 from
 * the readiness service completes state1. */
void CardMenuModeEnter(void);

#endif
