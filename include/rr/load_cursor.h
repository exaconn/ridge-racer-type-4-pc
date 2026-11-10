#ifndef RR_LOAD_CURSOR_H
#define RR_LOAD_CURSOR_H

/* Current end of the shared loading/work arena. Consumers select a real byte,
 * resource or workspace view; this root is always a complete C pointer. */
extern void *LoadCursor;

#endif
