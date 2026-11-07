#ifndef RR_STRING_TAIL_COPY_H
#define RR_STRING_TAIL_COPY_H
/* Copy source bytes before its NUL, then clear the existing destination tail
 * until a destination NUL. Both scans require accessible terminating storage;
 * copying may overwrite an earlier destination terminator. */
void CopyStringClearTail(char *destination,char *source);
#endif
