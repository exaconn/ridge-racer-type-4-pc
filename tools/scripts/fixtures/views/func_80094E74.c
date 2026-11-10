/* CC1_FLAGS: -mno-split-addresses */

/* MASPSX_FLAGS: --assembler-profile=gnu */

extern int DMACallback(int, int);

extern int SetIntrMask(int);

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

extern struct GpuSys GpuResetState;

extern int D_800ABACC;

extern volatile unsigned int *GPU_STATUS;

extern volatile unsigned int *DMA2_CHCR;

extern volatile int _qin;

extern volatile int _qout;

extern int GpuSavedIntrMask;

extern volatile struct GpuQueue GpuCommandQueue[];

int _exeque(void) {
    int m0, m1;
    if (*DMA2_CHCR & 0x01000000) {
        return 1;
    }
    GpuSavedIntrMask = SetIntrMask(0);
    while (_qin != _qout && (m0 = 0x01000000, (*DMA2_CHCR & m0) == 0)) {
        if (((_qout + 1) & 0x3F) == _qin && D_800ABACC == 0) {
            DMACallback(2, 0);
        }
        while ((m1 = 0x04000000, (*GPU_STATUS & m1) == 0))
;
        GpuCommandQueue[_qout].op(GpuCommandQueue[_qout].p0, GpuCommandQueue[_qout].p1);
        _qout = (_qout + 1) & 0x3F;
    }
    SetIntrMask(GpuSavedIntrMask);
    if (_qin == _qout && (*DMA2_CHCR & 0x01000000) == 0) {
        void (*fn)();
        if (GpuResetState.unk8 != 0 && (fn = GpuResetState.unkC) != 0) {
            struct GpuSys *sys = &GpuResetState;
            *(volatile int *)&sys->unk8 = 0;
            fn();
        }
    }
    return (_qin - _qout) & 0x3F;
}
