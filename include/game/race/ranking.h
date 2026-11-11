#ifndef RR_GAME_RACE_RANKING_H
#define RR_GAME_RACE_RANKING_H

#include "types.h"

/* Course record tables, indexed [direction][course] (direction 0 normal,
 * 1 reverse) unless noted.
 *   CourseLapCounts      0x8009F900  laps per course; row 1 is the time-attack row
 *   CourseBestLap        0x800ACC28  best lap time
 *   CourseBestLapTimes   0x800F3838  best lap split times, three per course
 *   CourseBestLapRecords 0x800F3B00  time-attack best lap per course */
extern u8 CourseLapCounts[2][8];
extern s32 CourseBestLap[2][8];
extern s32 CourseBestLapTimes[2][8][3];
extern s32 CourseBestLapRecords[8];

void InitDefaultTimeRecords(void);
void InitCourseChoices(void);
void BackupSaveData(void);
void RestoreSaveData(void);
void MenuSceneNoop(void);
void SplashModeEnter(void);
void SplashModeUpdate(void);
void DrawSmallFontString(s32 x, s32 y, const char *text, s32 color);

#endif /* RR_GAME_RACE_RANKING_H */
