"""Real menu allocation/loading chain: complete native pointers vs both MIPS images."""
import ctypes as C
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
SIZE=0x48000
ARENAS=(0x80200000,0x80280000)
STACK,STOP=0x803F0000,0x803FF000
ALLOCATORS=('AllocBadgeRows','AllocRect16Rows','AllocGlyphRows','AllocImageRows','AllocUiRows')
POINTERS=('CdDirectoryBuffer','MenuArenaBase','LoadCursor','MenuBaseLoadCursor',
          'BadgeRows','Rect16Rows','GlyphRows','ImageRows','UiRows')
SCALARS=(('BadgeRowCount',4),('Rect16RowCount',4),('GlyphRowCount',4),
         ('ImageRowCount',4),('UiRowCount',4),('MenuTextRowCount',4),
         ('LoadState',1),('LoadIndex',1),('GameModeType',2),('MainMenuChoice',2))
SERVICES=('PsyqBzero','DebugPrintStub','CdFileReadStart','CdFileBytesLoaded')
PREFIX=r'''
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include "rr/menu_arenas.h"
#include "rr/cd_directory_buffer.h"
#include "rr/menu_rect_rows.h"
#include "rr/diagnostics.h"
#include "rr/bios_devices.h"
#include "game/gfx/gfxmtx.h"
#include "game/ui/uidraw.h"
#include "game/ui/uitext.h"
#include "game/ui/uiwin.h"
#include "game/menu/mnrows.h"
#include "game/data/ldscn1.h"
#include "game/data/ldscn4.h"
#include "game/data/cdfile2.h"
#include "game/data/cdfile1.h"
u8 *CdDirectoryBuffer;
void *MenuArenaBase,*LoadCursor,*MenuBaseLoadCursor;
MenuArenas8 MenuArenas;
BadgeRow1C *BadgeRows;
MenuRectRow10 *Rect16Rows;
MenuGlyphRow1C *GlyphRows;
ImageRow20 *ImageRows;
UiRow24 *UiRows;
s32 BadgeRowCount,Rect16RowCount,GlyphRowCount,ImageRowCount,UiRowCount,MenuTextRowCount;
u8 LoadState;
char LoadIndex;
s16 GameModeType,MainMenuChoice;
char StrCdReadPtrFmt[64],StrSizeEqLdFmt[64],SizeLdFmt[64];
typedef u32 (*Observer)(u32,const uintptr_t *);
static Observer observer;
void set_observer(Observer value){observer=value;}
void PsyqBzero(void *buffer,s32 size){
 uintptr_t args[]={(uintptr_t)buffer,(u32)size,0};observer(0,args);
 memset(buffer,0,(size_t)size);
}
s32 DebugPrintStub(const char *format,...){
 uintptr_t args[]={(uintptr_t)format,0,0};va_list ap;va_start(ap,format);
 if(format==StrCdReadPtrFmt)args[1]=(uintptr_t)va_arg(ap,void *);
 else args[1]=(u32)va_arg(ap,int);
 va_end(ap);return observer(1,args);
}
s32 CdFileReadStart(s32 entry,void *buffer){uintptr_t args[]={(u32)entry,(uintptr_t)buffer,0};return(s32)observer(2,args);}
s32 CdFileBytesLoaded(void){uintptr_t args[]={0,0,0};return(s32)observer(3,args);}
_Static_assert(sizeof(void *)==8,"exercise full native pointers");
_Static_assert(sizeof(MenuArenas8)==16,"two full native pointer cells");
'''


class MenuArenaPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);code=PREFIX
        for unit,names in (
            ('game/gfx/gfxmtx',('AllocBadgeRows',)),
            ('game/ui/uidraw',('AllocRect16Rows','AllocGlyphRows','InitRect16Row')),
            ('game/ui/uitext',('AllocImageRows',)),('game/ui/uiwin',('AllocUiRows',)),
            ('game/menu/mnrows',('InitMenuArenas',)),
            ('game/data/cdfile1',('GetR4FileIndex',)),
            ('game/data/ldscn1',('LoadModeMenuAssetsStep','LoadOpeningMovieTimStep'))):
            text=(ROOT/f'src/main/PAL/{unit}.c').read_text()
            for name in names:
                body=definition_source(text,name)
                body=re.sub(r'\s+(?:__asm__|asm)\("\$[0-9]+"\)','',body)
                code+=body
        source=directory/'menu.c';source.write_text(code)
        result=subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC',
                               '-Wall','-Wextra','-Werror','-Wno-implicit-fallthrough',
                               '-Wno-error=return-type','-fsanitize=undefined,bounds',
                               '-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                               str(source),'-o',str(directory/'menu.so')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.library=C.CDLL(str(directory/'menu.so'))
        for name in ALLOCATORS:
            fn=getattr(cls.library,name);fn.argtypes=[C.c_void_p];fn.restype=C.c_int32
        for name in ('LoadModeMenuAssetsStep','LoadOpeningMovieTimStep'):
            fn=getattr(cls.library,name);fn.argtypes=[];fn.restype=C.c_int32
        cls.library.InitMenuArenas.argtypes=[];cls.library.InitMenuArenas.restype=None
        cls.library.InitRect16Row.argtypes=[C.c_int32]+[C.c_int16]*6;cls.library.InitRect16Row.restype=None
        cls.buffers=[(C.c_uint32*(SIZE//4))()for _ in ARENAS]
        cls.pointer_cells={n:C.c_void_p.in_dll(cls.library,n)for n in POINTERS}
        cls.arena_cells=(C.c_void_p*2).in_dll(cls.library,'MenuArenas')
        cls.scalar_cells={n:(C.c_ubyte*size).in_dll(cls.library,n)for n,size in SCALARS}
        cls.messages=[(C.c_ubyte*64).in_dll(cls.library,n)for n in ('StrCdReadPtrFmt','StrSizeEqLdFmt','SizeLdFmt')]
        cls.mapping=[(C.addressof(buf),pal,SIZE)for buf,pal in zip(cls.buffers,ARENAS)]
        cls.mapping += [(C.addressof(buf),address_of(n),32)for buf,n in zip(cls.messages,('StrCdReadPtrFmt','StrSizeEqLdFmt','SizeLdFmt'))]
        assert all(base>0xFFFFFFFF for base,_,_ in cls.mapping)
        cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
        with(ROOT/'build/PAL/main.elf').open('rb')as stream:
            elf=ELFFile(stream)
            cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']

    def native(self,pal):
        for base,address,size in self.mapping:
            if address<=pal<=address+size:return base+pal-address
        raise AssertionError(('unknown PAL owner',hex(pal)))

    def pal(self,pointer):
        if not pointer:return 0
        for base,address,size in self.mapping:
            if base<=pointer<=base+size:return address+pointer-base
        raise AssertionError(('unknown native owner',hex(pointer)))

    def read(self,address,size):return bytes(self.u.mem_read(address&0x1FFFFFFF,size))
    def write(self,address,data):self.u.mem_write(address&0x1FFFFFFF,data)
    def put(self,address,value,size=4):self.write(address,int(value&((1<<(size*8))-1)).to_bytes(size,'little'))

    def set_pointer(self,name,value,native):
        if native:self.pointer_cells[name].value=self.native(value)
        else:self.put(address_of(name),value)

    def snapshot(self,native):
        if native:
            pointers=tuple(self.pal(self.pointer_cells[n].value)for n in POINTERS)
            pointers+=tuple(self.pal(v)for v in self.arena_cells)
            scalars=tuple(bytes(self.scalar_cells[n])for n,_ in SCALARS)
            buffers=tuple(C.string_at(C.addressof(b),SIZE)for b in self.buffers)
        else:
            pointers=tuple(int.from_bytes(self.read(address_of(n),4),'little')for n in POINTERS)
            pointers+=struct.unpack('<II',self.read(address_of('MenuArenas'),8))
            scalars=tuple(self.read(address_of(n),size)for n,size in SCALARS)
            buffers=tuple(self.read(a,SIZE)for a in ARENAS)
        return pointers,scalars,buffers

    def service(self,index,args,native):
        args=list(args[:3]);pointers={0:(0,),1:(0,),2:(1,),3:()}[index]
        if index==1 and (self.pal(args[0])if native else args[0])==address_of('StrCdReadPtrFmt'):pointers=(0,1)
        for i in range(3):args[i]=self.pal(args[i])if native and i in pointers else args[i]&0xFFFFFFFF
        if index==0:args[2]=0
        if index in (2,3):args[2]=0
        if index==3:args=[0,0,0]
        # The original cd_read_ptr format has one conversion. A2 is an
        # allocator temporary at this call, not another variadic argument.
        if index==1:args[2]=0
        self.events.append((index,tuple(args),self.snapshot(native)))
        if index==0:
            self.assertLessEqual(args[1],0x1000)
            if not native:self.write(args[0],bytes(args[1]))
        if index==1 and self.case&1:self.set_pointer('CdDirectoryBuffer',ARENAS[1]+64,native)
        if index==3 and self.case&2:self.set_pointer('LoadCursor',ARENAS[1]+128,native)
        if index==2:return(0,1,0xFFFFFFFF)[self.case%3]
        if index==3:return(0,4,73,0xFFFFFFFF)[(self.case//3)%4]
        return 1

    def initialize(self,native):
        for i,(buf,address)in enumerate(zip(self.buffers,ARENAS)):
            data=bytes([0xA7+i])*SIZE
            if native:C.memmove(C.addressof(buf),data,SIZE)
            else:self.write(address,data)
        for i,name in enumerate(POINTERS):self.set_pointer(name,ARENAS[0]+64+i*32,native)
        values=(ARENAS[0]+32,ARENAS[1]+32)
        if native:
            for i,v in enumerate(values):self.arena_cells[i]=self.native(v)
        else:self.write(address_of('MenuArenas'),struct.pack('<II',*values))
        scalar_values=(self.case%11,4,4,1,40,55,self.case%5,3,(0,4,5)[self.case%3],6)
        for(name,size),v in zip(SCALARS,scalar_values):
            data=int(v).to_bytes(size,'little')
            if native:C.memmove(C.addressof(self.scalar_cells[name]),data,size)
            else:self.write(address_of(name),data)

    def run_mips(self,name,args):
        cpu=self.cpu;cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
        cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=address_of(name)
        for i,a in enumerate(args):
            if i<4:cpu.r[4+i]=a&0xFFFFFFFF
            else:self.put(STACK+16+(i-4)*4,a)
        services={address_of(n):i for i,n in enumerate(SERVICES)}
        for _ in range(10000):
            if cpu.pc==STOP:
                for r in (*range(16,24),28,30):self.assertEqual(0xBEEF0000+r,cpu.r[r])
                self.assertEqual(STACK,cpu.r[29])
                return cpu.r[2]&0xFFFFFFFF
            if cpu.pc not in services:cpu.step();continue
            index=services[cpu.pc];resume=cpu.r[31]
            result=self.service(index,cpu.r[4:7],False)
            for r in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[r]=0xDEAD0000+r
            cpu.r[2]=result;cpu.pc=resume
        self.fail('MIPS instruction limit')

    def test_real_allocators_and_loader_callers(self):
        callback_type=C.CFUNCTYPE(C.c_uint32,C.c_uint32,C.POINTER(C.c_size_t))
        failures=[]
        def observe(index,args):
            try:return self.service(index,args,True)
            except BaseException as error:failures.append(error);return 0
        callback=callback_type(observe);self.library.set_observer(callback)
        names=(*ALLOCATORS,'InitMenuArenas','LoadOpeningMovieTimStep','LoadModeMenuAssetsStep','InitRect16Row')
        for linked in (False,True):
            self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x4000000)
            self.write(0x80010000,self.original)
            if linked:
                for address,data in self.linked:self.write(address,data)
            allowed=[(address_of(n),4)for n in POINTERS]
            allowed += [(address_of(n),size)for n,size in SCALARS]
            allowed += [(address_of('MenuArenas'),8),(STACK-256,256)]
            allowed += [(a,SIZE)for a in ARENAS]
            def check_store(cpu,access,address,size,value,user):
                p=address&0x1FFFFFFF
                self.assertTrue(any((a&0x1FFFFFFF)<=p and p+size<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(address),size))
            self.cpu=Cpu(self.u,0,100,101,102,check_store)
            for self.case in range(48):
                for name in names:
                    with self.subTest(linked=linked,case=self.case,function=name):
                        self.initialize(False);self.events=[]
                        args=[ARENAS[0]+128]if name in ALLOCATORS else[]
                        if name=='InitRect16Row':
                            values=(-32768,-1,0,1,32767,1234)
                            args=[self.case%8-2]+[values[(self.case+i)%6]for i in range(6)]
                        expected_result=self.run_mips(name,args);expected_events=self.events;expected_state=self.snapshot(False)
                        self.initialize(True);self.events=[]
                        native_args=[self.native(a)for a in args]if name in ALLOCATORS else args
                        result=getattr(self.library,name)(*native_args)
                        if failures:raise failures[0]
                        self.assertEqual(expected_events,self.events)
                        self.assertEqual(expected_state,self.snapshot(True))
                        if name=='InitRect16Row':
                            if args[0]<4:
                                record=self.pointer_cells['Rect16Rows'].value+args[0]*16
                                self.assertEqual(struct.pack('<7h',args[6],0,*args[1:6]),C.string_at(record,14))
                            continue
                        if name!='InitMenuArenas':self.assertEqual(expected_result,result&0xFFFFFFFF)
                        else:
                            base=ARENAS[0]+64+0x2D000
                            expected=(base,base+280,base+344,base+456,base+484)
                            actual=tuple(self.pal(self.pointer_cells[n].value)for n in ('BadgeRows','Rect16Rows','GlyphRows','ImageRows','UiRows'))
                            self.assertEqual(expected,actual)
                            self.assertEqual(base+1924,self.pal(self.pointer_cells['MenuBaseLoadCursor'].value))
                            self.assertEqual(base+1924+0x19000,self.pal(self.pointer_cells['LoadCursor'].value))


if __name__=='__main__':unittest.main()

# Native glyph rows retain full pointer cells; the historical PAL oracle and
# arena input/output schema remain unchanged.
from menu_glyph_arena_transport import install as _install_glyph_arena_transport
_install_glyph_arena_transport(MenuArenaPipelineTest, globals())
