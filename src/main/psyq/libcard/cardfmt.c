#include "rr/card_format.h"

s32 _card_format(s32 arg0){
    s32 port=arg0;
    s32 i=0;
    CardFormatSector *scratch=&D_80114328;
    CardDirectoryRecord *base=CardFormatDirRecords;
    s32 state=0xA0,last=0xFFFF,success=1;
    s32 next;
    s32 status;
    s32 result;
    CardDirectoryRecord *entry;
    u32 offset;
    CardBrokenRecord *slot;

    do {
        PsyqBzero(scratch,128);
        offset=i*32U;
        entry=(CardDirectoryRecord*)(offset+(unsigned long)base);
        PsyqBzero(entry,32);

        CardFormatDirRecords[i].fields.state=state;
        CardFormatDirRecords[i].fields.size=0;
        CardFormatDirRecords[i].fields.next=last;
        scratch->directory=entry->wire;
        next=i+1;
        if(_card_sector_write(port,next,scratch->bytes)!=success){result=0;goto done;}
        i=next;
    }while(i<15);
    {
    CardFormatSector *second;
    s32 invalid;
    s32 one;
    i=0;
    invalid=-1;second=scratch;one=1;
    slot=D_801142D8;

    do {
        slot->value=invalid;
        PsyqBzero(second,128);
        second->broken=slot->wire;
        status=_card_sector_write(port,i+16,second->bytes);
        i++;
        if(status!=one){result=0;goto done;}
        slot++;
    }while(i<20);
    PsyqBzero(second,128);
    second->bytes[0]='M';second->bytes[1]='C';
    result=_card_sector_write(port,0,second->bytes)==1;
}
done:
    return result;
}
