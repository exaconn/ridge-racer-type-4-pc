#ifndef RR_PSYQ_LIBCARD_CARDC162_H
#define RR_PSYQ_LIBCARD_CARDC162_H
#include "types.h"
/* Starts PocketStation card command 0x5A (get directory index, flags, serial,
 * date and time) on channel port*16+slot; the reply lands in buffer
 * (RR_CARD_RAW_REPLY_BYTES). Returns nonzero when the command was accepted. */
s32 _cardCmd5ABegin(s32 request, u8 *buffer);
#endif /* RR_PSYQ_LIBCARD_CARDC162_H */
