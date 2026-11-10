#ifndef RR_OVERLAY667_STATUS_H
#define RR_OVERLAY667_STATUS_H
#include "rr/menu_transition.h"
extern s32 ovl667_sio_status_flag;
/* Only kind2 rejects elapsed values25000..UINT32_MAX. */
s32 ovl667_check_sio_timeout(s32 kind,u32 elapsed);
s32 ovl667_get_sio_status_flag(void);
/* Publishes scene marker315 through the canonical unsigned16 cell. */
void ovl667_select_scene(void);
#endif
