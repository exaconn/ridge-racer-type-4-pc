/* GCC_VERSION: 2.7.2 */
extern int func_800940E0(), func_80094100(), func_80094198(), func_80094230(), func_8009424C();
extern short D_800ABAC4, D_800ABAC6;
struct S { short x, y; unsigned short w, h; short ox, oy; unsigned char pad[2]; unsigned short t; unsigned char c16, c17, f18, r, g, b; };
void func_80093E70(a0, a1) int *a0; struct S *a1; {
    unsigned short sp10, sp12, sp14, sp16;
    int t0;
    unsigned short v;
    unsigned short ta;
    a0[1] = func_80094100(a1->x, a1->y);
    a0[2] = func_80094198((short)(a1->w + (unsigned short)a1->x - 1), (short)((unsigned short)a1->y + a1->h - 1));
    a0[3] = func_80094230(a1->ox, a1->oy);
    a0[4] = func_800940E0(a1->c17, a1->c16, a1->t);
    a0[5] = func_8009424C((char *)a1 + 0xC);
    a0[6] = 0xE6000000;
    t0 = 7;
    if (a1->f18 != 0) {
        sp10 = a1->x;
        sp12 = a1->y;
        ta = a1->w;
        sp14 = ta;
        sp16 = a1->h;
        v = 0;
        if ((short)ta >= 0) {
            v = D_800ABAC4 - 1;
            if (D_800ABAC4 - 1 >= (short)ta) v = ta;
        }
        sp14 = v;
        if ((short)sp16 >= 0) {
            v = D_800ABAC6 - 1;
            if (D_800ABAC6 - 1 >= (short)sp16) v = sp16;
        } else v = 0;
        sp16 = v;
        if ((sp10 & 0x3F) == 0 && (sp14 & 0x3F) == 0) {
            t0 = 10;
            a0[7] = (a1->b << 16) | ((a1->g << 8) | 0x02000000) | a1->r;
            a0[8] = sp10;
            a0[9] = sp14;
        } else {
            t0 = 10;
            sp10 -= a1->ox;
            sp12 -= a1->oy;
            a0[7] = (a1->b << 16) | ((a1->g << 8) | 0x60000000) | a1->r;
            a0[8] = sp10;
            a0[9] = sp14;
        }
    }
    *((char *)a0 + 3) = t0 - 1;
}
