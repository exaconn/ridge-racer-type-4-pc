/* BIOS sprintf (A(5Bh)) as the host C library formatter. The game only uses
 * %s, %d and width/zero-padded integer conversions, which behave the same. */
#include <stdarg.h>
#include <stdio.h>
#include "psyq/libc/printf.h"

s32 PsyqSprintf(s8 *destination, const s8 *format, ...) {
    va_list arguments;
    int length;
    va_start(arguments, format);
    length = vsprintf((char *)destination, (const char *)format, arguments);
    va_end(arguments);
    return length;
}
