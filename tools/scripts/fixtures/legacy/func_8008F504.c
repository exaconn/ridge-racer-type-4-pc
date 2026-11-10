/* GCC_VERSION: 2.7.2 */
extern int StRingSize, StRingReadIndex, StRingFrameStartIndex, StRingWriteIndex, StStreamStarted, CdMediaChangeCount, StCurFrameNo;
extern short StCurSectorOffset;
extern void init_ring_status();
void StClearRing(void) {
    int a = StRingSize;
    StRingReadIndex = 0;
    StRingFrameStartIndex = 0;
    StRingWriteIndex = 0;
    StStreamStarted = 0;
    init_ring_status(0, a);
    CdMediaChangeCount = 0;
    StCurSectorOffset = 0;
    StCurFrameNo = 0;
}
