/* CC1_FLAGS: -mno-split-addresses */
/* MASPSX_FLAGS: --assembler-profile=gnu */
extern int func_8008B3C4(int, int);
extern int func_8008B4E0(int);

struct GpuSys {
    unsigned char pad0;
    unsigned char enabled;
    unsigned char pad2[6];
    int unk8;
    void (*unkC)();
};

struct GpuQueue {
    void (*op)();
    int p0;
    int p1;
    int buf[21];
};

extern struct GpuSys D_800ABAC0;
extern int D_800ABACC;
extern volatile unsigned int *D_800ABBC8;
extern volatile unsigned int *D_800ABBD4;
extern volatile int D_800ABBE8;
extern volatile int D_800ABBEC;
extern int D_800ABBF4;
extern volatile struct GpuQueue D_801128D8[];

int func_80094E74(void) {
    int m0, m1;
    if (*D_800ABBD4 & 0x01000000) {
        return 1;
    }
    D_800ABBF4 = func_8008B4E0(0);
    while (D_800ABBE8 != D_800ABBEC && (m0 = 0x01000000, (*D_800ABBD4 & m0) == 0)) {
        if (((D_800ABBEC + 1) & 0x3F) == D_800ABBE8 && D_800ABACC == 0) {
            func_8008B3C4(2, 0);
        }
        while ((m1 = 0x04000000, (*D_800ABBC8 & m1) == 0))
            ;
        D_801128D8[D_800ABBEC].op(D_801128D8[D_800ABBEC].p0, D_801128D8[D_800ABBEC].p1);
        D_800ABBEC = (D_800ABBEC + 1) & 0x3F;
    }
    func_8008B4E0(D_800ABBF4);
    if (D_800ABBE8 == D_800ABBEC && (*D_800ABBD4 & 0x01000000) == 0) {
        void (*fn)();
        if (D_800ABAC0.unk8 != 0 && (fn = D_800ABAC0.unkC) != 0) {
            struct GpuSys *sys = &D_800ABAC0;
            /* ASM_BARRIER: materialize base pointer from offset pointer before clearing unk8 */
            __asm__("" : "+r"(sys));
            *(volatile int *)&sys->unk8 = 0;
            fn();
        }
    }
    return (D_800ABBE8 - D_800ABBEC) & 0x3F;
}
