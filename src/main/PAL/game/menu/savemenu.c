/* RODATA 0x80011570 0x18 */
#include "game/data/cardini.h"
#include "game/data/savedat.h"
#include "game/data/savebld.h"
#include "game/system/modeinit.h"
#include "game/menu/savemenu.h"
#include "rr/bios_devices.h"
typedef struct {
    char b[0x4000];
} Struct4000;

typedef struct {
    char b[0x2C];
} Struct2C;


#include "rr/cd_directory_buffer.h"
extern char CardHeaderTitles[][0x40];
extern char CardTitleStrings[][0x40];
extern unsigned char CardSaveStepState;
extern unsigned char CardSaveIsMainData __asm__("CardSaveStepState+1");
extern short CardSaveRetryCount __asm__("CardSaveStepState+2");
extern char CardExtraDataTitleBuf[];
extern char CardSlotHeaderTitles[][0x80];
extern Struct4000 *SaveBuffer;
extern Struct2C SelectionState;
extern Struct2C SaveSlotSelections[3];
extern short CardTransferMode;
extern short GpSaveSlotCursor;
extern char CardTargetFileName[];
extern short CardMenuOperation;
extern Struct2C SaveSlotSnapshots[3];
extern int GarageExtraDefaultTimes[];

int CardSaveStep(short a0, int a1) {
    int i;
    int r;
    Struct4000 *p;

    if (a0 > 0) {
        CardSaveStepState = 0;
    }
    switch (CardSaveStepState) {
    case 0:
        CardSaveRetryCount = 0;
        PsyqBzero(SaveBuffer, 0x1AA8);
        i = ExtraModeUnlockedFlag != 0;
        if (a0 == 1) {
            CardSaveIsMainData = 1;
            if (CardTransferMode < 0x10) {
                BuildCardHeaderStatic(SaveBuffer, CardHeaderTitles[i], 0x250, 0x1FC, 0x254, 0x1D8, CardExtraDataTitleBuf);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode & 0x10) {
                BuildCardHeaderStatic(SaveBuffer, CardTitleStrings[GpSaveSlotCursor], 0x250, 0x1FC, 0x254, 0x1D8, CardSlotHeaderTitles[GpSaveSlotCursor]);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode == 0x20) {
                *SaveBuffer = *(const Struct4000 *)CdDirectoryBuffer;
            }
        } else if (a0 >= 2) {
            CardSaveIsMainData = 0;
            if (CardTransferMode < 0x10) {
                BuildCardHeaderStatic(SaveBuffer, CardHeaderTitles[i], 0x250, 0x1FC, 0x254, 0x1D8, CardExtraDataTitleBuf);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode & 0x10) {
                BuildCardHeaderStatic(SaveBuffer, CardTitleStrings[GpSaveSlotCursor], 0x250, 0x1FC, 0x254, 0x1D8, CardSlotHeaderTitles[GpSaveSlotCursor]);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode == 0x20) {
                *SaveBuffer = *(const Struct4000 *)CdDirectoryBuffer;
            }
        }
        CardSaveStepState = 1;
        break;
    case 1:
        CardSaveStepState = 2;
        break;
    case 2:
        for (i = 0x1D; i >= 0; i--) {
        }
        if (CardTransferMode == 0x20) {
            r = CardWriteFile((short)a1, 0, CardTargetFileName, SaveBuffer, 2, CardSaveIsMainData);
        } else {
            r = CardWriteFile((short)a1, 0, CardTargetFileName, SaveBuffer, 1, CardSaveIsMainData);
        }
        if (r == -1) {
            if (CardSaveRetryCount < 3) {
                CardSaveRetryCount = (unsigned short)CardSaveRetryCount + 1;
                break;
            }
            CardSaveStepState = 5;
        } else {
            CardSaveStepState = 3;
        }
        break;
    case 3:
        i = GetHwCardEvent();
        if (i != 0) {
            if (i == 1) {
                CardSaveStepState = 4;
                switch (CardMenuOperation) {
                case 3:
                    SaveSlotSnapshots[GpSaveSlotCursor] = SelectionState;
                    for (i = 0; i < 3; i++) {
                        SaveSlotSelections[i] = SaveSlotSnapshots[i];
                    }
                    break;
                case 6:
                case 7:
                    p = SaveBuffer;
                    GarageExtraDefaultTimes[GpSaveSlotCursor] = ((int *)p)[0x280 / 4];
                    break;
                }
            } else {
                if (CardSaveRetryCount >= 3) {
                    CardSaveStepState = 5;
                } else {
                    CardSaveStepState = 1;
                    CardSaveRetryCount = (unsigned short)CardSaveRetryCount + 1;
                }
            }
        }
        break;
    case 4:
    case 5:
        break;
    }
    return CardSaveStepState;
}
