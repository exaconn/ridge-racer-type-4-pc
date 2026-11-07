/* GCC_VERSION: 2.7.2 */
extern int D_80110314, D_801102FC, D_801102F8, StRingWriteIndex, D_801102EC, D_801102DC, StCurFrameNo;
extern short D_801102D4;
extern void init_ring_status();
void StClearRing(void) {
    int a = D_80110314;
    D_801102FC = 0;
    D_801102F8 = 0;
    StRingWriteIndex = 0;
    D_801102EC = 0;
    init_ring_status(0, a);
    D_801102DC = 0;
    D_801102D4 = 0;
    StCurFrameNo = 0;
}
