#ifndef RR_PSYQ_LIBPAD_PADGATE_H
#define RR_PSYQ_LIBPAD_PADGATE_H
#include "types.h"
/* Input buffers each permit two byte stores and may overlap each other or
 * byte fields in the record arena; pointer slots must remain valid. Clears
 * both channel records, installs callbacks and publishes ready last. */
void PadInitDirect(u8 *first, u8 *second);
#endif
