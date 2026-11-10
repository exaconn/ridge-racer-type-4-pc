/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_decode.h"
LibInterruptCallback ovl668_mdec_out_callback(LibInterruptCallback callback){return DMACallback(1,callback);}
