/* CC1_FLAGS: -mno-split-addresses */
extern int D_800A6390;
extern void (*D_800A6370[8])();
void func_8008BA7C() {
    int i = 0;
    void (**p)() = D_800A6370;
    D_800A6390++;
    do {
        if (*p) (*p)();
        i++;
        p++;
    } while (i < 8);
}
