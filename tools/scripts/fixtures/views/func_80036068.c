#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define LinkHostFlag (*(short (*))&LinkHostFlag)

#undef LinkHostFlag

/* legacy typing of shared globals in this part of the unit */
#define LinkEntryState (*(char (*)[])&LinkEntryState)

#undef LinkEntryState

/* legacy typing of shared globals in this part of the unit */
#define LinkHostFlag (*(s16 (*))&LinkHostFlag)

#undef LinkHostFlag

/* legacy typing of shared globals in this part of the unit */
#define ModeFrame (*(unsigned int (*))&ModeFrame)

#define ModeIndex (*(unsigned short (*))&ModeIndex)

#define SubmodeState (*(unsigned short (*))&SubmodeState)

#undef ModeFrame

#undef ModeIndex

#undef SubmodeState

/* legacy typing of shared globals in this part of the unit */
#define SubmodeState (*(unsigned short (*))&SubmodeState)

#define ModeFrame (*(unsigned int (*))&ModeFrame)

#undef SubmodeState

#undef ModeFrame

/* legacy typing of shared globals in this part of the unit */
#define SubmodeState (*(unsigned short (*))&SubmodeState)

#undef SubmodeState

/* legacy typing of shared globals in this part of the unit */
#define ModeFrame (*(u32 (*))&ModeFrame)

#undef ModeFrame

/* legacy typing of shared globals in this part of the unit */
#define SelectionState (*(struct w3g2s (*))&SelectionState)

#undef SelectionState

extern int SoundSetMasterVolume();

extern s32 MaskBitSetting;

void StopRaceAudioAndRumble(void) {
    SoundSetMasterVolume(0, 0);
    StopRaceRumble();
    MaskBitSetting = 0;
}
