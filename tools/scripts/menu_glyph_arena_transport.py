"""Transport the frozen menu-arena oracle across the native glyph stride.

The original Python test/World and MIPS models remain unchanged. Native C uses
expanded real allocations; complete pointer cells are bound by C, then mapped
back to the old PAL record/arena schema for comparison. Actual allocator byte
counts are also checked by the new glyph-owner pipeline fixture.
"""
import ctypes as C
import struct

EXTRA_PREFIX = r'''
#include "@WIRE@"
void *rr_glyph_arena_allocate(size_t size){
 u8 *p=malloc(size+32);assert(p);memset(p,0xA5,16);memset(p+16+size,0xA5,16);
 assert((uintptr_t)p>UINT32_MAX);return p+16;
}
void rr_glyph_arena_free(void *p){free((u8*)p-16);}
void rr_glyph_arena_check(const void *p,size_t size){
 rr_glyph_check_guard((const u8*)p-16);rr_glyph_check_guard((const u8*)p+size);rr_test_glyph_check();
}
void rr_glyph_arena_reset_images(void){rr_test_glyph_reset();}
void rr_glyph_arena_load_rows(void *output,const void *input,size_t count){
 TestNativeMenuGlyphRow *rows=output;const FrozenMenuGlyphRow1C *wire=input;
 for(size_t i=0;i<count;i++){
  u32 token;memcpy(&token,&wire[i].phase18,4);memcpy(&rows[i],&wire[i],24);
  rows[i].imageBase18=rr_glyph_bind_image(token);
 }
}
void rr_glyph_arena_store_rows(void *output,const void *input,size_t count){
 FrozenMenuGlyphRow1C *wire=output;const TestNativeMenuGlyphRow *rows=input;
 for(size_t i=0;i<count;i++){
  u32 token=rr_glyph_image_token(rows[i].imageBase18);
  memcpy(&wire[i],&rows[i],24);memcpy(&wire[i].phase18,&token,4);
 }
}
'''


class _Library:
    def __init__(self, library, transport):
        self.library, self.transport = library, transport

    def __getattr__(self, name):
        value = getattr(self.library, name)
        if name not in self.transport.operations:
            return value
        return lambda *args: self.transport.invoke(name, value, args)


