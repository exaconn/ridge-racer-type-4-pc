#ifndef RR_GAME_DATA_SAVEDAT_H
#define RR_GAME_DATA_SAVEDAT_H

#include "rr/card_header_types.h"

int ApplySaveData(int mode);
void BuildCardHeaderStatic(CardFileHeader280 *header, const char *title,
    s32 paletteX, s32 paletteY, s32 imageX, s32 imageY,
    const CardHeaderBlock128 *extra);
void BuildCardHeaderAnim(CardFileHeader280 *header, const char *title,
    s32 paletteX, s32 paletteY, s32 imageX, s32 imageY);


#endif /* RR_GAME_DATA_SAVEDAT_H */
