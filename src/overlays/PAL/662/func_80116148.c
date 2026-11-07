#include "rr/menu_arenas.h"
void ovl662_func_80116148(void){
 ResetScreenCounters();
 MenuArenas.second=CdDirectoryBuffer+0x2D000;
 InitMenuArenas();
 LoadCursor=MenuArenas.second;
 SetMenuFrameOptions(0,1,1);
 ovl662_func_80116020();
}
