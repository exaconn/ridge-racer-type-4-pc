/* RODATA 0x800100e4 0x20 */
extern int *func_80017F84();
extern int *func_800183EC();
extern int *func_800189AC();
extern int *func_80018D44();
extern int *func_80019144();

void func_800194BC(char *a0, int a1)
{
    int *s = (int *)0x1F80007C;
    int w, v;
    int *p;
    a0 += a1 * 12;
    v = *(int *)(a0 + 4);
    s[0x3F] = 0x20180000;
    s[0x3E] = 0x60000;
    s[0x1C] = v;
    p = *(int **)(a0 + 0xC);
    w = *p++;
    while (w) {
        s[0x39] = w >> 16;
        switch (w & 0xFFFF) {
        case 9:
            p = func_80017F84(p, s);
            break;
        case 10:
            p = func_800183EC(p, s);
            break;
        case 13:
            p = func_800189AC(p, s);
            break;
        case 15:
            p = func_80018D44(p, s);
            break;
        case 16:
            p = func_80019144(p, s);
            break;
        }
        w = *p++;
    }
}
