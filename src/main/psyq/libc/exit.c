/* BIOS exit (B(38h)): terminate the host process. */
#include <stdlib.h>
#include "psyq/libc/exit.h"

void PsyqExit(s32 status) { exit(status); }
