/* CC1_FLAGS: -mno-split-addresses */
extern int _spu_FsetDelayW(), _spu_FsetDelayR();
struct H { char p[0x1A6]; volatile unsigned short f1A6; char q[2]; volatile unsigned short f1AA; };
extern struct H *_spu_RXX;
extern int *DMA4_MADR, *DMA4_BCR, *DMA4_CHCR;
extern unsigned short _spu_tsa;
extern int _spu_mem_mode_plus, _spu_dmaIsRead, SpuDmaStartAddr, _spu_dma_block_count;
int _spu_t(int a0, ...) {
    unsigned *s0;
    unsigned v1;
    unsigned short x;
    /* ASM_BARRIER: Keep control value in $a2 across case 3 setup */
    register int val asm("$6");
    /* ASM_BARRIER: Keep temporary values in $a0 in case 2 and case 3 tail */
    register unsigned r_a0 asm("$4");
    s0 = (unsigned *)(&a0 + 1);
    switch (a0) {
    case 2:
        {
            /* ASM_BARRIER: Keep shifted transfer address in $v0 */
            register unsigned r_v0 asm("$2");
            r_a0 = *s0;
            r_v0 = r_a0 >> _spu_mem_mode_plus;
            _spu_tsa = r_v0;
            _spu_RXX->f1A6 = r_v0;
        }
        break;
    case 1:
        _spu_dmaIsRead = 0;
        v1 = 0;
        while (_spu_RXX->f1A6 != _spu_tsa) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        _spu_RXX->f1AA = (_spu_RXX->f1AA & 0xFFCF) | 0x20;
        break;
    case 0:
        _spu_dmaIsRead = 1;
        v1 = 0;
        while (_spu_RXX->f1A6 != _spu_tsa) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        _spu_RXX->f1AA |= 0x30;
        break;
    case 3:
        x = (_spu_dmaIsRead == 1) ? 0x30 : 0x20;
        v1 = 0;
        while ((_spu_RXX->f1AA & 0x30) != x) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        if (_spu_dmaIsRead == 1) {
            s0++;
            _spu_FsetDelayR();
            val = 0x01000000;
        } else {
            s0++;
            _spu_FsetDelayW();
            val = 0x01000000;
        }
        {
            /* ASM_BARRIER: Keep block count in $v1 */
            register unsigned r_v1 asm("$3");
            r_a0 = s0[-1];
            SpuDmaStartAddr = r_a0;
            r_a0 = s0[0];
            r_v1 = r_a0 >> 6;
            /* ASM_BARRIER: Equalize dependence depth so srl schedules before andi */
            __asm__("" : "=r"(r_v1) : "0"(r_v1));
            r_v1 += (r_a0 & 0x3F) != 0;
            _spu_dma_block_count = r_v1;
            r_a0 = SpuDmaStartAddr;
            *DMA4_MADR = r_a0;
        }
        *DMA4_BCR = (_spu_dma_block_count << 16) | 0x10;
        /* ASM_BARRIER: Keep low half of 0x01000201 after *DMA4_BCR store so it fills bne delay slot */
        __asm__ volatile("" : "=r"(val) : "0"(val));
        val |= 0x201;
        if (_spu_dmaIsRead == 1) val = 0x01000200;
        *DMA4_CHCR = val;
        break;
    }
    return 0;
}
