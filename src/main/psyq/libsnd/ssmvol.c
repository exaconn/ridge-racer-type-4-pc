#include "psyq/libsnd/sstick3.h"
#include "psyq/libsnd/ssmvol.h"

void SsSepStop(short a, short b);

void SsSeqStop(short a) { _SsSndStop(a, 0); }

void SsSepStop(short a, short b) { _SsSndStop(a, b); }