class _ArenaTransport:
    def __init__(self, cls, module):
        self.cls, self.module = cls, module
        self.size = module['SIZE']; self.capacity = self.size+4096
        self.addresses = module['ARENAS']; self.section = None; self.owner = None
        self.operations = {*module['ALLOCATORS'], 'InitMenuArenas', 'LoadOpeningMovieTimStep',
                           'LoadModeMenuAssetsStep', 'InitRect16Row'}
        self.lib = cls.library
        for name, args, result in (
            ('rr_glyph_arena_allocate', [C.c_size_t], C.c_void_p),
            ('rr_glyph_arena_free', [C.c_void_p], None),
            ('rr_glyph_arena_check', [C.c_void_p,C.c_size_t], None),
            ('rr_glyph_arena_reset_images', [], None),
            ('rr_glyph_arena_load_rows', [C.c_void_p,C.c_void_p,C.c_size_t], None),
            ('rr_glyph_arena_store_rows', [C.c_void_p,C.c_void_p,C.c_size_t], None)):
            fn = getattr(self.lib, name); fn.argtypes=args; fn.restype=result
        self.buffers = [self.lib.rr_glyph_arena_allocate(self.capacity) for _ in self.addresses]
        self.last_clear_glyph = False

    def close(self):
        for pointer in self.buffers:
            self.lib.rr_glyph_arena_free(pointer)

    def native(self, address):
        if not address:
            return None
        for bank, (base, pal) in enumerate(zip(self.buffers, self.addresses)):
            if pal <= address <= pal+self.size:
                offset=address-pal
                if self.section and bank==self.section[0]:
                    _, start, count=self.section; end=start+count*28
                    if start <= offset < end:
                        row, field=divmod(offset-start,28); offset=start+row*32+field
                    elif offset >= end:
                        offset += count*4
                return base+offset
        return self.old_native(self.owner, address)

    def pal(self, pointer):
        if not pointer:
            return 0
        for bank,(base,pal) in enumerate(zip(self.buffers,self.addresses)):
            if base <= pointer <= base+self.capacity:
                offset=pointer-base
                if self.section and bank==self.section[0]:
                    _,start,count=self.section;end=start+count*32
                    if start <= offset < end:
                        row,field=divmod(offset-start,32)
                        assert field<28, 'No PAL address denotes the upper native pointer half'
                        offset=start+row*28+field
                    elif offset >= end:
                        offset -= count*4
                assert offset <= self.size
                return pal+offset
        return self.old_pal(self.owner, pointer)

    def initialize(self, owner):
        self.owner=owner;self.section=None;self.last_clear_glyph=False
        self.lib.rr_glyph_arena_reset_images()

    def import_buffers(self):
        for bank,(native,wire) in enumerate(zip(self.buffers,self.owner.buffers)):
            C.memset(native,0xA7,self.capacity)
            source=C.addressof(wire)
            if self.section and bank==self.section[0]:
                _,start,count=self.section;old_end=start+count*28;new_end=start+count*32
                C.memmove(native,source,start)
                self.lib.rr_glyph_arena_load_rows(native+start,source+start,count)
                C.memmove(native+new_end,source+old_end,self.size-old_end)
            else:
                C.memmove(native,source,self.size)

    def export_buffers(self):
        for bank,(native,wire) in enumerate(zip(self.buffers,self.owner.buffers)):
            output=C.addressof(wire);used=self.size
            if self.section and bank==self.section[0]:
                _,start,count=self.section;old_end=start+count*28;new_end=start+count*32
                C.memmove(output,native,start)
                self.lib.rr_glyph_arena_store_rows(output+start,native+start,count)
                C.memmove(output+old_end,native+new_end,self.size-old_end);used+=count*4
            else:
                C.memmove(output,native,self.size)
            assert C.string_at(native+used,self.capacity-used)==bytes([0xA7])*(self.capacity-used)
            self.lib.rr_glyph_arena_check(native,self.capacity)

    def invoke(self, name, function, args):
        self.last_clear_glyph=False
        pointers=[self.pal(cell.value)for cell in self.owner.pointer_cells.values()]
        arena_values=[self.pal(value)for value in self.owner.arena_cells]
        incoming=[self.pal(args[0])]if name in self.module['ALLOCATORS']else None
        count=int.from_bytes(bytes(self.owner.scalar_cells['GlyphRowCount']),'little')
        assert 0 <= count <= 32
        window=None
        if name=='AllocGlyphRows':
            start=incoming[0]
            window=(start,count)
        elif name=='InitMenuArenas':
            directory=self.pal(self.owner.pointer_cells['CdDirectoryBuffer'].value)
            window=(directory+0x2D000+10*28+4*16,4)
        if window:
            start,count=window
            for bank,address in enumerate(self.addresses):
                if address <= start and start+count*28 <= address+self.size:
                    self.section=(bank,start-address,count);break
            else:
                raise AssertionError(('glyph allocation outside full arenas',window))
            self.import_buffers()
            for cell,value in zip(self.owner.pointer_cells.values(),pointers):cell.value=self.native(value)
            for i,value in enumerate(arena_values):self.owner.arena_cells[i]=self.native(value)
            if incoming:args=(self.native(incoming[0]),)
        result=function(*args)
        self.export_buffers()
        if name=='AllocGlyphRows':
            assert result==count*32, 'Actual native allocator must reserve full pointer cells'
            return count*28
        return result

    def service_args(self, operation, args):
        values=list(args[:3])
        if operation==0:
            self.last_clear_glyph=False
            if self.section:
                bank,start,count=self.section
                self.last_clear_glyph=(values[0]==self.buffers[bank]+start and values[1]==count*32)
                if self.last_clear_glyph:values[1]=count*28
        elif operation==1 and self.last_clear_glyph:
            count=self.section[2]
            if values[1]==count*32:values[1]=count*28
            self.last_clear_glyph=False
        return values


def install(cls, module):
    """Keep the original class/model literal and attach only its native codec."""
    header=str(module['ROOT']/'tools/scripts/menu_glyph_wire.h').replace('\\','\\\\').replace('"','\\"')
    module['PREFIX'] += EXTRA_PREFIX.replace('@WIRE@',header)
    original_setup=cls.setUpClass.__func__
    old_native,old_pal=cls.native,cls.pal
    old_initialize,old_snapshot,old_service=cls.initialize,cls.snapshot,cls.service

    def setup(current):
        original_setup(current)
        transport=_ArenaTransport(current,module);current._glyph_transport=transport
        transport.old_native=old_native;transport.old_pal=old_pal
        current.addClassCleanup(transport.close)
        current.library=_Library(transport.lib,transport)

    def native(self,address):return self._glyph_transport.native(address)
    def pal(self,pointer):return self._glyph_transport.pal(pointer)

    def initialize(self,native):
        transport=self._glyph_transport
        if native:transport.initialize(self)
        old_initialize(self,native)
        if native:transport.import_buffers()

    def snapshot(self,native):
        if native:self._glyph_transport.export_buffers()
        return old_snapshot(self,native)

    def service(self,operation,args,native):
        if native:args=self._glyph_transport.service_args(operation,args)
        return old_service(self,operation,args,native)

    cls.setUpClass=classmethod(setup)
    cls.native=native;cls.pal=pal;cls.initialize=initialize;cls.snapshot=snapshot;cls.service=service
