#include "rr/overlay_menu.h"
s32 ovl662_func_8011662C(u32 unused,s32 mode){
 s32 selected;
 if(mode)selected=D_800F4D99;else selected=D_800F4D98.bytes[0];
 func_8002B12C(160,204,48,0,selected,0,4);
 return func_8003E13C(selected);
}
