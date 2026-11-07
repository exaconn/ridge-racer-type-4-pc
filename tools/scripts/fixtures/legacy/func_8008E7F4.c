/* CC1_FLAGS: -mno-split-addresses */
extern int CdReadSavedSyncCallback;
extern int CdSyncCallback();
void cd_read_restore_sync() { int *p = &CdReadSavedSyncCallback; int a = *p; p -= 10; CdSyncCallback(a); p[9] = 0; }
