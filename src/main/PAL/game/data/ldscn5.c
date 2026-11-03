/* RODATA 0x8001085c 0x18 */
#include "game/data/cdfile2.h"
#include "game/data/ldscn4.h"
#include "game/data/ldscn5.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "rr/course_choices.h"
extern void func_800436E8();
extern unsigned char D_800F4D7C;

int func_8003BEA8(int a) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (func_8001292C(func_8001223C(0, a + 1), D_800ACC18) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58() != 0) {
            func_800436E8(D_800ACC18);
    case 3:
            if (func_8001292C(func_8001223C(0, a + 10), D_800ACC18) != 0) {
                D_800F4D7C = 4;
            }
        }
        break;
    case 4:
        if (func_80012A58() != 0) {
            D_800AC64C = D_800ACC18;
            func_80012A68(D_800ACC18);
            D_800F4D7C = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}
