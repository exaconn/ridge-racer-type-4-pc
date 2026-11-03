/* CC1_FLAGS: -mno-split-addresses */
extern int D_800A67D8;
extern int func_8008C118();
void func_8008E7F4() { int *p = &D_800A67D8; int a = *p; p -= 10; func_8008C118(a); p[9] = 0; }
