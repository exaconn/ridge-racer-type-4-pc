#ifndef RR_GAME_DATA_LDSCN1_H
#define RR_GAME_DATA_LDSCN1_H

#include "types.h"

extern u8 *LoadDestCursor;
extern signed char *DialogueAssetTable;
extern char *TrackNodesEnd;
extern short MainMenuChoice;
int LoadOpeningMovieTimStep(void);
int LoadTimListStep();
int LoadDialogueStep(void);
int LoadSplashAssetsStep(void);
int LoadDefaultRaceStep(void);
int LoadModeMenuAssetsStep(void);
int LoadMenuSoundStep(void);
int LoadSceneStep(void);
int LoadAllCarsStep(void);
int LoadCarRangeStep(int arg0, int arg1);
int LoadIndexedTimStep(void);
int LoadResultsSceneStep(void);
int LoadCarClutStep(void);
int LoadGlyphRowsStep(void);
int LoadMenuCarModelsStep(void);
int LoadTrackNodesStep(void);


#endif /* RR_GAME_DATA_LDSCN1_H */
