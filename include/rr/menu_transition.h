#ifndef RR_MENU_TRANSITION_H
#define RR_MENU_TRANSITION_H

#include "rr/badge_rows.h"

/* Script operations return completion; scene-entry callbacks take no arguments.
 * Script bytes are signed: negative values select the next scene. The current
 * script position is reloaded after operations, which may replace the script. */
typedef s32 (*RrMenuScriptOperation)(void);
typedef void (*RrMenuSceneEntry)(void);
extern RrMenuScriptOperation D_8009F840[];
extern RrMenuSceneEntry D_8009F870[];
extern s8 *D_8009F8D0[];
extern u16 D_800AD50A;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s8 D_800F3A2C;
extern u16 D_800F4BE8,D_800F4C6A;
extern s16 D_800F4D44;
extern s8 D_800F4D7C;
extern s16 D_800F4D8A;
extern u16 D_800FFC10;
int func_80035024(int);
s32 func_80059E40(s32);
void func_8005CA14(void);
s32 func_8005CA88(void);
void SetDispMask(s32);
void func_8003A544(void);

#endif
