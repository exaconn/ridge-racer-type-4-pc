#ifndef RR_DIAGNOSTICS_H
#define RR_DIAGNOSTICS_H
#include "types.h"
/* PAL's disabled diagnostic entry returns1 without reading any arguments.
 * It remains in original assembly: stock GCC emits argument-home stores for
 * a C variadic definition, whereas the original entry is exactly8 bytes. */
s32 DebugPrintStub(const char *format,...);
#endif
