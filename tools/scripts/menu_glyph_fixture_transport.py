"""Bind unchanged legacy glyph Worlds to complete canonical native row owners.

Production function text is inserted verbatim. Only fixture type bindings,
entry transport and controlled-service boundaries are instrumented. The old
World definitions, models, case generators and PRNG streams remain literal.
"""
from pathlib import Path
import re
from overlay_units import function_extent

ROOT = Path(__file__).resolve().parents[2]
MASK = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)


def _wire_declaration_position(prefix):
    position = prefix.index('MenuGlyphRow1C')
    code = MASK.sub(lambda m: re.sub(r'[^\n]', ' ', m[0]), prefix)
    stack = []
    for at, char in enumerate(code[:position]):
        if char == '{':
            stack.append(at)
        elif char == '}':
            stack.pop()
    start = stack[0] if stack else position
    return prefix.rfind('\n', 0, start)+1


def _service_boundary(source, name, condition):
    start, end = function_extent(source, name)
    definition = source[start:end]
    opening = definition.index('{')
    signature = definition[:opening]
    match = re.search(r'\b'+re.escape(name)+r'\s*\((.*)\)\s*$', signature, re.S)
    assert match, name
    params = match[1].strip()
    arguments = []
    if params and params != 'void':
        for parameter in params.split(','):
            parameter = re.sub(r'\[[^]]*\]', '', parameter).strip()
            arguments.append(re.search(r'(\w+)\s*$', parameter)[1])
    call = 'rr_wire_impl_'+name+'('+','.join(arguments)+')'
    return_type = re.sub(r'\b(?:static|inline|__inline__)\b', '', signature[:match.start()]).strip()
    implementation = re.sub(r'\b'+re.escape(name)+r'(?=\s*\()', 'rr_wire_impl_'+name, definition, count=1)
    assert implementation[implementation.index('{'):] == definition[opening:]
    if return_type == 'void':
        invoke = call+';'
        result = ''
    else:
        invoke = return_type+' rr_result = '+call+';'
        result = 'return rr_result;'
    wrapper = signature+'{\n if ('+condition+') rr_test_glyph_store();\n '+invoke+'\n if ('+condition+') rr_test_glyph_load();\n '+result+'\n}\n'
    return source[:start]+implementation+'\n'+wrapper+source[end:]


def transport(source, body, entry, array, count, callbacks):
    """Instrument one generated C fixture, preserving the supplied actual body."""
    assert source.count(body) == 1
    prefix, suffix = source.split(body)
    old_root = re.search(r'^#define\s+GlyphRows\s+([^\n]+)', prefix, re.M)
    assert old_root, entry
    root_expression = old_root[1].strip()
    at = _wire_declaration_position(prefix)
    header = str(ROOT/'tools/scripts/menu_glyph_wire.h').replace('\\', '\\\\').replace('"', '\\"')
    prefix = prefix[:at]+'#include "'+header+'"\n#define MenuGlyphRow1C FrozenMenuGlyphRow1C\n'+prefix[at:]
    native_body = ('\n#undef MenuGlyphRow1C\n#undef GlyphRows\n'
                   '#define GlyphRows rr_test_glyph_root('+root_expression+')\n'+body+
                   '\n#undef GlyphRows\n#define GlyphRows '+root_expression+'\n'
                   '#define MenuGlyphRow1C FrozenMenuGlyphRow1C\n')
    entry_transport = ('\nstatic void rr_fixture_glyph_begin(void){\n'
                       ' rr_test_glyph_reset();\n rr_test_glyph_register('+array+','+count+');\n}\n'
                       'static void rr_fixture_glyph_end(void){rr_test_glyph_store();rr_test_glyph_check();}\n'
                       '#define '+entry+'(...) (rr_fixture_glyph_begin(),'+entry+'(__VA_ARGS__),rr_fixture_glyph_end())\n')
    result = prefix+native_body+entry_transport+suffix
    for name, condition in callbacks:
        result = _service_boundary(result, name, condition)
    assert result.count(body) == 1, 'Production body changed during fixture transport'
    return result
