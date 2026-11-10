#ifndef RR_CD_DIRECTORY_BUFFER_H
#define RR_CD_DIRECTORY_BUFFER_H
#include "types.h"

/* Reusable loading allocation. Directory words and saved-card images are views
 * of its contents, not different types of the shared CPU pointer cell. */
extern u8 *CdDirectoryBuffer;
#endif
