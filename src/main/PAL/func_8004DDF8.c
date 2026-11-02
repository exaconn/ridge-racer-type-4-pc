extern int D_800A0220;
extern int func_8004DB94();
extern int func_8004DCCC();

int func_8004DDF8(a0, a1, a2)
int a0, a1, a2;
{
    switch (D_800A0220) {
    case 0:
        if (func_8004DB94(a0, 0) == 0) return 0;
        if (a1 >= 0) D_800A0220 = 1;
        else D_800A0220 = 2;
        break;
    case 1:
        if (func_8004DB94(a1, 1)) {
            D_800A0220 = 0;
            return 1;
        }
        break;
    case 2:
        if (func_8004DCCC(a2)) {
            D_800A0220 = 0;
            return 1;
        }
    }
    return 0;
}
