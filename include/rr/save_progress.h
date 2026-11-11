#ifndef RR_SAVE_PROGRESS_H
#define RR_SAVE_PROGRESS_H
#include "rr/layout.h"
#include "rr/selection_types.h"
typedef union {
    u8 bytes[16];
    struct {
        char name[12];
        s32 configuration0C;
    } fields;
} SaveHeader16;
/* Unlock words following the progress masks; the PS1 map names the first
 * two separately. */
typedef struct {
    s32 extraModeUnlocked50; /* ExtraModeUnlockedFlag (0x800F3970) */
    s32 mainMenuUnlocks54;   /* MainMenuUnlockBitmask (0x800F3974); bit 0 adds a mode */
    u8 bonusUnlocks58[4];    /* set by GP results; gate emblem presets 21..24 */
} SaveUnlocks12;

/* Sixteen progress masks shared by the garage counts and result screens. */
typedef struct {SaveHeader16 prefix00;u32 progress10[4][4];} SaveProgressPrefix50;
typedef struct {
    SaveProgressPrefix50 progress;
    SaveUnlocks12 unlocks50;
    s8 flags5C[4];
} SaveProgressRecord60;
/* Saved progress (0x800F3920..0x800F399C), copied whole to the save file.
 * Resident initialization clears0x7C bytes and seeds four records at0x6C.
 * unlocked60 holds per-team clear counts; the PS1 map names unlocked60[1]
 * NextMenuTransitionStyle (0x800F3981). */
typedef struct {
    SaveProgressRecord60 prefix;
    u8 unlocked60[9],reserved69[3];
    SelectionRow history6C[4];
} SaveProgressRecord7C;
extern SaveProgressRecord7C SaveProgress;
RR_ASSERT_SIZE(SaveHeader16,16);
RR_ASSERT_SIZE(SaveUnlocks12,12);
RR_ASSERT_OFFSET(SaveProgressRecord60,unlocks50,0x50);
RR_ASSERT_SIZE(SaveProgressRecord7C,0x7C);
RR_ASSERT_OFFSET(SaveProgressRecord7C,unlocked60,0x60);
RR_ASSERT_OFFSET(SaveProgressRecord7C,history6C,0x6C);
RR_ASSERT_SIZE(SaveProgressPrefix50,0x50);
RR_ASSERT_OFFSET(SaveProgressPrefix50,progress10,0x10);
RR_ASSERT_SIZE(SaveProgressRecord60,0x60);
RR_ASSERT_OFFSET(SaveProgressRecord60,flags5C,0x5C);
#endif
