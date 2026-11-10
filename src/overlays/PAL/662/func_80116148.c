#include "rr/menu_arenas.h"
#include "rr/load_cursor.h"
void ovl662_setup_history_arenas(void){
 ResetScreenCounters();
 MenuArenas.second=CdDirectoryBuffer+0x2D000;
 InitMenuArenas();
 LoadCursor=MenuArenas.second;
 SetMenuFrameOptions(0,1,1);
 ovl662_init_history_overview();
}
