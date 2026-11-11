#include "rr/emblem_images.h"
#include "rr/save_slots.h"
/* RODATA 0x800114a0 0xc4 */
#include "game/gfx/color.h"
#include "psyq/libc/printf.h"
#include "game/data/savedat.h"
#include "game/data/savetype.h"
#include "game/data/globals.h"
#include "rr/menu_selection.h"
#include "rr/overlay662_frame_step.h"
#include "rr/overlay668_playback_step.h"
#include "game/race/raceset.h"
#include "rr/course_choices.h"
#include "rr/audio_loop.h"
#include "rr/overlay667_course_initializer.h"
#include "rr/string_tail_copy.h"

extern int CourseBestLap[2][8];
extern TimeRecord16 RaceRecords[2][8][5];
extern int CourseBestLapTimes[2][8][3];
extern EmblemTim840 *GarageSavedDecalTimB;
extern EmblemTim840 *GarageSavedDecalTimA;
extern short GpSaveSlotCursor;
extern TimeRecord16 LapRecords[2][8][5];
extern CourseChoice4 LinkCarChoiceTable[2][2][9];
extern char VsPlayerNames[2][12];
extern CourseChoice4 VsCarChoiceTable[2][2][9];
extern int GarageExtraDefaultTimes[];
extern char StrFmtPercentS[];
extern GpuRectangle SavedEmblemClutRect;
extern char StrCardMagicCrd0[];
extern GpuRectangle SavedEmblemPixelRect;


int ApplySaveData(int mode) {
    int result = 0;
    GameSaveData1AA8 *gameSave;
    EmblemSaveData1380 *emblemSave;
    u32 checksum;
    int i;
    int j;
    int k;
    int headerFrames;
    int emblemFrames;

    switch (mode) {
    case 0:
    case 1:
    case 0x30:
    default:
        gameSave = (GameSaveData1AA8 *)SaveBuffer;
        checksum = 0;
        for (i = 0; (u32)i < 0x6A9U; i++) {
            checksum += gameSave->words[i];
        }
        if (gameSave->fields.checksum1AA4 == checksum) {
            SaveProgress = gameSave->fields.progress280;
            Options = gameSave->fields.options2FC;
            for (i = 0; i < 3; i++) {
                if (mode == 0x30) {
                    SaveSlotSelections[i] = gameSave->fields.slots360[i];
                } else {
                    SaveSlotSnapshots[i] = gameSave->fields.slots360[i];
                }
            }
            for (i = 0; i < 9; i++) {
                CourseChoiceTable[1][i] = gameSave->fields.courseChoices33C[i];
            }
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 8; j++) {
                    for (k = 0; k < 3; k++) {
                        CourseBestLapTimes[i][j][k] = gameSave->fields.bestLapTimes3E4[i][j][k];
                    }
                    for (k = 0; k < 5; k++) {
                        LapRecords[i][j][k] = gameSave->fields.lapRecords4A4[i][j][k];
                        RaceRecords[i][j][k] = gameSave->fields.raceRecords9A4[i][j][k];
                    }
                    CourseBestLap[i][j] = LapRecords[i][j][0].time;
                    CourseBestTotal[i][j] = RaceRecords[i][j][0].time;
                }
            }
            for (i = 0; i < 0x210; i++) {
                CustomEmblemBuffers.words[i] = gameSave->fields.emblemWordsEA4[i];
            }
            ApplySoundVolumes();
            result = 1;
        }
        break;
    case 2:
        gameSave = (GameSaveData1AA8 *)SaveBuffer;
        checksum = 0;
        for (i = 0; (u32)i < 0x6A9U; i++) {
            checksum += gameSave->words[i];
        }
        if (gameSave->fields.checksum1AA4 == checksum) {
            for (i = 0; i < 9; i++) {
                if (GameModeType == 2 || GameModeType == 3) {
                    VsCarChoiceTable[ActivePlayerIndex & 1][1][i] = gameSave->fields.courseChoices33C[i];
                    CopyStringClearTail(VsPlayerNames[ActivePlayerIndex & 1], gameSave->fields.progress280.prefix.progress.prefix00.fields.name);
                } else if (GameModeType == 4) {
                    LinkCarChoiceTable[ActivePlayerIndex & 1][1][i] = gameSave->fields.courseChoices33C[i];
                    CopyStringClearTail(LinkPlayerNames[ActivePlayerIndex], gameSave->fields.progress280.prefix.progress.prefix00.fields.name);
                }
            }
            for (i = 0; i < 0x210; i++) {
                *(u32 *)(CustomEmblemBuffers.bytes + (ActivePlayerIndex & 1) * sizeof(EmblemTim840) + i * sizeof(u32)) = gameSave->fields.emblemWordsEA4[i];
            }
            result = 1;
        }
        break;
    case 0x10:
        headerFrames = ((CardSaveBlock2000 *)SaveBuffer)->words[0];
        GarageExtraDefaultTimes[GpSaveSlotCursor] = headerFrames;
        result = 1;
        break;
    case 0x11:
        emblemFrames = (emblemSave = (EmblemSaveData1380 *)SaveBuffer)->editFrames280;
        EmblemEditFrames = GarageExtraDefaultTimes[GpSaveSlotCursor] = emblemFrames;
        for (j = 0; j < 2; j++) {
            for (i = 0; i < 0x840; i++) {
                if (j == 0) {
                    GarageSavedDecalTimA->bytes[i] = emblemSave->decals300[0].bytes[i];
                } else {
                    GarageSavedDecalTimB->bytes[i] = emblemSave->decals300[j].bytes[i];
                }
            }
        }
        result = 1;
        break;
    }
    return result;
}

