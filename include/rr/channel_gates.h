#ifndef RR_CHANNEL_GATES_H
#define RR_CHANNEL_GATES_H
#include "rr/channels.h"
#include "rr/bios_devices.h"

/* Clears six output gates, then reserves costs below signed budget 61 for rows
 * selected by payload5D and data28. Each entries04 row covers five bytes;
 * entry_countE9 rows must be accessible whenever the active path is entered.
 * Input length is capped at six. The inactive response4/5/7 path with pending0
 * and length>=2 requires two readable input bytes. Byte buffers may overlap gate/mapping
 * storage.
 * Cost addition wraps32 before the signed comparison. */
void func_8009A05C(void *record);
#endif
