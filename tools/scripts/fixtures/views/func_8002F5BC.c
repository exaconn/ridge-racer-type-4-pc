#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define SaveProgress (*(char (*)[])&SaveProgress)

#undef SaveProgress

extern s16 MainMenuChoice;

extern s16 CardMenuOperation;

void SetNextModeByGameType(void){
 switch(MainMenuChoice){
 case 0:ActiveSceneId=0x105;break;
 case 1:default:ActiveSceneId=0x131;break;
 case 2:ActiveSceneId=0x137;break;
 case 3:ActiveSceneId=0x127;break;
 case 6:ActiveSceneId=0x201;break;
 case 4:ActiveSceneId=0x149;break;
 case 5:CardMenuOperation=0;ActiveSceneId=0x147;break;
 case 7:ActiveSceneId=0x14E;break;
 case 8:ActiveSceneId=0x13B;break;
 }
}
