"""Complete dialogue owner and real loading/controller/renderer chain on LP64 and MIPS.

Only explicit owner/field transport differs between the native and PAL layouts.
Native pointer values are never truncated or repaired. Public assertions remain
enabled, and original/linked MIPS provide the behavioral oracle.
"""
import ctypes as C
import hashlib
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
WIRE_SIZE=0x348
PREFIX_SIZE,ASSET_SIZE=0xAF000,0x20000
LOAD_BASE=0x80200010
LOAD_TEXT=LOAD_BASE+PREFIX_SIZE
ASSETS=LOAD_TEXT+WIRE_SIZE
STATES=(0x80160010,0x80161010,LOAD_TEXT)
SCRIPT,IMAGES,COUNTS,BADGES=0x80170000,0x80174000,0x80178000,0x80179000
STACK,STOP=0x803F0000,0x803FF000
FIELDS=(('b4',4,1),('b5',5,1),('st',6,1),('b7',7,1),('b8',8,1),('b9',9,1),
        ('bA',10,1),('bB',11,1),('sC',12,2),('sE',14,2),('s10',16,2),
        ('s12',18,2),('s14',20,2),('s16',22,2),('i18',24,4),
        ('b1C',28,1),('b1D',29,1),('b1E',30,1),('b1F',31,1),('reserved24',36,4))
POINTERS=('Dialogue','CardDialogueStatePtr','LoadDestCursor','CdDirectoryBuffer',
          'DialoguePageCounts','DialogueAssetTable','BadgeRows')
SCALARS=(('DialogueSceneMode',4),('MenuReturnSubmodeIndex',4),('LoadState',1),('LoadIndex',1),('SelectedStoryBranchId',4),('PendingModeId',2),('CardMenuOperation',2))
SERVICES=(('PsyqBzero',2,(0,)),('PlaySfx',1,()),('DrawDialogueContinueMark',2,()),
          ('DrawAnimatedGlyph',5,(4,)),('UpdateSelectionRank',0,()),
          ('SeqStartTrack',1,()),('CdFileReadRange',4,(3,)),
          ('CdFileReadStart',2,(1,)),('CdFileBytesLoaded',0,()),
          ('LoadTimImage',1,(0,)),('LoadTimList',1,(0,)))
