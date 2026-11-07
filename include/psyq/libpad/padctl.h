#ifndef RR_PSYQ_LIBPAD_PADCTL_H
#define RR_PSYQ_LIBPAD_PADCTL_H

#include "types.h"

void PadStartCom();
int PadGetState();
int PadInfoMode();
int PadInfoAct();
void PadSetActAlign();
void PadSetAct();

#endif /* RR_PSYQ_LIBPAD_PADCTL_H */
