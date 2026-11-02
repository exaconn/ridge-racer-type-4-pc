extern int func_8003EC88();
extern char *D_800F4D80;
void func_8007D448(int a0, int a1) {
    int i = 0;
    unsigned char *p, *q;
    int v;
    do {
        if (a1 == 1) {
            p = (unsigned char *)(i * 0x24 + (int)D_800F4D80 + 0x558);
            if (p[0x14] == 5) { p[0x14] = 0; goto tail; }
        }
        if (a1 == 0) {
            q = (unsigned char *)(i * 0x24 + (int)D_800F4D80 + 0x558);
            v = q[0x14];
            if (v == 2) { q[0x14] = 3; goto tail; }
            if (v != 0) goto tail;
            return;
        }
tail:
        func_8003EC88(i + 0x26);
        if (i == a0) *(char *)(i * 0x24 + (int)D_800F4D80 + 0x564) = 2;
        else *(char *)(i * 0x24 + (int)D_800F4D80 + 0x564) = 0;
        i++;
    } while (i < 2);
}
