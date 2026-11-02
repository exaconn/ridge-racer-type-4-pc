typedef struct { unsigned char *c0; int c4, c8, cC; char p10[4]; unsigned char b14; char p15[2]; unsigned char b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f, b20, b21; char p22[4]; signed char b26; unsigned char b27[0x10]; unsigned char b37[0x10]; char p47[1]; short h48[8]; short h58[4]; unsigned short h60[0x10]; short h80; char p82[6]; int w88; int w8c; int w90; char p94[4]; int w98; int w9c; char pa0[0x10]; } Ch;
extern Ch *D_8010F068[];
extern int func_80089D28();
void func_80082204(int a0, short a1) {
    int t = a0 << 16;
    Ch **b = D_8010F068;
    Ch **q = (Ch **)((int)b + (t >> 14));
    int o = a1 * 0xB0;
    Ch *p = (Ch *)((char *)*q + o);
    func_80089D28((short)(a0 | (a1 << 8)), (char *)p + 0x5C, (char *)p + 0x5E);
    ((Ch *)(o + (int)*q))->w98 &= ~1;
    ((Ch *)(o + (int)*q))->w98 &= ~8;
    ((Ch *)(o + (int)*q))->w98 |= 2;
}
