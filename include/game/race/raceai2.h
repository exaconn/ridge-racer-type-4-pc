#ifndef RR_GAME_RACE_RACEAI2_H
#define RR_GAME_RACE_RACEAI2_H

#include "types.h"

void StopRaceRumble(void);
void FinishRace();
int ExitOnLinkLost(void);
int UpdateLinkHandshake(int a0);
void DrawLinkEntryScreen(s32 mode);
int LinkExchange(int a0, int a1, int a2, int a3);
void LinkWaitModeEnter(void);
void LinkWaitModeUpdate(void);
void LinkExchangeModeUpdate(void);
void LinkErrorModeUpdate(void);
void LinkBattleEndModeUpdate(void);
void StopRaceAudioAndRumble(void);
void RaceAbortToMenuUpdate(void);
void InitPostRaceScreen(void);
void PostRaceScreenModeUpdate(void);
void EvaluateGpQualifyingResult(void);
void SortRaceRankTable(void);
void EnterRaceResultScreen(void);
void SortRankAndShowResults(void);
void SortRankAndEnterResults(void);


#endif /* RR_GAME_RACE_RACEAI2_H */
