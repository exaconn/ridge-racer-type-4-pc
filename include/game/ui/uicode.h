#ifndef RR_GAME_UI_UICODE_H
#define RR_GAME_UI_UICODE_H

#include "types.h"

/* XOR/cipher the six password bytes in place and write 14 code characters
 * (checksum pair then one pair per byte). */
void EncodePasswordCode(u8 *bytes, u8 *chars);

#endif /* RR_GAME_UI_UICODE_H */
