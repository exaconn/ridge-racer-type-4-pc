#include "rr/overlay662_progress_init.h"
#include "rr/load_cursor.h"

void ovl662_init_progress_menu(void)
{
    s32 i,j,k;
    ModeFrame=0;
    ScreenFrame=0;
    MenuCursor=0;
    MenuItemCount=3;
    ResetScreenCounters();
    MenuArenas.second=CdDirectoryBuffer+0x2D000;
    InitMenuArenas();
    LoadCursor=MenuArenas.second;
    InitUiRow(0,118,176,68,32,0,0,1,0,54);
    InitUiRow(1,118,216,68,32,0,0,1,1,15);
    InitUiRow(2,118,256,68,32,0,0,1,0,85);
    SetupRenderer(5);
    ovl662_total_unlocked_cars=0;
    LoadCursor=MenuArenaBase;
    MenuArenas.first=MenuArenaBase;
    for(i=0;i<4;i++) {
        ovl662_team_unlocked_counts[i]=0;
        for(j=0;j<4;j++) {
            ovl662_team_maker_car_counts[i][j]=0;
            for(k=0;k<20;k++)
                ovl662_team_maker_car_counts[i][j]+=(SaveProgress.prefix.progress.progress10[i][j]>>k)&1;
            ovl662_team_unlocked_counts[i]+=ovl662_team_maker_car_counts[i][j];
        }
        ovl662_total_unlocked_cars+=ovl662_team_unlocked_counts[i];
    }
}
