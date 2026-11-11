"""Bind literal legacy fixtures to a complete native selection owner.

Worlds, models, cases and PRNG streams remain literal. Entry/service adapters
transport their old byte views; native pointer aliases map between the two real
allocations at full width. Production function text is preserved verbatim.
"""
import re
from overlay_units import function_extent as _line_extent
from c_functions import _NONCODE


def function_extent(source,name):
    """A controlled fixture may put several service definitions on one line."""
    start,end=_line_extent(source,name)
    code=_NONCODE.sub(lambda m:re.sub(r'[^\n]',' ',m[0]),source)
    definition=re.search(r'\b'+re.escape(name)+r'\s*\([^(){};]*\)\s*\{',code[start:end])
    assert definition,name
    name_at=start+definition.start()
    # Discard preceding complete declarations/definitions on that same line.
    start=max(start,code.rfind('}',start,name_at)+1,code.rfind(';',start,name_at)+1)
    while start<name_at and code[start].isspace():start+=1
    return start,end


def _signature(definition,name):
    signature=definition[:definition.index('{')]
    match=re.search(r'\b'+re.escape(name)+r'\s*\((.*)\)\s*$',signature,re.S)
    assert match,name
    params=match[1].strip();args=[];pointer=[]
    if params and params!='void':
        for parameter in params.split(','):
            assert '...'not in parameter and '('not in parameter
            is_pointer='*'in parameter or '['in parameter
            parameter=re.sub(r'\[[^]]*\]','',parameter).strip()
            args.append(re.search(r'(\w+)\s*$',parameter)[1]);pointer.append(is_pointer)
    ret=re.sub(r'\b(?:static|inline|__inline__)\b','',signature[:match.start()]).strip()
    return signature,args,pointer,ret


def _boundary(source,name,condition):
    start,end=function_extent(source,name);definition=source[start:end]
    signature,args,pointers,ret=_signature(definition,name)
    implementation=re.sub(r'\b'+re.escape(name)+r'(?=\s*\()','rr_selection_impl_'+name,definition,count=1)
    assert implementation[implementation.index('{'):]==definition[definition.index('{'):]
    passed=[('(rr_selection_active?rr_selection_wire_pointer('+a+'):'+a+')')if p else a for a,p in zip(args,pointers)]
    call='rr_selection_impl_'+name+'('+','.join(passed)+')'
    invoke=call+';'if ret=='void'else ret+' rr_selection_result='+call+';'
    returned=''if ret=='void'else ('return rr_selection_active?rr_selection_native_pointer(rr_selection_result):rr_selection_result;'if '*'in ret else 'return rr_selection_result;')
    wrapper=signature+'{\n const int rr_selection_active=('+condition+');\n if(rr_selection_active&&rr_selection_depth++==0)rr_selection_store();\n '+invoke+'\n if(rr_selection_active&&--rr_selection_depth==0)rr_selection_load();\n '+returned+'\n}\n'
    return source[:start]+implementation+'\n'+wrapper+source[end:]


POINTERS=r'''
static void *rr_selection_native_pointer(const void *p){
 uintptr_t address=(uintptr_t)p,base=(uintptr_t)rr_selection_legacy;
 if(address>=base&&address-base<=rr_selection_extent)return rr_selection.value.bytes+(address-base);
 return (void*)p;
}
static void *rr_selection_wire_pointer(const void *p){
 uintptr_t address=(uintptr_t)p,base=(uintptr_t)rr_selection.value.bytes;
 if(address>=base&&address-base<=32){assert(address-base<=rr_selection_extent);return rr_selection_legacy+(address-base);}
 return (void*)p;
}
'''


