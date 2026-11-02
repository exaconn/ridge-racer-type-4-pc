extern short func_80090424();
extern short func_800904F4();

void func_8006E030(short *a0, int a1) {
    short s = func_80090424(a1);
    short c = func_800904F4(a1);
    a0[1] = -s;
    a0[0] = c;
    a0[4] = c;
    a0[2] = 0;
    a0[3] = s;
    a0[5] = 0;
    a0[6] = 0;
    a0[7] = 0;
    a0[8] = 0x1000;
}
