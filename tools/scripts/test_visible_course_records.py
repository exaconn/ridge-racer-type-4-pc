"""Real PAL visibility packets through relocation, selection, build and append.

All on-disc packets run through current native C with full owners and pointers.
An independent index model checks the lists, including repeated extra records.
Selected complete flows also execute original and linked MIPS instruction bytes.
"""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
NAMES = ['GetViewTrackNode', 'GetViewTrackSegment', 'YawToOctant', 'GetSegmentViewTable',
         'GetTrackSegmentAndYaw', 'BuildViewSegmentObjectList', 'UpdateVisibleViewRecords',
         'RelocateRelativeFile']
RAM, ALT, RECORDS, NODES, SYNTHETIC = 0x80180000, 0x802A0000, 0x802B0000, 0x80270000, 0x80280010
STACK, STOP, SCRATCH = 0x803F0000, 0x803FF000, 0x1F800000
SECTION_NAMES = ['RelFileSection00','RelFileSection01','TrackRelSegment2Ptr','RelFileOffsetPtr3','TrackRelSegment4Ptr',
                 'RelFileSection05','RelFileOffsetPtr6','RelFileOffsetPtr7','TrackRelSegment8Ptr','RelFileOffsetPtr9',
                 'TrackCollisionGridPtr','RelFileSection11','RelFileOffsetPtr12','RelFileOffsetPtr13','TrackSectionBoundsPtr','RelFileSection15']
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/player_cars.h"
#include "rr/track_sample_types.h"
#include "rr/relative_file.h"
#include "rr/rro_model.h"
#include "game/gfx/model.h"
#include "game/camera/viewcam.h"
#include "game/camera/viewini.h"
#include "game/course/trkclip.h"
typedef struct {
 u8 before[32];VisibleCourseViews lists[2];AdditionalCourseViews extra;
 RroModelScratch scratch;RaceVehicle cars[2];TrackSample3C nodes[768];
 VisibleCourseViews *active;CourseViewIndices *packet;CourseViewRecord *records;
 RelativeHeader *header;RelativeSlot *slots;u8 *first,*sections[16];s32 count,mode;
 s16 segmentYaw[2];u8 after[32];
} World;
static World live,initial,expected;
static u8 *allocation,*arena,*registered,*alternate,*synthetic;
static size_t arenaBytes,delta,wireBytes,headerOffset,recordOffset,recordBytes;
static unsigned segments,recordCount,number,selected,flags,player,searches;
static u8 *initialArena;
#define SCRATCHPAD ((u8*)&live.scratch)
#define G ((RroSceneScratchPrefix *)SCRATCHPAD)
#define PlayerCars live.cars
#define ViewUsesCarTrackNode live.mode
#define TrackNodes (live.nodes)
#define ModelTrackSegmentResult live.segmentYaw
#define PrimaryVisibleViews live.lists[0]
#define ActiveVisibleViews live.active
#define TrackCourseViewIndices live.packet
#define ExtraCourseViewTable live.extra
#define TrackRelFileHeaderPtr live.header
#define RelFileSegmentCount live.count
#define RelFileSegmentViewSlots live.slots
#define TrackRelSecondTablePtr live.first
#define RelFileViewRecords live.records
'''
PREFIX += ''.join(f'#define {name} live.sections[{i}]\n' for i,name in enumerate(SECTION_NAMES))
PREFIX += r'''
static u32 get(const u8 *p){u32 x;memcpy(&x,p,4);return x;}
static void put(u8 *p,u32 x){memcpy(p,&x,4);}
static s32 word(u32 x){s32 n;memcpy(&n,&x,4);return n;}
int FindTrackNodeAtXZ(int x,int z) {
 assert(searches++==0);assert(x==0x12345678&&z==word(0x87654321));
 if(flags&1U)live.records=(CourseViewRecord*)alternate;
 if(flags&2U)live.active=&live.lists[1];
 return (int)selected*5+2;
}
'''
TAIL = r'''
void load_course(const u8 *file,unsigned length,unsigned offset,unsigned records,unsigned bytes) {
 free(allocation);free(registered);free(alternate);free(synthetic);free(initialArena);
 headerOffset=offset;wireBytes=length-offset;segments=get(file+offset);
 size_t oldPrefix=80+(size_t)segments*8*4;
 size_t newPrefix=sizeof(RelativeHeader)+(size_t)segments*8*sizeof(RelativeSlot);
 delta=newPrefix-oldPrefix;arenaBytes=wireBytes+delta;
 allocation=malloc(arenaBytes+64);assert(allocation);arena=allocation+32;
 memset(allocation,0xA6,arenaBytes+64);memset(arena,0xA5,newPrefix);
 memcpy(arena+newPrefix,file+offset+oldPrefix,wireBytes-oldPrefix);
 RelativeHeader *header=(RelativeHeader*)arena;header->count=segments;
 for(unsigned i=0;i<3;i++)header->first[i].relative=get(file+offset+4+i*4)+(u32)delta;
 for(unsigned i=0;i<16;i++)header->offsets[i]=get(file+offset+16+i*4)+(u32)delta;
 RelativeSlot *slots=(RelativeSlot*)(header+1);
 for(unsigned i=0;i<segments*8;i++)slots[i].relative=get(file+offset+80+i*4)+(u32)delta;
 recordOffset=records;recordBytes=bytes;recordCount=bytes/80;
 alternate=malloc(bytes);synthetic=malloc(4+255*2);registered=malloc(arenaBytes);initialArena=malloc(arenaBytes);
 assert(alternate&&synthetic&&registered&&initialArena);memcpy(alternate,file+offset+records,bytes);
 memset(&live,0xA5,sizeof(live));RelocateRelativeFile(header);
 assert(live.header==header&&live.count==(s32)segments&&live.slots==slots);
 assert(live.records==(CourseViewRecord*)(arena+recordOffset+delta));
 for(unsigned i=0;i<3;i++)assert(header->first[i].absolute==arena+get(file+offset+4+i*4)+delta);
 for(unsigned i=0;i<16;i++)assert(live.sections[i]==arena+get(file+offset+16+i*4)+delta);
 for(unsigned i=0;i<segments*8;i++)assert(slots[i].absolute==arena+get(file+offset+80+i*4)+delta);
 memcpy(registered,arena,arenaBytes);
 _Static_assert(sizeof(void*)==8,"full pointers");
 _Static_assert(__alignof__(CourseViewRecord)==1,"serialized displacement transport");
 assert((uintptr_t)arena>UINT32_MAX&&(uintptr_t)&live>UINT32_MAX);
}
static u32 address(const void *pointer) {
 uintptr_t p=(uintptr_t)pointer,a=(uintptr_t)arena;
 if(p>=a+sizeof(RelativeHeader)+(size_t)segments*8*sizeof(RelativeSlot)&&p<a+arenaBytes)
  return 0x80180000U+(u32)headerOffset+(u32)(p-a-delta);
 if(p>=(uintptr_t)alternate&&p<(uintptr_t)alternate+recordBytes)return 0x802B0000U+(u32)(p-(uintptr_t)alternate);
 if(p==(uintptr_t)synthetic)return 0x80280010U;
 assert(!"pointer outside actual owners");return 0;
}
u32 begin_case(unsigned serial,unsigned segment,unsigned octant,unsigned chosen,unsigned mode,unsigned changes,
               u32 syntheticCount,unsigned extraCount,const u8 *extras) {
 memcpy(arena,registered,arenaBytes);number=serial;selected=segment;player=chosen;flags=changes;searches=0;
 memset(&live,0xA5,sizeof(live));
 live.header=(RelativeHeader*)arena;live.slots=(RelativeSlot*)(live.header+1);
 live.records=(CourseViewRecord*)(arena+recordOffset+delta);
 live.mode=(s32)mode;live.active=&live.lists[1];live.packet=(CourseViewIndices*)synthetic;
 live.count=(s32)segments;
 live.lists[1].objects.count=2;live.lists[1].objects.records[0]=live.records;
 live.lists[1].objects.records[1]=live.records+1;
 live.cars[0].trackNodeF0=(s32)segment*5;live.cars[1].trackNodeF0=(s32)segment*5+3;
 live.scratch.scene.view.projection.pose08.position00.xyz.x=0x12345678;
 live.scratch.scene.view.projection.pose08.position00.xyz.z=word(0x87654321);
 live.scratch.scene.view.projection.pose08.angles10.xyz.y=(s32)octant*512;
 for(unsigned i=0;i<768;i++)live.nodes[i].road.heading=(s16)(i*173U+serial*19U);
 live.extra.count=(u8)extraCount;memcpy(live.extra.indices,extras,31);
 unsigned slot=segment*8+octant;
 if(syntheticCount!=0xBADF00DU) {
  put(synthetic,syntheticCount);
  for(unsigned i=0;i<255;i++){u16 index=0;memcpy(synthetic+4+i*2,&index,2);}
  live.slots[slot].absolute=synthetic;
 }
 memcpy(&initial,&live,sizeof(live));memcpy(initialArena,arena,arenaBytes);
 return address(live.slots[slot].absolute);
}
void invoke(void){BuildViewSegmentObjectList((int)player);UpdateVisibleViewRecords();}
static CourseViewRecord *record_address(u32 value) {
 u32 base=0x80180000U+(u32)headerOffset+(u32)recordOffset;
 if(value>=base&&value-base<recordBytes) return (CourseViewRecord*)(arena+recordOffset+delta+value-base);
 assert(value>=0x802B0000U&&value-0x802B0000U<recordBytes);
 return (CourseViewRecord*)(alternate+value-0x802B0000U);
}
void check(unsigned primaryCount,const u32 *primary,unsigned activeCount,const u32 *active,unsigned changed,unsigned mode) {
 memcpy(&expected,&initial,sizeof(expected));
 unsigned bank=mode==0&&(changed&1U),root=mode==0&&((changed>>1)&1U);
 CourseViewRecord *records=bank?(CourseViewRecord*)alternate:(CourseViewRecord*)(arena+recordOffset+delta);
 expected.records=records;expected.active=&live.lists[root];
 unsigned slot=selected*8+(unsigned)initial.scratch.scene.view.projection.pose08.angles10.xyz.y/512;
 expected.packet=(CourseViewIndices*)live.slots[slot].absolute;
 expected.lists[0].objects.count=primaryCount;
 for(unsigned i=0;i<primaryCount;i++)expected.lists[0].objects.records[i]=record_address(primary[i]);
 expected.lists[root].objects.count=activeCount;
 for(unsigned i=0;i<activeCount;i++)expected.lists[root].objects.records[i]=record_address(active[i]);
 if(mode==0){
  expected.segmentYaw[0]=(s16)selected;
  s32 yaw=initial.scratch.scene.view.projection.pose08.angles10.xyz.y;
  s32 heading=initial.nodes[selected*5+2].road.heading;
  expected.segmentYaw[1]=(s16)(((u32)yaw-(u32)(2048-heading))&4095U);
 }
 assert(searches==(mode==0));assert(!memcmp(&live,&expected,sizeof(live)));
 assert(!memcmp(arena,initialArena,arenaBytes));
 for(unsigned i=0;i<32;i++)assert(allocation[i]==0xA6&&allocation[arenaBytes+32+i]==0xA6);
 for(unsigned i=0;i<primaryCount;i++)assert((uintptr_t)live.lists[0].objects.records[i]>UINT32_MAX);
}
'''


def packed_list(indices):
    result = bytearray(b'\xA5'*1024)
    struct.pack_into('<I', result, 0, len(indices))
    for i,index in enumerate(indices):struct.pack_into('<I', result, 4+i*4, index)
    return result


class VisibleCourseRecordsTest(unittest.TestCase):
    def test_disc_packets_native_owners_and_original_mips(self):
        owners=source_owners(ROOT/'src/main/PAL');bodies=[]
        for name in NAMES:
            body=definition_source(owners[name].read_text(),name)
            if name=='RelocateRelativeFile':
                body=re.sub(r'\s+(?:asm|__asm__)\("\$\d+"\)','',body)
                body=re.sub(r'__asm__\(""[^;]*;','',body)
            bodies.append(body)
        blob=(ROOT/'disc/PAL/R4.BIN').read_bytes();sectors=struct.unpack_from('<951I',blob,4)
        exe=(ROOT/'assets/PAL/main.exe').read_bytes();exe_base=struct.unpack_from('<I',exe,24)[0]
        names=['PrimaryVisibleViews','ActiveVisibleViews','TrackCourseViewIndices','ExtraCourseViewTable','ModelTrackSegmentResult','PlayerCars','TrackNodes',
               'ViewUsesCarTrackNode','TrackRelFileHeaderPtr','RelFileSegmentCount','RelFileSegmentViewSlots','TrackRelSecondTablePtr','RelFileViewRecords',*SECTION_NAMES]
        symbols={name:address_of(name) for name in names};start={name:address_of(name) for name in NAMES}
        spans={n:(int(a,16),int(s,16)) for n,a,s in map(str.split,(ROOT/'configs/PAL/functions.txt').read_text().splitlines())}
        self.assertEqual(symbols['ActiveVisibleViews']-symbols['PrimaryVisibleViews'],1024)
        variants={'original':{}}
        with (ROOT/'build/PAL/main.elf').open('rb') as stream:
            elf=ELFFile(stream);table=elf.get_section_by_name('.symtab');code={}
            for name in NAMES:
                symbol=table.get_symbol_by_name(name)[0];section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr']
                self.assertEqual(symbol['st_value'],start[name]);code[name]=section.data()[offset:offset+symbol['st_size']]
            variants['compiled']=code
        engines=[];phase=[0];writes=[]
        with TemporaryDirectory() as directory:
            source=Path(directory)/'native.c';library=Path(directory)/'native.dylib';source.write_text(PREFIX+'\n'.join(bodies)+TAIL)
            subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),'-o',str(library)],check=True)
            native=C.CDLL(str(library));native.load_course.argtypes=[C.c_void_p,*([C.c_uint]*4)]
            native.begin_case.argtypes=[*([C.c_uint]*8),C.c_void_p];native.begin_case.restype=C.c_uint32
            native.check.argtypes=[C.c_uint,C.c_void_p,C.c_uint,C.c_void_p,C.c_uint,C.c_uint]
            for variant,code in variants.items():
                memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(SCRATCH,4096)
                write=lambda a,data,m=memory:m.mem_write(a&0x1FFFFFFF,bytes(data))
                read=lambda a,n,m=memory:bytes(m.mem_read(a&0x1FFFFFFF,n))
                write(exe_base,exe[0x800:])
                for name,data in code.items():write(start[name],data)
                def store(cpu,access,address,size,value,user):
                    a=address&0x1FFFFFFF;stack=(STACK-128)&0x1FFFFFFF
                    allowed=[(stack,128)]
                    if phase[0]:allowed.extend([((RAM+phase[0])&0x1FFFFFFF,phase[1]),*[(symbols[n]&0x1FFFFFFF,4) for n in ['TrackRelFileHeaderPtr','RelFileSegmentCount','RelFileSegmentViewSlots','TrackRelSecondTablePtr','RelFileViewRecords',*SECTION_NAMES]]])
                    else:allowed.extend([(symbols['PrimaryVisibleViews']&0x1FFFFFFF,1024),(ALT&0x1FFFFFFF,1024),
                                         *[(symbols[n]&0x1FFFFFFF,4) for n in ['ActiveVisibleViews','TrackCourseViewIndices','ModelTrackSegmentResult']]])
                    self.assertTrue(any(lo<=a and a+size<=lo+n for lo,n in allowed),(hex(address),size))
                    writes.append((address,size,value&((1<<(size*8))-1)))
                engines.append((variant,Cpu(memory,0,100,101,102,store),write,read))
            def run(cpu,entry,args,write,read,handler=None,limit=20000):
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[4:4+len(args)]=[v&0xFFFFFFFF for v in args]
                cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=start[entry];write(STACK-256,b'\xA5'*512)
                for _ in range(limit):
                    if cpu.pc==STOP:break
                    if handler and cpu.pc==address_of('FindTrackNodeAtXZ'):
                        self.assertIsNone(cpu.delay);handler(cpu);resume=cpu.r[31]
                        for reg in (1,2,3,*range(4,16),24,25):cpu.r[reg]=0xDEAD0000+reg
                        cpu.r[2]=selected*5+2;cpu.pc=resume;cpu.hi=0x12345678;cpu.lo=0x87654321
                    else:cpu.step()
                self.assertEqual(cpu.pc,STOP,entry);self.assertEqual(cpu.r[29],STACK);self.assertEqual(cpu.r[31],STOP)
                for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)
                self.assertEqual(read(STACK-256,128),b'\xA5'*128);self.assertEqual(read(STACK,256),b'\xA5'*256)
            serial=0;target_cases=0;seen=set();duplicates=0;clamps=set();all_counts=[]
            for course in range(9):
                entry=course*2+1;file=blob[sectors[entry]*2048:sectors[entry+1]*2048];offset=struct.unpack_from('<I',file,4)[0]
                header=struct.unpack_from('<20I',file,offset);segments=header[0];slots=struct.unpack_from('<'+str(segments*8)+'I',file,offset+80)
                end=min([p for p in header[1:] if p>header[3]]+[len(file)-offset]);record_bytes=end-header[3];record_count=record_bytes//80
                self.assertEqual(record_bytes%80,0);native.load_course(C.create_string_buffer(file),len(file),offset,header[3],record_bytes)
                pal_header=RAM+offset;record_base=pal_header+header[3]
                relocated=bytearray(file)
                for i in range(1,4):struct.pack_into('<I',relocated,offset+i*4,pal_header+header[i])
                for i,slot in enumerate(slots):struct.pack_into('<I',relocated,offset+80+i*4,pal_header+slot)
                for variant,cpu,write,read in engines:
                    write(RAM,file);phase[:]=[offset,80+segments*8*4];writes.clear()
                    run(cpu,'RelocateRelativeFile',[pal_header],write,read,limit=100000)
                    self.assertEqual(read(RAM,len(file)),relocated,(variant,course))
                    self.assertEqual(read(symbols['RelFileViewRecords'],4),struct.pack('<I',record_base))
                    self.assertEqual(read(symbols['RelFileSegmentViewSlots'],4),struct.pack('<I',pal_header+80))
                    for i,n in enumerate(SECTION_NAMES):self.assertEqual(read(symbols[n],4),struct.pack('<I',pal_header+header[4+i]))
                cases=[(slot,None) for slot in range(segments*8)]
                if course==0:
                    for n in [0,1,2,31,32,254,255,256,65535,0x7FFFFFFF,0x80000000,0xFFFFFFFF]:
                        for f in range(8):cases.append((f%8,n))
                counts_seen=set()
                for slot,synthetic_count in cases:
                    selected,octant=divmod(slot,8);player=serial%2;mode=(serial//2)%2;flags=(serial//4)%4
                    if synthetic_count is None:
                        packet=pal_header+slots[slot];count=struct.unpack_from('<I',file,offset+slots[slot])[0]
                        indices=list(struct.unpack_from('<'+str(count)+'H',file,offset+slots[slot]+4));count_key=count
                    else:count=synthetic_count;indices=[0]*min(count,255);packet=SYNTHETIC;count_key=count
                    initial_indices=indices[:255];self.assertTrue(all(i<record_count for i in initial_indices));all_counts.append(len(initial_indices))
                    extra_count=serial%32;extras=[(serial*17+i*13)%record_count for i in range(31)]
                    missing=next((i for i in range(record_count) if i not in initial_indices),0)
                    if len(initial_indices)>=255:extras=[0]*31
                    elif len(initial_indices)==254:extras=[1]+[0]*30
                    elif extra_count>=3:extras[:3]=[missing,missing,initial_indices[0] if initial_indices else missing]
                    active_root=int(mode==0 and flags&2!=0);bank=int(mode==0 and flags&1!=0)
                    base=RECORDS if bank else record_base
                    primary_addresses=[base+i*80 for i in initial_indices]
                    before=[record_base,record_base+80] if active_root else primary_addresses
                    appended=[base+i*80 for i in extras[:extra_count] if base+i*80 not in before]
                    result=before+appended;self.assertLessEqual(len(result),255)
                    if len(appended)!=len(set(appended)):duplicates+=1
                    primary=primary_addresses if active_root else result
                    native.begin_case(serial,selected,octant,player,mode,flags,0xBADF00D if synthetic_count is None else synthetic_count,extra_count,C.create_string_buffer(bytes(extras)))
                    native.invoke();native.check(len(primary),(C.c_uint32*len(primary))(*primary),len(result),(C.c_uint32*len(result))(*result),flags,mode)
                    seen.add((course,player,mode,flags,active_root,bank));clamps.add(min(count,255))
                    target=synthetic_count is not None or serial%31==0 or count_key not in counts_seen
                    counts_seen.add(count_key)
                    if target:
                        for variant,cpu,write,read in engines:
                            phase[:]=[0];write(SCRATCH,b'\xA5'*4096);put=lambda a,v:write(a,struct.pack('<I',v&0xFFFFFFFF))
                            pose=bytearray(b'\xA5'*0xDC);struct.pack_into('<I',pose,8,0x12345678);struct.pack_into('<I',pose,16,0x87654321);struct.pack_into('<I',pose,28,octant*512);write(SCRATCH,pose)
                            cars=bytearray(b'\xA5'*1600);struct.pack_into('<I',cars,0xF0,selected*5);struct.pack_into('<I',cars,800+0xF0,selected*5+3);write(symbols['PlayerCars'],cars)
                            nodes=bytearray(b'\xA5'*(768*60))
                            for i in range(768):struct.pack_into('<H',nodes,i*60+18,(i*173+serial*19)&65535)
                            write(NODES,nodes);put(symbols['TrackNodes'],NODES);put(symbols['ViewUsesCarTrackNode'],mode)
                            put(symbols['RelFileViewRecords'],record_base);put(symbols['RelFileSegmentViewSlots'],pal_header+80)
                            put(pal_header+80+slot*4,packet)
                            if synthetic_count is not None:write(SYNTHETIC,struct.pack('<I',synthetic_count)+bytes(510))
                            write(RECORDS,file[offset+header[3]:offset+header[3]+record_bytes])
                            write(symbols['PrimaryVisibleViews'],b'\xA5'*1024);write(ALT-16,b'\xA6'*16+packed_list([record_base,record_base+80])+b'\xA6'*16)
                            put(symbols['ActiveVisibleViews'],ALT);put(symbols['TrackCourseViewIndices'],SYNTHETIC)
                            write(symbols['ExtraCourseViewTable'],bytes([extra_count,*extras]));write(symbols['ModelTrackSegmentResult'],b'\xA5'*4)
                            def search(cpu):
                                self.assertEqual(cpu.r[4:6],[0x12345678,0x87654321])
                                if flags&1:put(symbols['RelFileViewRecords'],RECORDS)
                                if flags&2:put(symbols['ActiveVisibleViews'],ALT)
                            writes.clear();run(cpu,'BuildViewSegmentObjectList',[player],write,read,search)
                            run(cpu,'UpdateVisibleViewRecords',[],write,read)
                            base=RECORDS if bank else record_base
                            self.assertEqual(read(symbols['PrimaryVisibleViews'],1024),packed_list(primary),(variant,serial,'primary'))
                            alt=packed_list(result) if active_root else packed_list([record_base,record_base+80])
                            self.assertEqual(read(ALT-16,1056),b'\xA6'*16+alt+b'\xA6'*16)
                            self.assertEqual(read(symbols['ActiveVisibleViews'],4),struct.pack('<I',ALT if active_root else symbols['PrimaryVisibleViews']))
                            self.assertEqual(read(symbols['TrackCourseViewIndices'],4),struct.pack('<I',packet))
                            self.assertEqual(read(symbols['RelFileViewRecords'],4),struct.pack('<I',base))
                            self.assertEqual(read(symbols['ExtraCourseViewTable'],32),bytes([extra_count,*extras]))
                            expected_pair=b'\xA5'*4
                            if mode==0:
                                heading=C.c_int16((selected*5+2)*173+serial*19).value
                                expected_pair=struct.pack('<hh',selected,(octant*512-(2048-heading))&4095)
                            self.assertEqual(read(symbols['ModelTrackSegmentResult'],4),expected_pair)
                            self.assertEqual(read(SCRATCH,len(pose)),pose);self.assertEqual(read(symbols['PlayerCars'],1600),cars)
                            self.assertEqual(read(NODES,len(nodes)),nodes)
                        target_cases+=2
                    serial+=1
            self.assertEqual(serial,7728);self.assertGreater(duplicates,100)
            self.assertEqual({(c,p,m) for c,p,m,f,r,b in seen},{(c,p,m) for c in range(9) for p in range(2) for m in range(2)})
            self.assertTrue({0,1,254,255}<=clamps);self.assertEqual(max(all_counts),255)
            print(f'{serial} native flows (7632 on-disc lists +96 count boundaries), {target_cases} original/compiled MIPS flows +18 actual relocations; both players, live roots, preserved extra duplicates, full pointers, complete owner guards and UBSan')