def transport(source,body,entries,legacy,extent,callbacks=(),pointers=()):
    """Bind a legacy record view without changing its model or case generation."""
    assert source.count(body)==1
    if entries is None:
        entries=re.findall(r'^\s*(?:void|s32|u32|int)\s+(ovl\d+_\w+)\s*\([^{};]*\)\s*\{',body,re.M)
        assert entries,'No overlay entries found'
    if isinstance(entries,str):entries=[entries]
    prefix,suffix=source.split(body)
    root=re.search(r'^#define\s+SelectionRows\s+([^\n]+)',prefix,re.M)
    assert root,entries
    old_root=root[1].strip()
    uses_pointers=bool(pointers)
    for name,_ in callbacks:
        start,end=function_extent(source,name);_,_,args,ret=_signature(source[start:end],name)
        uses_pointers|=any(args)or '*'in ret
    support='\n#undef SelectionRows\n#define SelectionRows rr_selection_declaration\n#include "game/data/selection.h"\n#undef SelectionRows\n#define SelectionRows '+old_root+'\n'+r'''
static struct {u32 before[4];CurrentSelection value;u32 after[4];} rr_selection;
static CurrentSelection rr_selection_initial;
static u8 *rr_selection_legacy;
static size_t rr_selection_extent;
static unsigned rr_selection_depth;
@POINTERS@
static void rr_selection_load(void){memcpy(rr_selection.value.bytes,rr_selection_legacy,rr_selection_extent);}
static void rr_selection_store(void){memcpy(rr_selection_legacy,rr_selection.value.bytes,rr_selection_extent);}
static void rr_selection_begin(void *legacy,size_t extent){
 assert(!rr_selection_depth&&extent>=4&&extent<=32&&extent%4==0);
 memset(&rr_selection,0xA7,sizeof(rr_selection));rr_selection_legacy=legacy;rr_selection_extent=extent;rr_selection_load();
 rr_selection_initial=rr_selection.value;
 assert(sizeof(CurrentSelection)==32&&sizeof(void*)==8&&(uintptr_t)&rr_selection.value>UINT32_MAX);
}
static void rr_selection_end(void){
 assert(!rr_selection_depth);rr_selection_store();
 for(unsigned i=0;i<4;i++)assert(rr_selection.before[i]==0xA7A7A7A7U&&rr_selection.after[i]==0xA7A7A7A7U);
 assert(!memcmp(rr_selection.value.bytes+rr_selection_extent,rr_selection_initial.bytes+rr_selection_extent,32-rr_selection_extent));
}
'''.replace('@POINTERS@',POINTERS if uses_pointers else '')
    before='\n#undef SelectionRows\n#define SelectionRows rr_selection.value\n';after='\n#undef SelectionRows\n#define SelectionRows '+old_root+'\n'
    for name,ctype in pointers:
        binding=re.search(r'^#define\s+'+re.escape(name)+r'\s+([^\n]+)',prefix,re.M)
        assert binding,name
        before+='\n#undef '+name+'\n#define '+name+' (('+ctype+')rr_selection_native_pointer('+binding[1]+'))\n'
        after+='\n#undef '+name+'\n#define '+name+' '+binding[1]+'\n'
    bound=before+body+after;result=prefix+support+bound+suffix
    if 'rr_fixture_glyph_begin'in result:
        # Compose entry adapters; neither literal World nor existing call sites
        # need another replacement or a different deterministic input stream.
        for name,call in [('rr_fixture_glyph_begin','rr_selection_begin('+legacy+','+str(extent)+');'),('rr_fixture_glyph_end','rr_selection_end();')]:
            start,end=function_extent(result,name);opening=result.index('{',start,end)+1
            result=result[:opening]+call+result[opening:]
    else:
        wrappers=''
        for name in entries:
            start,end=function_extent(body,name);signature,args,_,ret=_signature(body[start:end],name)
            wrapper_signature=re.sub(r'\b'+re.escape(name)+r'(?=\s*\()','rr_selection_entry_'+name,signature,count=1)
            call=name+'('+','.join(args)+')';invoke=call+';'if ret=='void'else ret+' rr_selection_result='+call+';'
            returned=''if ret=='void'else 'return rr_selection_result;'
            wrappers+='\n'+wrapper_signature+'{rr_selection_begin('+legacy+','+str(extent)+');'+invoke+'rr_selection_end();'+returned+'}\n#define '+name+' rr_selection_entry_'+name+'\n'
        at=result.index(bound)+len(bound);result=result[:at]+wrappers+result[at:]
    for name,condition in callbacks:result=_boundary(result,name,condition)
    # Some controlled services precede the actual body in old C harnesses.
    # Their adapter declarations need no legacy World or production types.
    if callbacks:
        declarations='static unsigned rr_selection_depth;\nstatic void rr_selection_store(void);\nstatic void rr_selection_load(void);\n'
        for name in ('rr_selection_wire_pointer','rr_selection_native_pointer'):
            if result.count(name+'(')>1:
                declarations+='static void *'+name+'(const void *p);\n'
        result=declarations+result
    for name in ('rr_selection_wire_pointer','rr_selection_native_pointer'):
        if result.count(name+'(')==1:
            start,end=function_extent(result,name);result=result[:start]+result[end:]
    assert result.count(body)==1,'Production body changed during fixture transport'
    return result
