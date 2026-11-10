#ifndef RR_GAME_RACE_RANKING_H
#define RR_GAME_RACE_RANKING_H

#include "types.h"

extern u8 CourseLapCounts[2][8];

void InitDefaultTimeRecords(void);
void InitCourseChoices(void);
void BackupSaveData(void);
void RestoreSaveData(void);
void MenuSceneNoop(void);
void SplashModeEnter(void);
void SplashModeUpdate(void);
void DrawSmallFontString(s32 x, s32 y, const char *text, s32 color);

#endif /* RR_GAME_RACE_RANKING_H */
