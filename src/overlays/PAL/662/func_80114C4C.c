#include "rr/overlay662_progress_controller.h"
#include "rr/course_choice_types.h"

extern RrCourseChoice4 D_800ABE08[2][9];
extern s16 D_800AC64A;
extern u8 D_800F4368[];
void func_80014A8C(s32 mode);
void func_8002B330(s32 mode, s32 width, char *selection, s32 limit, s32 flags);
void func_80053AF4(s32, s32);
void func_80053CB0(void);
void func_8006F5F0(void *);
void func_8001E854(void *, s32, s32);

void ovl662_func_80114C4C(void)
{
    s32 i;
    s32 state;
    s32 unused[2]; /* never referenced; original frame reserves 8 local bytes */

    D_800F2DE4++;
    switch ((u16)D_800F4D8A) {
    case 0:
        state = D_800F4D80->byte14;
        if (state != 2) {
            break;
        }
        func_8002AF8C(1, 1, 1);
        D_800F4D80[D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, 3, 0);
        D_800F4D80[D_800AC64A].cells[0] = state;
        if (D_800F3A38[0].a) {
            func_8002AF8C(0, 1, 1);
            D_800F4D80[D_800AC64A].cells[0] = 1;
            if (D_800AC64A == 0) {
                if (D_800ABE08[1][0].glyph != -1) {
                    func_80014A8C(-1);
                    func_8004F24C(1);
                    D_800F4D8A = 3;
                    D_800AC5D0 = 64;
                    for (i = 0; i < 3; i++) {
                        if (D_800F4D80[i].byte14 == 2) {
                            D_800F4D80[i].byte14 = 7;
                        }
                    }
                } else {
                    func_8004F24C(4);
                }
            } else if (D_800AC64A == 1) {
                func_8004F24C(1);
                func_80014A8C(-1);
                D_800F4D8A = 4;
                for (i = 0; i < 3; i++) {
                    if (D_800F4D80[i].byte14 == 2) {
                        D_800F4D80[i].byte14 = 7;
                    }
                }
                D_800AC5D0 = 64;
            } else if (D_800AC64A == state) {
                func_8004F24C(1);
                func_80014A8C(-1);
                D_800F4D8A = 8;
                for (i = 0; i < 3; i++) {
                    if (D_800F4D80[i].byte14 == 2) {
                        D_800F4D80[i].byte14 = 3;
                    }
                }
                D_800AC5D0 = 64;
                func_80053AF4(0, 60);
            }
        } else if (D_800F3A38[0].b) {
            func_8004F24C(2);
            func_80014A8C(-1);
            D_800F4D8A = 8;
            func_8002AF8C(0, 1, 1);
            for (i = 0; i < 3; i++) {
                if (D_800F4D80[i].byte14 == 2) {
                    D_800F4D80[i].byte14 = 3;
                }
            }
            D_800AC5D0 = 64;
            func_80053AF4(0, 60);
        }
        break;
    case 3:
        D_800AC5D0--;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,
                      (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 63;
            D_800F4D8A = 0;
        }
        break;
    case 4:
        D_800AC5D0--;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,
                      (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            func_8006F5F0(D_800F4368);
            func_8001E854(D_800F4368, 0, 0);
            D_800F4C6A = 3;
            D_800F3A26 = 1;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        D_800AC5D0--;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,
                      (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
            func_80053CB0();
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        func_8003EC88(i);
    }
    func_8002A344(0x10043);
}
