#include "psyq/libspu/smalloc2.h"
#include "rr/library.h"
void SsSetMarkCallback(short a0, short a1, int a2) { _SsMarkCallback[a0][a1] = a2; }
