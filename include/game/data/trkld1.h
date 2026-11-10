#ifndef RR_GAME_DATA_TRKLD1_H
#define RR_GAME_DATA_TRKLD1_H

#include "types.h"

extern s32 RaceSoundTrackLoadState;
extern s32 RaceLoadStepIndex;
extern int RaceLoadState;
extern int RaceLoadSubState;
extern int RaceLoadCursor;
int LoadCarSpecStep();
int LoadMenuCarStep();
int LoadCourseAssetsStep();
int LoadVabPairStep(char *a0);
void RaceLoadReset(void);
s32 LoadMenuSoundVabStep(void);
s32 LoadMenuSoundSeqStep(void);
int LoadRaceSoundStep(void);
void SetRaceLoadCursor(s32 arg0);
s32 GetRaceLoadCursor(void);
int LoadSlotBankStep(int a0, int a1);
int LoadComSlotBankStep(int a0);
int LoadSoundBanksStep(int a0, int a1, int a2);
int LoadSfxVabStep(int a0, int a1);


#endif /* RR_GAME_DATA_TRKLD1_H */
