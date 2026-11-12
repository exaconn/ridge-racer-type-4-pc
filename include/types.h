#ifndef TYPES_H
#define TYPES_H
#include <stddef.h>
#include <stdint.h>
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/* Compiler portability: GCC/Clang attributes and their MSVC meaning. */
#if defined(__GNUC__) || defined(__clang__)
#define RR_PACKED __attribute__((packed))
#define RR_MAY_ALIAS __attribute__((__may_alias__))
#define RR_UNUSED __attribute__((unused))
#else
/* The packed records have no padding, so packing only drops their alignment
 * (for unaligned wire data), which x86-64 and ARM64 accesses do not need;
 * MSVC does no type-based alias analysis and warns about no unused static. */
#define RR_PACKED
#define RR_MAY_ALIAS
#define RR_UNUSED
#endif
#if defined(_MSC_VER) && !defined(__clang__)
#include <stdlib.h>
#include <string.h>
#define __builtin_abs abs
#define __builtin_memcpy memcpy
#endif
#endif
