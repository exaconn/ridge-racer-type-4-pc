#include "rr/overlay_menu.h"
s32 ovl661_func_80117E10(void){
 SetMenuTextRowStyle(MenuCursor+38,0,0);
 StepMenuValue(0,2,(char*)&MenuCursor,3,0);
 SetMenuTextRowStyle(MenuCursor+38,1,1);
 if(D_800F3A60){PlaySfx(1);return MenuCursor;}
 return -1;
}
