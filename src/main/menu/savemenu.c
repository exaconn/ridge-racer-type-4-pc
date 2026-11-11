#include "rr/save_slots.h"
/* RODATA 0x80011570 0x18 */
#include "game/data/cardini.h"
#include "game/data/savedat.h"
#include "game/data/savebld.h"
#include "game/system/modeinit.h"
#include "game/menu/savemenu.h"
#include "rr/bios_devices.h"
#include "rr/save_buffer.h"
#include "game/data/savetype.h"

#include "rr/cd_directory_buffer.h"
extern char CardHeaderTitles[][0x40];
extern char CardTitleStrings[][0x40];
extern unsigned char CardSaveStepState;
extern unsigned char CardSaveIsMainData __asm__("CardSaveStepState+1");
extern short CardSaveRetryCount __asm__("CardSaveStepState+2");
extern CardHeaderBlock128 CardExtraDataTitleBuf;
extern CardHeaderBlock128 CardSlotHeaderTitles[3];
extern short CardTransferMode;
extern short GpSaveSlotCursor;
extern char CardTargetFileName[];
extern short CardMenuOperation;
extern int GarageExtraDefaultTimes[];

int CardSaveStep(short initialize, int port) {
    int index;
    int status;
    EmblemSaveData1380 *emblemSave;

    if (initialize > 0) {
        CardSaveStepState = 0;
    }
    switch (CardSaveStepState) {
    case 0:
        CardSaveRetryCount = 0;
        PsyqBzero(SaveBuffer, 0x1AA8);
        index = ExtraModeUnlockedFlag != 0;
        if (initialize == 1) {
            CardSaveIsMainData = 1;
            if (CardTransferMode < 0x10) {
                BuildCardHeaderStatic((CardFileHeader280 *)SaveBuffer, CardHeaderTitles[index], 0x250, 0x1FC, 0x254, 0x1D8, &CardExtraDataTitleBuf);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode & 0x10) {
                BuildCardHeaderStatic((CardFileHeader280 *)SaveBuffer, CardTitleStrings[GpSaveSlotCursor], 0x250, 0x1FC, 0x254, 0x1D8, &CardSlotHeaderTitles[GpSaveSlotCursor]);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode == 0x20) {
                *(CardFileImage4000 *)SaveBuffer = *(const CardFileImage4000 *)CdDirectoryBuffer;
            }
        } else if (initialize >= 2) {
            CardSaveIsMainData = 0;
            if (CardTransferMode < 0x10) {
                BuildCardHeaderStatic((CardFileHeader280 *)SaveBuffer, CardHeaderTitles[index], 0x250, 0x1FC, 0x254, 0x1D8, &CardExtraDataTitleBuf);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode & 0x10) {
                BuildCardHeaderStatic((CardFileHeader280 *)SaveBuffer, CardTitleStrings[GpSaveSlotCursor], 0x250, 0x1FC, 0x254, 0x1D8, &CardSlotHeaderTitles[GpSaveSlotCursor]);
                BuildSaveData(CardTransferMode);
            } else if (CardTransferMode == 0x20) {
                *(CardFileImage4000 *)SaveBuffer = *(const CardFileImage4000 *)CdDirectoryBuffer;
            }
        }
        CardSaveStepState = 1;
        break;
    case 1:
        CardSaveStepState = 2;
        break;
    case 2:
        for (index = 0x1D; index >= 0; index--) {
        }
        if (CardTransferMode == 0x20) {
            status = CardWriteFile((short)port, 0, CardTargetFileName, SaveBuffer, 2, CardSaveIsMainData);
        } else {
            status = CardWriteFile((short)port, 0, CardTargetFileName, SaveBuffer, 1, CardSaveIsMainData);
        }
        if (status == -1) {
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
        index = GetHwCardEvent();
        if (index != 0) {
            if (index == 1) {
                CardSaveStepState = 4;
                switch (CardMenuOperation) {
                case 3:
                    SaveSlotSnapshots[GpSaveSlotCursor] = SelectionState;
                    for (index = 0; index < 3; index++) {
                        SaveSlotSelections[index] = SaveSlotSnapshots[index];
                    }
                    break;
                case 6:
                case 7:
                    emblemSave = (EmblemSaveData1380 *)SaveBuffer;
                    GarageExtraDefaultTimes[GpSaveSlotCursor] = emblemSave->editFrames280;
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
