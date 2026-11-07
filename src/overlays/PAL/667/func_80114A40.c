#include "rr/serial_handles.h"
void ovl667_func_80114A40(void){
 _sio_control(2,3,0);_sio_control(2,0,0);
 func_80096EC4(ovl667_D_80118ED8);func_80096EC4(ovl667_D_80118EDC);
 FileClose(D_800F2AAC);FileClose(D_800F2AA8);
 CloseEvent(ovl667_D_80118ED8);CloseEvent(ovl667_D_80118EDC);
 _sio_control(4,0,0);_sio_control(1,1,1);
}
