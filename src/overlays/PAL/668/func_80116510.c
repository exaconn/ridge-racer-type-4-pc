/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_control.h"
LibInterruptCallback ovl668_mdec_in_callback(LibInterruptCallback callback){return DMACallback(0,callback);}
