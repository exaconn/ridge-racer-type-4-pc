#ifndef RR_PSYQ_SDK_TYPES_H
#define RR_PSYQ_SDK_TYPES_H

/* Integer types of the PsyZ (__psyz) SDK declarations that the game headers
 * mirror. PsyZ u_long is pointer width: unsigned long, or unsigned long long
 * on Win64. Parameters PsyZ declares as long stay long. */
#if defined(_WIN64)
typedef unsigned long long PsyqULong;
#else
typedef unsigned long PsyqULong;
#endif

#endif
