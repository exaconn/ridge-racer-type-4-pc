extern int func_8004B864();
extern int func_8004BAD4();
typedef struct { char pad[0x50]; short f; char pad2[0xA]; } S1;
typedef struct { char pad[0x1E4]; short f; char pad2[0x13A]; } S2;
extern S2 D_800ABEF8[];
extern S1 D_800F3A38[];
void func_800347BC(void) {
    int i;
    func_8004B864(0);
    func_8004B864(1);
    for (i = 0; i < 2; i++) {
        if (D_800ABEF8[i].f != 0) {
            D_800F3A38[i].f = 1;
            func_8004BAD4(i, 0, 0xF);
        }
    }
}
