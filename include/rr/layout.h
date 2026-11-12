#ifndef RR_LAYOUT_H
#define RR_LAYOUT_H

#include "types.h"

/* Byte offsets supported by the original GCC and native fixture compilers. */
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 4)
#define RR_FIELD_OFFSET(type, field) __builtin_offsetof(type, field)
#else
#include <stddef.h>
#define RR_FIELD_OFFSET(type, field) offsetof(type, field)
#endif

/* Original 32-bit layout checks. They describe the PS1 ABI (4-byte pointers)
 * and are enforced by 32-bit layout-check builds; with 64-bit host pointers
 * runtime structures holding pointers legitimately grow, so the host build
 * does not apply them. Serialized disc/card formats contain no host pointers
 * and keep the same layout on both. */
#if __SIZEOF_POINTER__ == 4
#define RR_ASSERT_SIZE(type, size) \
    typedef char rr_size_##type[(sizeof(type) == (size)) ? 1 : -1]
#define RR_ASSERT_OFFSET(type, field, offset) \
    typedef char rr_offset_##type##_##field[ \
        (RR_FIELD_OFFSET(type, field) == (offset)) ? 1 : -1]
#else
#define RR_ASSERT_SIZE(type, size) struct rr_layout_unchecked
#define RR_ASSERT_OFFSET(type, field, offset) struct rr_layout_unchecked
#endif

#endif
