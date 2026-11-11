#include "rr/relative_file.h"
void RelocateRelativeFile(RelativeHeader *input){
 RelativeHeader *base=input;
 RelativeSlot *cursor=(RelativeSlot*)(base+1);
 u32 count=base->count;
 u32 i;
 u8 *first,*second;
 u32 last;
 
 TrackRelFileHeaderPtr=base;
 RelFileSegmentCount=count;
 i=0;
 count*=8U;
 first=rr_relative_file_address(base,base->first[0].relative);
 second=rr_relative_file_address(base,base->first[1].relative);
 base->first[0].absolute=first;
 base->first[1].absolute=second;
 TrackRelSecondTablePtr=second;
 base->first[2].absolute=rr_relative_file_address(base,base->first[2].relative);
 RelFileViewRecords=(CourseViewRecord *)base->first[2].absolute;
 RelFileSection00=rr_relative_file_address(base,base->offsets[0]);
 RelFileSection01=rr_relative_file_address(base,base->offsets[1]);
 TrackRelSegment2Ptr=rr_relative_file_address(base,base->offsets[2]);
 RelFileOffsetPtr3=rr_relative_file_address(base,base->offsets[3]);
 TrackRelSegment4Ptr=rr_relative_file_address(base,base->offsets[4]);
 RelFileSection05=rr_relative_file_address(base,base->offsets[5]);
 RelFileOffsetPtr6=rr_relative_file_address(base,base->offsets[6]);
 RelFileOffsetPtr7=rr_relative_file_address(base,base->offsets[7]);
 TrackRelSegment8Ptr=rr_relative_file_address(base,base->offsets[8]);
 RelFileOffsetPtr9=rr_relative_file_address(base,base->offsets[9]);
 TrackCollisionGridPtr=rr_relative_file_address(base,base->offsets[10]);
 RelFileSection11=rr_relative_file_address(base,base->offsets[11]);
 RelFileOffsetPtr12=rr_relative_file_address(base,base->offsets[12]);
 RelFileOffsetPtr13=rr_relative_file_address(base,base->offsets[13]);
 TrackSectionBoundsPtr=rr_relative_file_address(base,base->offsets[14]);
 last=base->offsets[15];
 RelFileSegmentViewSlots=cursor;
 RelFileSection15=rr_relative_file_address(base,last);
 if(count!=0)do {
  cursor->absolute=rr_relative_file_address(base,cursor->relative);
  i++;
  cursor++;
 }while(i<RelFileSegmentCount*8U);
}
