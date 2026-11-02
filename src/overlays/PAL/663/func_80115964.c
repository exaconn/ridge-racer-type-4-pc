#include "rr/overlay663_menu_handler.h"
void ovl663_func_80115964(void) {
 s32 old;
 if(func_8003BEA8(ovl663_D_801168A0[ovl663_D_80116898])) {
  switch(D_800F4D8A) {
  case 0:
   if(D_800F4D80->byte14==2){func_8002AF8C(1,1,0);D_800F4D8A=1;}
   break;
  case 1:
   old=ovl663_D_80116898;
   func_8002B330(1,4,(char*)&ovl663_D_80116898,ovl663_D_801168CC,0);
   if(old!=ovl663_D_80116898)func_8003A198();else ovl663_func_801158B4();
   D_800F4D80->cells[0]=2;
   if(D_800F3A60){
    func_8004F24C(1);func_8002AF8C(0,1,0);D_800F4D8A=8;
    D_800F4D80->cells[0]=1;D_800AC5D0=64;D_800F4D80->byte14=3;
   }
   break;
  case 8:
   D_800AC5D0--;
   func_800207D4(((RrFrameOrderingPrefix*)D_800ACC14)->ordering70,(64-D_800AC5D0)*4,69);
   if(D_800AC5D0<=0){ovl663_func_801148D8();D_800F4C6A=1;D_800F3A26=74;D_800F4D8A=0;return;}
   break;
  }
 }
 if(ovl663_D_801168CC>=2)func_8002B12C(160,384,16,1,ovl663_D_80116898,0,ovl663_D_801168CC);
 func_8003EC88(0);func_8002A344(0x10058);
}
