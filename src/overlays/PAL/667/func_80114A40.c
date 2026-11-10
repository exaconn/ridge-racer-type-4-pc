#include "rr/serial_handles.h"
void ovl667_close_sio_session(void){
 _sio_control(2,3,0);_sio_control(2,0,0);
 DisableEvent(ovl667_sio_read_event);DisableEvent(ovl667_sio_error_event);
 FileClose(LinkWriteHandle);FileClose(LinkReadHandle);
 CloseEvent(ovl667_sio_read_event);CloseEvent(ovl667_sio_error_event);
 _sio_control(4,0,0);_sio_control(1,1,1);
}
