#ifndef RR_OVERLAY668_WAIT_H
#define RR_OVERLAY668_WAIT_H
#include "rr/overlay668_control.h"

extern char ovl668_mdec_in_timeout_msg[];
extern char ovl668_mdec_out_timeout_msg[];
s32 ovl668_mdec_handle_timeout(const char *message);
#endif
