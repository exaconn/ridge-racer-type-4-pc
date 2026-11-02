extern void func_800528A8();
extern int func_80096464();
extern short D_800AD520;
extern short D_800F3914;
extern short D_800F4D44;
void func_800590DC(void) {
 int r, a1; if (D_800F4D44 == 1) { if (D_800F3914 == 7) { a1 = D_800AD520 + 0x20; func_800528A8(0, a1); return; } } r = func_80096464(); a1 = r % 6 + 0x1A; func_800528A8(0, a1);
}
