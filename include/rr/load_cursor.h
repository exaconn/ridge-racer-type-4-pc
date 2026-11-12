#ifndef RR_LOAD_CURSOR_H
#define RR_LOAD_CURSOR_H

#include <stddef.h>
#include <stdint.h>

/* Current end of the shared loading/work arena. Consumers select a real byte,
 * resource or workspace view; this root is always a complete C pointer. */
extern void *LoadCursor;

/* The PS1 arena is word aligned; a host structure holding pointers placed in
 * it starts at its own alignment (RR_ARENA_ALIGN(LoadCursor, Type)). */
static inline void *RrArenaAlign(void *cursor, size_t align) {
    uintptr_t p = (uintptr_t)cursor;
    return (char *)cursor + ((align - p % align) % align);
}
#define RR_ARENA_ALIGN(cursor, type) RrArenaAlign((cursor), _Alignof(type))

#endif
