#include "rr/spu_tick_control.h"
void setIntrVSync(s32 slot,SpuTickCallback callback)
{
    SpuTickCallback *base=VSyncCallbackTable;
    SpuTickCallback *p=base+slot;
    if(callback!=*p) *p=callback;
}
