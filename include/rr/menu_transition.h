#ifndef RR_MENU_TRANSITION_H
#define RR_MENU_TRANSITION_H

#include "rr/badge_rows.h"

/* Script operations return completion; scene-entry callbacks take no arguments.
 * Script bytes are signed: negative values select the next scene. The current
 * script position is reloaded after operations, which may replace the script. */
typedef s32 (*MenuScriptOperation)(void);
typedef void (*MenuSceneEntry)(void);
extern MenuScriptOperation MenuScriptOps[];
extern MenuSceneEntry MenuSceneEntries[];
extern s8 *MenuScripts[];
extern u16 ActiveSceneId;
extern s32 ModeFrame;
extern s16 D_800F3A26;
extern s8 MenuScriptStep;
extern u16 MenuScriptId,ModeGroup;
extern s16 GameModeType;
extern s8 LoadState;
extern s16 D_800F4D8A;
extern u16 D_800FFC10;
s32 UpdateLinkHandshake(s32);
s32 UpdateRaceOpeningOverlay(s32);
void StartDemoFadeOutAndCloseVab();
s32 IntroScriptStep(void);
void SetDispMask(s32);
void UpdateMenuScript(void);

#endif
