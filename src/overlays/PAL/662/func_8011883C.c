#include "rr/overlay662_time.h"
u8 *ovl662_format_record_time(s32 value){
 if(value>=0){
  s32 seconds=value/3000;
  s32 minutes=seconds/60;
  seconds%=60;
  ovl662_record_time_str_buf[0]=minutes/10+'0';
  ovl662_record_time_str_buf[1]=minutes%10+'0';
  ovl662_record_time_str_buf[3]=seconds/10+'0';
  ovl662_record_time_str_buf[4]=seconds%10+'0';
 }else{
  ovl662_record_time_str_buf[0]='-';ovl662_record_time_str_buf[1]='-';
  ovl662_record_time_str_buf[3]='-';ovl662_record_time_str_buf[4]='-';
 }
 return ovl662_record_time_str_buf;
}
