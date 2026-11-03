#include "rr/diagnostics.h"
#include "rr/course_choices.h"
#include "rr/save_progress.h"
#include "rr/frame_environment.h"
extern s16 ovl661_D_80118418[2],ovl661_D_8011844C[2],ovl661_D_80118450[2];
extern s16 D_800ABEC8,D_800AC59C,D_800AC64A,D_800AC5E0;
extern s32 D_800F3A30,ovl661_D_80118414;
extern RrCourseChoice4 ovl661_D_80118458[2][2][9];
extern char ovl661_D_80118420[24],ovl661_D_8011487C[];
extern const char ovl661_D_80114888[];
extern u16 D_800F4C6A;
extern s16 D_800F3A26,D_800F4D8A;
void SetDispMask(s32 mode);
void func_8002A604(void);
void func_8002B75C(char *destination,char *source);
void ovl661_func_80115598(s32 mode);
void func_80053BB4(s32 mode);
void func_8002AF8C(s32,s32,s32);
/* Both destination name slots contain a NUL within12 bytes; the saved name
 * is terminated within its first12 bytes. String-tail cleanup keeps that bound. */
void ovl661_func_80115DB8(void)
{
    s32 i;
    s32 j;

    SetDispMask(0);
    func_8002A604();
    for (i = 0; i < 2; i++) {
        ovl661_D_80118450[i] = 0;
        ovl661_D_8011844C[i] = 0;
        ovl661_D_80118418[i] = 0;
    }
    D_800ABEC8 = 0;
    D_800AC648 = 0;
    D_800F3914 = 0;
    D_800F3A30 = 3;
    D_800AC59C = 2;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 9; j++) {
            ovl661_D_80118458[0][i][j] = D_800ABE08[i][j];
            ovl661_D_80118458[1][i][j] = D_800ABE08[0][j];
        }
    }
    func_8002B75C(ovl661_D_80118420, (char *)D_800F3920.prefix.progress.prefix00.bytes);
    func_8002B75C(ovl661_D_80118420 + 12, ovl661_D_8011487C);
    func_8001ECE8(0, 0, 0, 2);
    ovl661_func_80115598(0);
    func_80037040((s8 *)&D_800F4D98, D_800ABE08[0][0].glyph);
    func_80037040((s8 *)&D_800F4D98 + 4, D_800ABE08[0][0].glyph);
    func_80053BB4(0);
    ovl661_D_80118414 = 8;
    D_800F4C6A = 1;
    D_800F3A26 = 56;
    D_800F4D8A = 0;
    func_8001E84C(ovl661_D_80114888);
    func_8002AF8C(0, 1, 1);
}
