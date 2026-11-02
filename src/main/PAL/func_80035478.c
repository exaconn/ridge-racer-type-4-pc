#include "rr/player_status.h"

void func_80035478(s32 mode) {
    u8 text[4];
    s32 remaining,row,statusY,waitY,labelY;
    s8 status;
    unsigned long address;

    if (mode>=0) {
        /* Integer cursor retains native pointer width. Only the four in-owner
         * addresses are converted back to pointers; the final decrement is dead. */
        row=3;
        address=(unsigned long)&text[3];
        do {
            *(u8*)address=0;
            row--;
            address--;
        } while (row>=0);
        row=0;
        do {
            labelY=row*194+64;
            waitY=row*194+150;
            statusY=row*194+120;
            text[0]=row+((s32)((u32)(D_800AD508^1)<<16)>>15)+49;
            func_8001F40C(12,labelY,D_80010600);
            func_8001F40C(88,labelY,text);
            status=D_800ABEBC[1-mode][row];
            switch (status) {
            case 0:
                func_8001F40C(60,statusY,D_8001060C);
                break;
            case 1:
                func_8001F40C(60,statusY,D_80010628);
                func_8001F40C(60,waitY,D_8001063C);
                break;
            case -1:
                func_8001F40C(60,statusY,D_8001064C);
                break;
            }
            row++;
        } while (row<2);
        row=D_800AC5A4/50;
        text[0]=row/10+48;
        text[1]=row%10+48;
        func_8001F40C(44,244,text);
        func_8001F40C(32,244,D_80010658);
        remaining=(s32)(500U-(u32)D_800AC5A4);
        func_8001F40C(((s32)((u32)remaining<<8)/500)+32,224,D_80010674);
    }
}
