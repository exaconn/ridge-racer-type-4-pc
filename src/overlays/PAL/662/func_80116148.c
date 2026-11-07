#include "rr/menu_arenas.h"
void ovl662_func_80116148(void){
 ResetScreenCounters();
 D_800F4D70.second=CdDirectoryBuffer+0x2D000;
 InitMenuArenas();
 LoadCursor=D_800F4D70.second;
 SetMenuFrameOptions(0,1,1);
 ovl662_func_80116020();
}
