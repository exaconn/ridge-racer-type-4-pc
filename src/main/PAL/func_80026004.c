struct E { char pad[0x1C]; short w; char pad2[0x3C - 0x1E]; };
extern int *D_800ACB8C;
extern short D_800AD522;
extern int D_800ABECC;
extern struct E *D_800ABEA4;
void func_80026004(char *a, int f)
{
    int i;
    int t;
    int s;
    int m;
    int j;
    *(int *)(a + 0x184) = 0;
    i = *D_800ACB8C;
    t = *(int *)(a + 0xF0);
    s = 0;
    if (D_800AD522 == 0) {
        if (f == 0) {
            while (1) {
                m = i % D_800ABECC;
                s -= D_800ABEA4[m].w;
                if (t == m) break;
                i++;
            }
        } else {
            while (1) {
                i--;
                j = i;
                if (i < 0) j = D_800ABECC + i;
                i = j;
                if (t == i) break;
                s += D_800ABEA4[i].w;
            }
        }
    } else {
        if (f == 0) {
            do {
                i--;
                j = i;
                if (i < 0) j = D_800ABECC + i;
                i = j;
                s -= D_800ABEA4[i].w;
            } while (t != i);
        } else {
            while (1) {
                if (t == i % D_800ABECC) break;
                i++;
                s += D_800ABEA4[i % D_800ABECC].w;
            }
        }
    }
    *(int *)(a + 0x184) = s;
}
