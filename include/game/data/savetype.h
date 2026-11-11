#ifndef RR_GAME_DATA_SAVETYPE_H
#define RR_GAME_DATA_SAVETYPE_H

#include "rr/save_progress.h"
#include "rr/runtime_settings.h"
#include "rr/course_choice_types.h"
#include "rr/save_slot_types.h"
#include "rr/card_header_types.h"
#include "rr/time_record_types.h"
#include "rr/emblem_tim_types.h"

/* Layout of the save buffer shared by the save builder and the save writer. */

typedef struct {
    CardFileHeader280 header00;
    SaveProgressRecord7C progress280;
    RuntimeSettings options2FC;
    CourseChoice4 courseChoices33C[9];
    SavedMenuSettings slots360[3];
    int bestLapTimes3E4[2][8][3];
    TimeRecord16 lapRecords4A4[2][8][5];
    TimeRecord16 raceRecords9A4[2][8][5];
    int emblemWordsEA4[0x210];
    unsigned char reserved16E4[0x3C0];
    u32 checksum1AA4;
} GameSaveFields1AA8;
typedef union {
    GameSaveFields1AA8 fields;
    u32 words[0x6AA];
} GameSaveData1AA8;
RR_ASSERT_SIZE(GameSaveFields1AA8,0x1AA8);
RR_ASSERT_SIZE(GameSaveData1AA8,0x1AA8);
RR_ASSERT_OFFSET(GameSaveFields1AA8,slots360,0x360);
RR_ASSERT_OFFSET(GameSaveFields1AA8,checksum1AA4,0x1AA4);

/* A 0x100-byte read from file offset 0x300 is staged at arena +0x1F00. */
enum {RR_SAVE_SLOT_SCAN_BUFFER_OFFSET=0x1F00, RR_SAVE_SLOT_SCAN_FILE_OFFSET=0x300};
typedef struct {
    u8 settingsTail00[0x3C];
    CourseChoice4 choices3C[9];
    SavedMenuSettings slots60[3];
    s32 lapTimesE4[7];
} CardSlotScanWindow100;
RR_ASSERT_SIZE(CardSlotScanWindow100,0x100);
RR_ASSERT_OFFSET(CardSlotScanWindow100,slots60,0x60);
RR_ASSERT_OFFSET(CardSlotScanWindow100,lapTimesE4,0xE4);

typedef struct {
    CardFileHeader280 header00;
    int editFrames280;
    unsigned char reserved284[0x7C];
    EmblemTim840 decals300[2];
} EmblemSaveData1380;
RR_ASSERT_SIZE(EmblemSaveData1380,0x1380);
RR_ASSERT_OFFSET(EmblemSaveData1380,editFrames280,0x280);
RR_ASSERT_OFFSET(EmblemSaveData1380,decals300,0x300);

/* One 8 KiB card block; slot-header scanning uses its final 256 bytes. */
typedef union {
    GameSaveData1AA8 game;
    EmblemSaveData1380 emblem;
    u32 words[0x800];
    u16 halves[0x1000];
    u8 bytes[0x2000];
    struct {u8 prefix00[0x1F00]; CardSlotScanWindow100 window1F00;} scan;
} CardSaveBlock2000;
RR_ASSERT_SIZE(CardSaveBlock2000,0x2000);


#endif
