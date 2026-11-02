#include "rr/overlay665_dispatch.h"
void ovl665_func_80118EB8(void) {
 switch(ovl665_D_80119768.scene00) {
 case 0:
  func_80092CB8(0);ovl665_func_80114A24();
  ovl665_D_80119768.scene00=(s32)((u32)ovl665_D_80119768.scene00+1U);
  func_8005AF5C();func_8005B2D4(1);func_80053BB4(0);break;
 case 1:ovl665_func_80114C4C();break;
 case 2:
  switch(ovl665_D_80119768.selection08) {
  case 0:ovl665_func_80116D10(0);break;
  case 1:ovl665_func_80116D10(1);break;
  case 2:ovl665_func_801151A4();break;
  case 3:ovl665_func_80115820();break;
  case 4:ovl665_func_80115CFC();break;
  case 5:func_8005D974(1);break;
  case 6:D_800F4C6A=1;D_800F3A26=3;D_800F4D8A=0;break;
  }
  break;
 }
 func_8005B304();
}
