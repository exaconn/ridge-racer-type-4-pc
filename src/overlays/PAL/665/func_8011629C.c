#include "game/system/pad.h"
#include "rr/menu_pulse.h"
s32 ovl665_detect_controller_kind(s32 index)
{
    ControllerState5C *item=&PlayerControllers[index];
    s32 result=-1;
    switch(item->input00.fields.type){
    case 0x41:case 0x73:
        if(item->kind1C){result=3;if(item->flag40==0)result=1;}
        else result=item->input00.fields.type==0x73;
        break;
    case 0x23:result=2;break;
    case 0xE3:result=3;break;
    }
    return result;
}
