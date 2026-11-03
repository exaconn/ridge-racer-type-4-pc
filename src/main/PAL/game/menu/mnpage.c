extern void func_8001ECE8();
extern void func_8003D228();
extern void func_8003DC9C();
extern void func_8003E040();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern void func_80053BB4();
extern short D_800AC5D0;
extern int D_800AC5DC;
extern short D_800AD50A;
extern short D_800AD520;
extern int D_800F2DE4;
extern short D_800F3826;
typedef struct { short f0, f2; } U;
extern U *D_800F3AFC;
extern char *D_800F4D8C;
extern char D_800F4D98;
typedef struct { char pad[4]; char f4; char pad2[2]; char f7; } T;
extern T *D_800FF68C;

void func_80041434(void) {
    int i;
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    for (i = 0; i < 4; i++) {
        func_8003E040(i, -2, 0, 0);
    }
    D_800AD520 = 2;
    D_800F4D98 = 2;
    D_800F3826 = 0;
    func_80053BB4(0);
    func_8003DC9C(*(unsigned char *)(D_800F4D8C + 7), *(short *)(D_800F4D8C + 8), *(short *)(D_800F4D8C + 0xA), *(short *)(D_800F4D8C + 0xC), *(short *)(D_800F4D8C + 0xE), 0, 0);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    func_8003E928(0, 0xC, 0x48, 0xF0, 0x20, 0, 0, 4, 2, 0);
    D_800F3AFC->f2 = 1;
    D_800FF68C->f4 = 1;
    D_800FF68C->f7 = 0;
    D_800F2DE4 = 0;
    D_800AD50A = 0x108;
    D_800AC5D0 = 0;
}