PREFIX=r'''
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/menu_text_state.h"
#include "rr/menu_state_types.h"
#include "rr/badge_row_types.h"
#include "rr/bios_devices.h"
#include "rr/cd_directory_buffer.h"
#include "game/menu/menu13.h"
#include "game/menu/menu8.h"
#include "game/menu/menuslot.h"
#include "game/gfx/fontdraw.h"
#include "game/data/bankrec.h"
#include "game/data/ldscn1.h"
#include "game/data/ldscn4.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/course/trkintp2.h"
typedef struct {u32 before[4];MenuTextState value;u32 after[4];} TextBox;
typedef struct {u32 before[4];u8 prefix[0xAF000];MenuTextState value;u8 assets[0x20000];u32 after[4];} LoadArena;
TextBox texts[2];LoadArena loading;
u16 script[2048];u8 images[8192],counts[128];BadgeRow1C badges[4];
MenuSettings2C SelectionState;
MenuTextState *Dialogue,*CardDialogueStatePtr;
u8 *LoadDestCursor,*CdDirectoryBuffer,*DialoguePageCounts;
s8 *DialogueAssetTable;
u16 *DialogueScriptPtrs[40];BadgeRow1C *BadgeRows;
s32 DialogueSceneMode,MenuReturnSubmodeIndex,SelectedStoryBranchId;
u16 PendingModeId; s16 CardMenuOperation;
u8 LoadState;char LoadIndex;
/* This symbol is the low byte of the first current-choice record, not a
 * second allocation. The loader also reads that same field through its owner. */
#define MenuSelectedTeamId (SelectionState.copies[0].bytes[0])
#define MenuSelectedMakerId (SelectionState.copies[0].bytes[1])
#define MenuActiveDialogId (SelectionState.mode1A)
#define MenuSubmodePendingId (SelectionState.phase17)
#define MenuSelectionLockedFlag (SelectionState.reserved10[4])
typedef u32 (*Observer)(u32,const uintptr_t *);
static Observer observer;
void set_observer(Observer value){observer=value;}
void PsyqBzero(void *buffer,s32 size){uintptr_t a[]={(uintptr_t)buffer,(u32)size,0,0,0};observer(0,a);memset(buffer,0,(size_t)size);}
void PlaySfx(s32 sound){uintptr_t a[]={(u32)sound,0,0,0,0};observer(1,a);}
void DrawDialogueContinueMark(s32 x,s32 y){uintptr_t a[]={(u32)x,(u32)y,0,0,0};observer(2,a);}
void DrawAnimatedGlyph(s32 x,s32 y,s32 glyph,s32 clut,s16 *animation){uintptr_t a[]={(u32)x,(u32)y,(u32)glyph,(u32)clut,(uintptr_t)animation};observer(3,a);}
void UpdateSelectionRank(void){uintptr_t a[5]={0};observer(4,a);}
void SeqStartTrack(s32 track){uintptr_t a[]={(u32)track,0,0,0,0};observer(5,a);}
s32 CdFileReadRange(s32 file,s32 start,s32 sectors,void *buffer){uintptr_t a[]={(u32)file,(u32)start,(u32)sectors,(uintptr_t)buffer,0};return(s32)observer(6,a);}
s32 CdFileReadStart(s32 file,void *buffer){uintptr_t a[]={(u32)file,(uintptr_t)buffer,0,0,0};return(s32)observer(7,a);}
s32 CdFileBytesLoaded(void){uintptr_t a[5]={0};return(s32)observer(8,a);}
void LoadTimImage(int *data){uintptr_t a[]={(uintptr_t)data,0,0,0,0};observer(9,a);}
void LoadTimList(int *data){uintptr_t a[]={(uintptr_t)data,0,0,0,0};observer(10,a);}
_Static_assert(sizeof(void*)==8,"LP64 exercise");
_Static_assert(sizeof(DialogueGlyphAnimation4)==4,"wire glyph cell");
_Static_assert(sizeof(((MenuTextState*)0)->animation28)==800,"all 200 glyph cells");
_Static_assert(offsetof(LoadArena,value)-offsetof(LoadArena,prefix)==0xAF000,"real placement boundary");
_Static_assert(offsetof(LoadArena,assets)==offsetof(LoadArena,value)+sizeof(MenuTextState),"native assets follow the complete owner");
_Static_assert(offsetof(MenuSettings2C,phase17)==0x17&&offsetof(MenuSettings2C,mode1A)==0x1A&&offsetof(MenuSettings2C,copies)==0x1C,"shared scalar selection views");
size_t native_text_size(void){return sizeof(MenuTextState);}
'''


class Animation(C.Union):
    _fields_=[('word',C.c_int32),('halves',C.c_int16*2)]


class Text(C.Structure):
    _fields_=[('p',C.c_void_p),('b4',C.c_int8),('b5',C.c_int8),('st',C.c_int8),('b7',C.c_int8),
              ('b8',C.c_uint8),('b9',C.c_int8),('bA',C.c_uint8),('bB',C.c_uint8),
              *[(n,C.c_int16)for n in ('sC','sE','s10','s12','s14','s16')],('i18',C.c_int32),
              ('b1C',C.c_int8),('b1D',C.c_int8),('b1E',C.c_int8),('b1F',C.c_uint8),
              ('images20',C.c_void_p),('reserved24',C.c_int32),('animation28',Animation*200)]


class Box(C.Structure):
    _fields_=[('before',C.c_uint32*4),('value',Text),('after',C.c_uint32*4)]


class Arena(C.Structure):
    _fields_=[('before',C.c_uint32*4),('prefix',C.c_uint8*PREFIX_SIZE),('value',Text),
              ('assets',C.c_uint8*ASSET_SIZE),('after',C.c_uint32*4)]


class DialoguePipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);code=PREFIX
        for unit,names in (
            ('game/gfx/fontdraw',('DrawAnimatedText',)),
            ('game/menu/menu8',('SelectDialogueScript','DialogueWaitEnterHook','DialogueWaitExitHook')),
            ('game/menu/menuslot',('InitDialogueState',)),
            ('game/menu/menu13',('UpdateDialogue','SkipDialoguePage')),
            ('game/data/bankrec',('SetupDialogueTable',)),
            ('game/data/cdfile1',('GetR4FileIndex',)),('game/data/ldscn1',('LoadDialogueStep',))):
            source=(ROOT/f'src/main/PAL/{unit}.c').read_text()
            for name in names:code+=definition_source(source,name)
        source=directory/'dialogue.c';source.write_text(code)
        result=subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC',
                               '-Wall','-Wextra','-Werror','-Wno-implicit-fallthrough',
                               '-Wno-error=return-type','-fsanitize=undefined,bounds',
                               '-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                               str(source),'-o',str(directory/'dialogue.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.library=C.CDLL(str(directory/'dialogue.so'))
        cls.library.native_text_size.restype=C.c_size_t
        assert C.sizeof(Text)==cls.library.native_text_size()>WIRE_SIZE
        cls.boxes=(Box*2).in_dll(cls.library,'texts');cls.loading=Arena.in_dll(cls.library,'loading')
        cls.states=[b.value for b in cls.boxes]+[cls.loading.value]
        cls.script=(C.c_uint16*2048).in_dll(cls.library,'script')
        cls.images=(C.c_uint8*8192).in_dll(cls.library,'images')
        cls.counts=(C.c_uint8*128).in_dll(cls.library,'counts')
        cls.badges=(C.c_uint8*112).in_dll(cls.library,'badges')
        cls.selection=(C.c_uint8*44).in_dll(cls.library,'SelectionState')
        cls.pointer_cells={n:C.c_void_p.in_dll(cls.library,n)for n in POINTERS}
        cls.scalar_cells={n:(C.c_uint8*size).in_dll(cls.library,n)for n,size in SCALARS}
        cls.table=(C.c_void_p*40).in_dll(cls.library,'DialogueScriptPtrs')
        cls.mapping=[(C.addressof(cls.loading.prefix),LOAD_BASE,PREFIX_SIZE),
                     (C.addressof(cls.loading.assets),ASSETS,ASSET_SIZE),
                     (C.addressof(cls.script),SCRIPT,4096),(C.addressof(cls.images),IMAGES,8192),
                     (C.addressof(cls.counts),COUNTS,128),(C.addressof(cls.badges),BADGES,112)]
        cls.guards=[(C.addressof(b.before),a-16,16)for b,a in zip(cls.boxes,STATES)]
        cls.guards += [(C.addressof(b.after),a+WIRE_SIZE,16)for b,a in zip(cls.boxes,STATES)]
        cls.guards += [(C.addressof(cls.loading.before),LOAD_BASE-16,16),
                       (C.addressof(cls.loading.after),ASSETS+ASSET_SIZE,16)]
        cls.mapping+=cls.guards
        assert all(base>0xFFFFFFFF for base,_,_ in cls.mapping)
        assert all(C.addressof(s)>0xFFFFFFFF for s in cls.states)
        signatures={'InitDialogueState':([C.c_void_p],None),
                    'UpdateDialogue':([C.c_int32,C.c_int32,C.c_void_p],None),
                    'SkipDialoguePage':([C.c_void_p],None),
                    'DrawAnimatedText':([C.c_int32,C.c_int32,C.c_void_p,C.c_int32,C.c_int32],None),
                    'LoadDialogueStep':([],C.c_int32),'SelectDialogueScript':([],None)}
        for name,(args,returned)in signatures.items():
            fn=getattr(cls.library,name);fn.argtypes=args;fn.restype=returned
        for name,offset in (('MenuActiveDialogId',0x1A),('MenuSubmodePendingId',0x17),('MenuSelectionLockedFlag',0x14),('MenuSelectedTeamId',0x1C),('MenuSelectedMakerId',0x1D)):
            assert address_of(name)==address_of('SelectionState')+offset
        cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
        with(ROOT/'build/PAL/main.elf').open('rb')as stream:
            elf=ELFFile(stream)
            cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']

    def native(self,pal):
        if not pal:return 0
        for state,address in zip(self.states,STATES):
            if pal==address:return C.addressof(state)
            if address+40<=pal<address+WIRE_SIZE:return C.addressof(state.animation28)+pal-address-40
        for base,address,size in self.mapping:
            if address<=pal<address+size:return base+pal-address
        raise AssertionError(('unowned PAL address',hex(pal)))

    def pal(self,pointer):
        if not pointer:return 0
        for state,address in zip(self.states,STATES):
            if pointer==C.addressof(state):return address
            start=C.addressof(state.animation28)
            if start<=pointer<start+800:return address+40+pointer-start
        for base,address,size in self.mapping:
            if base<=pointer<base+size:return address+pointer-base
        raise AssertionError(('unowned native address',hex(pointer)))

    def read(self,address,size):return bytes(self.u.mem_read(address&0x1FFFFFFF,size))
    def write(self,address,data):self.u.mem_write(address&0x1FFFFFFF,bytes(data))
    def put(self,address,value,size=4):self.write(address,(value&((1<<(size*8))-1)).to_bytes(size,'little'))

    def set_pointer(self,name,value,native):
        if native:self.pointer_cells[name].value=self.native(value)
        else:self.put(address_of(name),value)

    def state_wire(self,state):
        out=bytearray(WIRE_SIZE)
        struct.pack_into('<I',out,0,self.pal(state.p))
        struct.pack_into('<I',out,32,self.pal(state.images20))
        for name,offset,size in FIELDS:
            out[offset:offset+size]=(getattr(state,name)&((1<<(size*8))-1)).to_bytes(size,'little')
        out[40:]=C.string_at(C.addressof(state.animation28),800)
        return bytes(out)

    def snapshot(self,native):
        if native:
            pointers=tuple(self.pal(self.pointer_cells[n].value)for n in POINTERS)
            table=tuple(self.pal(v)for v in self.table)
            scalars=tuple(bytes(self.scalar_cells[n])for n,_ in SCALARS)
            states=tuple(self.state_wire(s)for s in self.states)
            memory=tuple(hashlib.sha256(C.string_at(base,size)).digest()for base,_,size in self.mapping)
            selection=bytes(self.selection)
            # Native alignment bytes have no PAL field. They must retain their
            # initial contents unless the actual full-owner initializer clears them.
            for state,zeroed in zip(self.states,self.zeroed):
                base=C.addressof(state)
                for start,end in ((36,Text.images20.offset),(Text.animation28.offset+800,C.sizeof(Text))):
                    self.assertEqual(bytes([0 if zeroed else 0xA7])*(end-start),C.string_at(base+start,end-start))
        else:
            pointers=tuple(int.from_bytes(self.read(address_of(n),4),'little')for n in POINTERS)
            table=struct.unpack('<40I',self.read(address_of('DialogueScriptPtrs'),160))
            scalars=tuple(self.read(address_of(n),size)for n,size in SCALARS)
            states=tuple(self.read(a,WIRE_SIZE)for a in STATES)
            memory=tuple(hashlib.sha256(self.read(address,size)).digest()for _,address,size in self.mapping)
            selection=self.read(address_of('SelectionState'),44)
        return pointers,table,scalars,states,memory,selection

    def service(self,index,args,native):
        name,count,pointer_args=SERVICES[index];args=list(args[:count])
        for i in range(count):args[i]=self.pal(args[i])if native and i in pointer_args else args[i]&0xFFFFFFFF
        if name=='PsyqBzero':
            self.assertIn(args[0],STATES)
            self.assertEqual(C.sizeof(Text)if native else WIRE_SIZE,args[1]);args[1]=WIRE_SIZE
        self.events.append((name,tuple(args),self.snapshot(native)))
        if name=='PsyqBzero':
            if native:self.zeroed[STATES.index(args[0])]=True
            else:self.write(args[0],bytes(WIRE_SIZE))
        if name=='DrawAnimatedGlyph':
            self.assertTrue(any(a+40<=args[4]<a+WIRE_SIZE for a in STATES))
            value=(self.case*37+len(self.events)*17)&0xFFFF
            if native:C.c_uint16.from_address(self.native(args[4])).value=value
            else:self.put(args[4],value,2)
        if name=='UpdateSelectionRank'and self.case&1:
            self.set_pointer('Dialogue',STATES[1],native)
            if native:self.selection[0x17]=(self.selection[0x17]+1)%8
            else:self.put(address_of('SelectionState')+0x17,(self.read(address_of('SelectionState')+0x17,1)[0]+1)%8,1)
        if name=='DrawDialogueContinueMark'and self.case&1:
            if native:self.scalar_cells['MenuReturnSubmodeIndex'][0]=1
            else:self.put(address_of('MenuReturnSubmodeIndex'),1)
        if name=='PlaySfx'and self.case&2:self.set_pointer('Dialogue',STATES[1],native)
        if name=='SeqStartTrack'and self.case&4:self.set_pointer('Dialogue',STATES[1],native)
        if name in ('CdFileReadRange','CdFileReadStart'):return(0,1,0xFFFFFFFF)[self.case%3]
        if name=='CdFileBytesLoaded':return(0,2048,8192)[self.case%3]
        return 0

    def initialize(self,native):
        self.zeroed=[False]*3
        for base,address,size in self.mapping:
            data=bytes([0xA7])*size
            if native:C.memmove(base,data,size)
            else:self.write(address,data)
        # A real table image: forty word offsets, forty page counts, then the
        # asset-selection bytes and page payloads at the original file boundary.
        asset=bytearray(ASSET_SIZE)
        for i in range(40):struct.pack_into('<H',asset,i*2,i*8)
        asset[0x50:0x78]=bytes([2])*40
        asset[0x78:0x1B8]=bytes((i%7 for i in range(320)))
        if native:C.memmove(C.addressof(self.loading.assets),bytes(asset),len(asset))
        else:self.write(ASSETS,asset)
        script=bytearray(4096)
        length=(0,1,12,200)[self.case%4]
        data=[1+(i%20)for i in range(length)]
        if length>=12 and self.case&4:data[:10]=[1,0x6003,2,0x6005,3,0x1000,0x1002,0,2,3]
        overhead=4 if length>=12 and self.case&4 else 0
        struct.pack_into('<HBBBB',script,0,17,length,0,overhead,0)
        for i,value in enumerate(data):struct.pack_into('<H',script,6+i*2,value)
        struct.pack_into('<HBBBB4H',script,6+length*2,9,4,0,0,0,1,2,3,4)
        if native:C.memmove(C.addressof(self.script),bytes(script),4096)
        else:self.write(SCRIPT,script)
        for state,address in zip(self.states,STATES):
            wire=bytearray([0xA7])*WIRE_SIZE
            struct.pack_into('<I',wire,0,SCRIPT+6);struct.pack_into('<I',wire,32,IMAGES)
            values=dict(b4=self.case%40,b5=self.case%40,st=self.case%8,b7=self.case%3,
                        b8=2,b9=self.case%3,bA=length,bB=overhead,sC=17,sE=1,s10=self.case%3,
                        s12=length-overhead,s14=0,s16=self.case%2,i18=self.case%20,
                        b1C=self.case%4,b1D=(self.case//2)%4,b1E=0,b1F=0,reserved24=0x12345678)
            if native:
                C.memset(C.addressof(state),0xA7,C.sizeof(Text));state.p=self.native(SCRIPT+6);state.images20=self.native(IMAGES)
                for name,value in values.items():setattr(state,name,value)
            else:
                for name,offset,size in FIELDS:wire[offset:offset+size]=(values[name]&((1<<(size*8))-1)).to_bytes(size,'little')
                self.write(address,wire)
        selection=bytearray(44)
        for i in range(16):selection[i]=(self.case+i)%4
        selection[0x17]=(self.case//8)%8
        mode=(0,1,2,4,6,4,9,4)[self.case%8]
        if self.case%8==0 and self.case&8:selection[0x17]=7;mode=4
        struct.pack_into('<h',selection,0x1A,mode)
        selection[0x1C]=self.case%3;selection[0x1D]=(self.case//3)%4
        if native:C.memmove(C.addressof(self.selection),bytes(selection),44)
        else:self.write(address_of('SelectionState'),selection)
        pointers=dict(Dialogue=STATES[0],CardDialogueStatePtr=STATES[1],LoadDestCursor=ASSETS,
                      CdDirectoryBuffer=LOAD_BASE,DialoguePageCounts=COUNTS,
                      DialogueAssetTable=ASSETS+0x78,BadgeRows=BADGES)
        for name,value in pointers.items():self.set_pointer(name,value,native)
        for i in range(40):
            if native:self.table[i]=self.native(SCRIPT)
            else:self.put(address_of('DialogueScriptPtrs')+i*4,SCRIPT)
        count_data=bytes([2])*128
        if native:C.memmove(C.addressof(self.counts),count_data,128)
        else:self.write(COUNTS,count_data)
        badge_data=bytearray(112)
        for i in range(4):struct.pack_into('<h',badge_data,i*28+16,3 if self.case&1 else 11)
        if native:C.memmove(C.addressof(self.badges),bytes(badge_data),112)
        else:self.write(BADGES,badge_data)
        values=(self.case%3,self.case%2,self.case%8,self.case%8,self.case%4,0x123,3)
        for(name,size),value in zip(SCALARS,values):
            data=value.to_bytes(size,'little')
            if native:C.memmove(C.addressof(self.scalar_cells[name]),data,size)
            else:self.write(address_of(name),data)

    def run_mips(self,name,args):
        cpu=self.cpu;cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
        cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=address_of(name)
        for i,value in enumerate(args):
            if i<4:cpu.r[4+i]=value&0xFFFFFFFF
            else:self.put(STACK+16+(i-4)*4,value)
        targets={address_of(n):i for i,(n,_,_)in enumerate(SERVICES)}
        for _ in range(60000):
            if cpu.pc==STOP:
                for r in (*range(16,24),28,30):self.assertEqual(0xBEEF0000+r,cpu.r[r])
                self.assertEqual(STACK,cpu.r[29]);return cpu.r[2]&0xFFFFFFFF
            if cpu.pc not in targets:cpu.step();continue
            index=targets[cpu.pc];count=SERVICES[index][1];args=list(cpu.r[4:4+min(count,4)])
            args += [int.from_bytes(self.read(cpu.r[29]+16+(i-4)*4,4),'little')for i in range(4,count)]
            resume=cpu.r[31];result=self.service(index,args,False)
            for r in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[r]=0xDEAD0000+r
            cpu.r[2]=result;cpu.pc=resume
        self.fail('MIPS instruction limit')

    def test_actual_loading_and_text_execution(self):
        callback_type=C.CFUNCTYPE(C.c_uint32,C.c_uint32,C.POINTER(C.c_size_t));failures=[]
        def observer(index,args):
            try:return self.service(index,args,True)
            except BaseException as error:failures.append(error);return 0
        callback=callback_type(observer);self.library.set_observer(callback)
        entries=(('InitDialogueState',[STATES[0]],(0,)),
                 ('UpdateDialogue',[80,96,STATES[0]],(2,)),
                 ('SkipDialoguePage',[STATES[0]],(0,)),
                 ('DrawAnimatedText',[80,96,STATES[0],0,0x3FFC],(2,)),
                 ('LoadDialogueStep',[],()),('SelectDialogueScript',[],()))
        for linked in (False,True):
            self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x4000000)
            self.write(0x80010000,self.original)
            if linked:
                for address,data in self.linked:self.write(address,data)
            allowed=[(a,WIRE_SIZE)for a in STATES]+[(a,n)for _,a,n in self.mapping]
            allowed += [(address_of(n),4)for n in POINTERS]+[(address_of(n),size)for n,size in SCALARS]
            allowed += [(address_of('DialogueScriptPtrs'),160),(address_of('SelectionState')+0x1A,2),(STACK-128,160)]
            def store(cpu,access,address,size,value,user):
                p=address&0x1FFFFFFF
                self.assertTrue(any((a&0x1FFFFFFF)<=p and p+size<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(address),size))
            self.cpu=Cpu(self.u,0,100,101,102,store)
            for self.case in range(64):
                for name,args,pointer_args in entries:
                    with self.subTest(linked=linked,case=self.case,function=name):
                        self.initialize(False);self.events=[]
                        returned=self.run_mips(name,args);expected_events=self.events;expected=self.snapshot(False)
                        self.initialize(True);self.events=[]
                        native_args=[self.native(v)if i in pointer_args else v for i,v in enumerate(args)]
                        result=getattr(self.library,name)(*native_args)
                        if failures:raise failures[0]
                        self.assertEqual(expected_events,self.events)
                        self.assertEqual(expected,self.snapshot(True))
                        if name=='LoadDialogueStep':self.assertEqual(returned,result&0xFFFFFFFF)


if __name__=='__main__':unittest.main()
