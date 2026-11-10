#include "rr/overlay667_reset.h"
void ovl667_abort_on_link_timeout(void){
 if(ovl667_tx_miss_count>=7 || ovl667_rx_miss_count>=7 || ovl667_checksum_error_flag==1){
  ovl667_tx_miss_count=0;ovl667_rx_miss_count=0;ovl667_rx_packet_count=0;
  ovl667_tx_packet_count=0;ovl667_read_pending_flag=0;ModeFrame=0;
  ovl667_close_sio_session();ModeGroup=1;ModeIndex=44;SubmodeState=0;
 }
}
