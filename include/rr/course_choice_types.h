#ifndef RR_COURSE_CHOICE_TYPES_H
#define RR_COURSE_CHOICE_TYPES_H
#include "types.h"
/* Nine entries per course group; signed coordinates are stored in byte order. */
typedef struct {s16 glyph;s8 second,first;} CourseChoice4;
#endif
