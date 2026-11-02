#ifndef RR_TIME_RECORD_TYPES_H
#define RR_TIME_RECORD_TYPES_H
#include "rr/layout.h"
/* Five records per course, eight courses per mode; names terminate within10 bytes. */
typedef struct {char name[10];s16 car;s32 time;} RrTimeRecord16;
RR_ASSERT_SIZE(RrTimeRecord16,16);
RR_ASSERT_OFFSET(RrTimeRecord16,car,0x0A);
RR_ASSERT_OFFSET(RrTimeRecord16,time,0x0C);
#endif
