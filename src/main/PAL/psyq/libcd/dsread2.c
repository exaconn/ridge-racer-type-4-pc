/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libcd/s_1.h"
#include "rr/cd_read_request.h"
#include "psyq/libcd/dsread2.h"
void cd_read_restore_sync() { int *p = &CdReadSavedSyncCallback; int a = *p; p -= 10; CdSyncCallback(a); p[9] = 0; }
