#ifndef RR_OVERLAY662_NAME_STATE_H
#define RR_OVERLAY662_NAME_STATE_H
#include "rr/layout.h"
/* Complete name storage occupies273A8..273B4, before the selected-key word.
 * Eight positions are displayed and edited; the remaining bytes retain the
 * string terminator/tail used by the resident string-copy helper. */
extern s8 ovl662_name_entry_cursor,ovl662_name_grid_col,ovl662_name_grid_row;
extern u8 ovl662_name_entry_buffer[12];
extern s32 ovl662_name_grid_index;
#endif
