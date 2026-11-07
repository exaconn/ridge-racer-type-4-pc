#include "types.h"
extern int CdAudioBeginPlay();
extern struct { char p0[0x2664]; s32 x2664; char p1[0x4]; s32 x266C; char p2[0x4]; s32 x2674; char p3[0xC]; s32 x2684; char p4[0x10]; s32 x2698; char p5[0x8]; s32 x26A4; } SoundState;
void CdAudioLoopStep(void) {
    switch (SoundState.x26A4) {
    case 0:
        SoundState.x2674 = 0;
        SoundState.x26A4 = 1;
        SoundState.x2664 = SoundState.x2684;
        SoundState.x266C = SoundState.x2698;
        break;
    case 1:
        if (SoundState.x2674 == 0) {
            CdAudioBeginPlay(1);
            SoundState.x26A4 = 2;
        }
        break;
    }
}
