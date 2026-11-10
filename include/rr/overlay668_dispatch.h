#ifndef RR_OVERLAY668_DISPATCH_H
#define RR_OVERLAY668_DISPATCH_H
#include "rr/menu_choice.h"
#include "rr/overlay668_decode.h"
/* Current playback phase:0 starts from the published resource buffer,
 *1 updates playback,2 shuts it down. Other signed word values are idle. */
extern s32 D_800AC704;
void ovl668_update_stream_mode(void);
void ovl668_start_stream_playback(u8 *resource);
void ovl668_step_stream_playback(void);
#endif
