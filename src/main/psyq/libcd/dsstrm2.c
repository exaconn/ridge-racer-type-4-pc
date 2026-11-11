#include "psyq/libcd/dsstrm23.h"
#include "psyq/libcd/dsmedia2.h"
#include "psyq/libcd/dsstrm2.h"
#include "psyq/libcd/dsstrm32.h"
extern int StDataReadyFunc;
extern int StRingFullFunc;
void StSetStream(int a, int b, int c, int d, int e) {
    StSetMask(1);
    StRingBase = 0;
    StDataReadyFunc = d;
    D_801102D8 = a & 1;
    D_801102E8 = 0;
    D_801102E0 = 0;
    StCurSectorOffset = 0;
    StCurFrameNo = 0;
    StRingFullFunc = e;
}
