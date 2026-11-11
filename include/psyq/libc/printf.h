#ifndef RR_PSYQ_LIBC_PRINTF_H
#define RR_PSYQ_LIBC_PRINTF_H

#include "types.h"

/* Character bytes match the formatter implementation, including its real
 * variadic ABI; callers keep complete native string pointers. */
s32 PsyqSprintf(s8 *destination, const s8 *format, ...);

#endif /* RR_PSYQ_LIBC_PRINTF_H */
