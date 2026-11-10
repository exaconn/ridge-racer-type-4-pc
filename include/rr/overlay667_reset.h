#ifndef RR_OVERLAY667_RESET_H
#define RR_OVERLAY667_RESET_H
#include "rr/frame_loop.h"

extern s32 ovl667_tx_miss_count,ovl667_rx_miss_count;
extern s32 ovl667_rx_packet_count,ovl667_tx_packet_count,ovl667_read_pending_flag;
extern u8 ovl667_checksum_error_flag;
void ovl667_close_sio_session(void);
void ovl667_abort_on_link_timeout(void);
#endif
