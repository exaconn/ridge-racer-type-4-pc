struct A { char pad[0xF0]; int idx; char p2[0xE0]; short ang; };
struct B { char pad[0x12]; short x; char p2[0x28]; };
extern struct B *D_800ABEA4;
int func_80022848(struct A *a0) {
    int t = 0x800 - D_800ABEA4[a0->idx].x;
    return (unsigned)(((a0->ang - t) & 0xFFF) - 0x401) < 0x7FF;
}
