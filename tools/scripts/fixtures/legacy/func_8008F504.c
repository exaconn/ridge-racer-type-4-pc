/* GCC_VERSION: 2.7.2 */
extern int StRingSize, StRingReadIndex, StRingFrameStartIndex, StRingWriteIndex, StStreamStarted, D_801102DC, StCurFrameNo;
extern short StCurSectorOffset;
extern void init_ring_status();
void StClearRing(void) {
    int a = StRingSize;
    StRingReadIndex = 0;
    StRingFrameStartIndex = 0;
    StRingWriteIndex = 0;
    StStreamStarted = 0;
    init_ring_status(0, a);
    D_801102DC = 0;
    StCurSectorOffset = 0;
    StCurFrameNo = 0;
}
