#include "rr/driver_control.h"
void ovl667_func_80115494(void)
{
    ResetGraph(1);
    func_8009AEAC(3,0,1);
    while(func_8009AEAC(3,1,0)==0){}
    func_8009AEAC(5,0,0);
    func_8009AEAC(3,0,0);
    VSync(0);
    SetDispMask(1);
}
