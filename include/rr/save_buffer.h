#ifndef RR_SAVE_BUFFER_H
#define RR_SAVE_BUFFER_H
#include "types.h"
/* Shared loading allocation: card headers, game saves and full card images
 * are serialized views of this byte arena, not separate pointer objects. */
extern u8 *SaveBuffer;
#endif
