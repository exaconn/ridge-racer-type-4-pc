#include "types.h"

#include "rr/track_sample_types.h"

extern int SetModelTexturePage();

extern int InitBadgeRow();

extern int InitRect16Row();

extern int InitImageRow();

extern s32 MaskBitSetting;

void InitPreviewImageRows(void) {
    SetModelTexturePage(0x19);
    MaskBitSetting = 0;
    InitBadgeRow(0, 0x10, 0x158, 0x120, 0x3C, 0, 0x103);
    InitRect16Row(0, 0x10, 0x48, 0x120, 0x120, 0, 0);
    InitImageRow(0, -1, 0, 1);
}
