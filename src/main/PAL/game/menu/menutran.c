/* RODATA 0x80010514 0x24 */
#include "rr/menu_transition.h"
#include "game/menu/menutran.h"
#include "game/menu/menu8.h"
#include "game/data/ldscn1.h"
#include "game/data/globals.h"
#include "game/data/selection.h"

typedef struct { int x; } __attribute__((packed)) U4;

extern s16 CardMenuOperation;
extern int PsyqBzero();
extern int DecodeCourseIndex();
extern unsigned char D_8009F06C[];
extern short ComDifficultyLevel;
extern short SelectedCourseChoice;
extern short SelectedTeamIndex;
extern short SelectedMakerIndex;
extern short CourseIndex;
extern signed char SelectionState[];
extern unsigned char D_800AD4F0;
extern unsigned char D_800F3981;

void SetNextModeByGameType(void);

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

void UpdateGrandPrixStageProgress(void) {
    int x, c0, i0, j;
    int a0;
    register int v1;
    int a2;
    char *s0, *p;
    a2 = (unsigned char)SelectionState[0x17];
    i0 = (signed char)a2;
    if (i0 != -1) {
        if (SelectionState[0x19] <= D_8009F06C[i0 / 2]) {
            if (SelectionState[i0] != -1) {
                SelectionState[0x17] = a2 + 1;
            }
        }
    }
    a0 = D_800AD4EF;
    if (a0 == -1) {
        PsyqBzero(RankEntries, 0x40);
        j = 0;
        do {
            RankEntries[j].gridRank06 = j;
            RankEntries[j].carIndex04 = j;
            j++;
        } while (j < 8);
        v1 = 0;
        PsyqBzero(SelectionRows, 0x20);
        PsyqBzero(SelectionState, 0x2C);
        a0 = 0;
        v1 = 0;
        a2 = -1;
        *(U4 *)&SelectionState[0x1C] = *(U4 *)&SelectionRows[0];
        ComDifficultyLevel = 0;
        do {
            p = (char *)SelectionState + v1;
            v1++;
            p[0] = a2;
            p[8] = 3;
        } while (v1 < 8);
        D_800AD4F0 = D_800F3981;
    }
    x = (unsigned char)SelectionState[0x17];
    x = (x << 24) >> 25;
    SelectionState[0x15] = x;
    if ((x & 0xFF) >= 3U) {
        SelectionState[0x15] = 2;
    }
    *(U4 *)&SelectionRows[0] = *(U4 *)&SelectionState[0x1C];
    SelectedCourseChoice = a0;
    c0 = SelectionRows[0];
    SelectedTeamIndex = c0;
    SelectedMakerIndex = SelectionRows[1];
    SelectedMakerCarIndex = SelectionRows[2];
    CourseIndex = DecodeCourseIndex(a0, c0, a2);
    ComDifficultyLevel = 0;
    SaveProgress.prefix.progress.prefix00.fields.configuration0C |= 1 << SelectedCourseChoice;
    if (SelectionRows[2] >= 5) {
        ComDifficultyLevel = (SelectionRows[2] - 5) % 7;
        return;
    }
    if (SelectionRows[2] != 0) {
        ComDifficultyLevel = SelectionRows[2] - 1;
    }
}

int CountProgressFlags(void) {
    int a, b, i;
    int n = 0;
    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            for (i = 0; i < 20; i++) {
                if ((SaveProgress.prefix.progress.progress10[a][b] >> i) & 1) {
                    n++;
                }
            }
        }
    }
    return n;
}

