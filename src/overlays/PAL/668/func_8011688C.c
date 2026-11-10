#include "types.h"
extern u32 *ovl668_mdec1_ctrl_ptr;
u32 ovl668_mdec_read_status(void) { return *ovl668_mdec1_ctrl_ptr; }