void BuildCardHeaderAnim(CardFileHeader280 *header, const char *title,
    s32 paletteX, s32 paletteY, s32 imageX, s32 imageY) {
    int frameIndex;
    /* GPU rectangle coordinates wrap in their signed 16-bit fields. */
    s16 frameX;
    header->magic00[0] = 0x53;
    header->magic00[1] = 0x43;
    header->iconMode02 = 0x13;
    header->blockCount03 = 1;
    PsyqSprintf((s8 *)header->title04, (const s8 *)StrFmtPercentS, title);
    SavedEmblemClutRect.rect.x = paletteX;
    SavedEmblemClutRect.rect.w = 0x10;
    SavedEmblemClutRect.rect.y = paletteY;
    SavedEmblemClutRect.rect.h = 1;
    StoreImage(&SavedEmblemClutRect, header->palette60);
    DrawSync(0);
    frameX = imageX;
    frameIndex = 0;
    do {
        SavedEmblemClutRect.rect.x = frameX;
        SavedEmblemClutRect.rect.y = imageY;
        SavedEmblemClutRect.rect.w = 4;
        SavedEmblemClutRect.rect.h = 0x10;
        StoreImage(&SavedEmblemClutRect, &header->payload80.frames[frameIndex]);
        DrawSync(0);
        frameIndex++;
        frameX = frameX + 4;
    } while (frameIndex < 3);
}

void BuildCardHeaderStatic(CardFileHeader280 *header, const char *title,
    s32 paletteX, s32 paletteY, s32 imageX, s32 imageY,
    const CardHeaderBlock128 *extra) {
    int frameIndex;
    header->magic00[0] = 0x53;
    header->magic00[1] = 0x43;
    header->iconMode02 = 0x11;
    header->blockCount03 = 1;
    PsyqSprintf((s8 *)header->title04, (const s8 *)StrFmtPercentS, title);
    header->flags50[0] = 1;
    header->flags50[1] = 0;
    PsyqSprintf((s8 *)header->applicationTag52, (const s8 *)StrFmtPercentS, StrCardMagicCrd0);
    SavedEmblemPixelRect.rect.x = paletteX;
    SavedEmblemPixelRect.rect.w = 0x10;
    SavedEmblemPixelRect.rect.y = paletteY;
    SavedEmblemPixelRect.rect.h = 1;
    StoreImage(&SavedEmblemPixelRect, header->palette60);
    DrawSync(0);
    frameIndex = 0;
    while (frameIndex < 1) {
        SavedEmblemPixelRect.rect.x = imageX;
        SavedEmblemPixelRect.rect.y = imageY;
        SavedEmblemPixelRect.rect.w = 4;
        SavedEmblemPixelRect.rect.h = 0x10;
        StoreImage(&SavedEmblemPixelRect, &header->payload80.frames[frameIndex]);
        DrawSync(0);
        frameIndex++;
        imageX = (u32)imageX + 4U;
    }
    header->payload80.single.extra[0] = *extra;
    header->payload80.single.extra[1] = *extra;
    header->payload80.single.extra[2] = *extra;
}
