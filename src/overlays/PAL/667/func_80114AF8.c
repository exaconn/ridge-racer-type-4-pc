#include "rr/overlay667_packet.h"
void ovl667_init_sio_session(unsigned long configuration) {
    s32 work;
    u32 *base;
    /* Three word lanes in the genuine24-word ring. Keep cursor arithmetic
     * at full pointer width: after the last store, the unused second/third
     * integer cursors advance beyond the ring without forming C pointers. */
    unsigned long first;
    unsigned long second;
    unsigned long third;
    SetDispMask(0);
    _sio_control(1,3,configuration);
    _sio_control(1,4,4);
    _sio_control(4,0,(unsigned long)ovl667_check_sio_timeout);
    work=0;
    base=ovl667_rx_packet_ring.words;
    third=(unsigned long)base+8;
    second=(unsigned long)base+4;
    first=(unsigned long)base;
    ovl667_checksum_error_flag=0;
    ovl667_active_slot_index=0;
    ovl667_consumed_slot_index=7;
    ovl667_rx_slot_index=0;
    ovl667_tx_slot_index=0;
    ovl667_rx_miss_count=0;
    ovl667_tx_miss_count=0;
    ovl667_rx_packet_count=0;
    ovl667_tx_packet_count=0;
    ovl667_read_pending_flag=0;
    ovl667_sync_retry_state=0;
    ovl667_remote_pause_bits=0;
    ovl667_local_pause_bits=0;
    ((u8 *)&ovl667_pending_pause_bits)[0]=0;
    do {
        *(u32 *)first=0;
        *(u32 *)second=0;
        *(u32 *)third=0;
        third+=12;
        second+=12;
        work++;
        first+=12;
    } while (work<8);
    ovl667_tx_staging_packet.words[0]=0;
    ovl667_tx_staging_packet.words[1]=0;
    ovl667_tx_staging_packet.words[2]=0;
    ovl667_rx_staging_packet.words[0]=0;
    ovl667_rx_staging_packet.words[1]=0;
    ovl667_rx_staging_packet.words[2]=0;
    LinkPendingCarInputs.words[0]=0;
    LinkPendingCarInputs.words[1]=0;
    LinkLocalCarInputs.words[0]=0;
    LinkLocalCarInputs.words[1]=0;
    LinkRemoteCarInputs.words[0]=0;
    LinkRemoteCarInputs.words[1]=0;
    ovl667_peer_race_state=0;
    ovl667_link_shutdown_stage=0;
    if (_sio_control(3,1,0) != 0) {
        _sio_control(3,0,1);
    } else {
        _sio_control(3,0,1);
        while (_sio_control(3,1,0)==0) {
            work=((u32)_sio_control(0,0,0)>>7)&3;
            if (work==0 || work==3) break;
        }
    }
    _sio_control(5,0,0);
    VSync(0);
    _sio_control(3,0,0);
}
