#ifndef RR_PSYQ_LIBC_EXIT_H
#define RR_PSYQ_LIBC_EXIT_H
#include "types.h"
/* BIOS B(38h) terminates execution; no return path exists. */
_Noreturn void PsyqExit(s32 status);
#endif
