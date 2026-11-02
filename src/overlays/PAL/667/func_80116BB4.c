#include "rr/course_choices.h"
extern RrCourseChoice4 D_8010BFD8[2][2][9];
extern s16 D_8010C0D8[2],D_8010C0D0[2];
void ovl667_func_80116BB4(void) {
    func_8003A198();
    D_800ACC18 = D_800F4D70.first;
    func_80037040(
        (s8 *)&D_800F4D98 + D_800AC5D4 * 4,
        D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].glyph
    );
    D_800ACBD0[D_800AC5D4].first = D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].first;
    D_800ACBD0[D_800AC5D4].second = D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].second;
    D_800F2AB0[1] = 6;
}
