#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F3920 (*(char (*)[])&D_800F3920)

#undef D_800F3920

extern s16 D_800F4D2C;

extern s16 D_800FFC18;

void func_8002F5BC(void){
 switch(D_800F4D2C){
 case 0:D_800AD50A=0x105;break;
 case 1:default:D_800AD50A=0x131;break;
 case 2:D_800AD50A=0x137;break;
 case 3:D_800AD50A=0x127;break;
 case 6:D_800AD50A=0x201;break;
 case 4:D_800AD50A=0x149;break;
 case 5:D_800FFC18=0;D_800AD50A=0x147;break;
 case 7:D_800AD50A=0x14E;break;
 case 8:D_800AD50A=0x13B;break;
 }
}
