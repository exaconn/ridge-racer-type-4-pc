#ifndef RR_LAYOUT_H
#define RR_LAYOUT_H

#include "types.h"

/* Byte offsets supported by the original GCC and native fixture compilers. */
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 4)
#define RR_FIELD_OFFSET(type, field) __builtin_offsetof(type, field)
#else
#define RR_FIELD_OFFSET(type, field) ((unsigned long)&((type *)0)->field)
#endif

/* Compile-time layout checks supported by the original GCC toolchains. */
#define RR_ASSERT_SIZE(type, size) \
    typedef char rr_size_##type[(sizeof(type) == (size)) ? 1 : -1]
#define RR_ASSERT_OFFSET(type, field, offset) \
    typedef char rr_offset_##type##_##field[ \
        (RR_FIELD_OFFSET(type, field) == (offset)) ? 1 : -1]

#endif
