"""Source ownership for overlay translation units and legacy function files."""
from functools import lru_cache
from pathlib import Path
import json
import re
import subprocess

from elftools.elf.elffile import ELFFile
from c_functions import source_owners

ROOT = Path(__file__).resolve().parents[2]

OVERLAY_NAMES = {
    659: 'grand_prix',
    660: 'time_attack',
    661: 'versus',
    662: 'garage',
    663: 'records',
    665: 'options',
    666: 'extra_trial',
    667: 'serial_link',
    668: 'movie_playback',
}
OVERLAY_INDICES = {name: index for index, name in OVERLAY_NAMES.items()}
OVERLAY_INDICES.update({
    'race_results': 659,
    'time_attack_course_select': 660,
    'versus_select': 661,
    'extra_course_select': 666,
})


def overlay_index(overlay):
    if isinstance(overlay, int):
        return overlay
    text = str(overlay)
    return int(text) if text.isdigit() else OVERLAY_INDICES[text]


def overlay_name(overlay):
    index = overlay_index(overlay)
    return OVERLAY_NAMES.get(index, str(index))


@lru_cache(maxsize=None)
def manifest(overlay, root=ROOT):
    index = overlay_index(overlay)
    name = overlay_name(index)
    for stem in (name, str(index)):
        path = root / f'configs/PAL/overlays/{stem}.json'
        if path.exists():
            return json.loads(path.read_text())
    return None


def function_owner(overlay, address, root=ROOT):
    index = overlay_index(overlay)
    name = overlay_name(index)
    config = manifest(index, root)
    if config:
        for function in config['functions']:
            if function['address'] == address and function.get('source'):
                return root / function['source'], function.get(
                    'symbol', f'ovl{index}_func_{address:08X}')
    named = root / f'src/overlays/PAL/{name}/func_{address:08X}.c'
    if named.exists():
        return named, f'ovl{index}_func_{address:08X}'
    return (root / f'src/overlays/PAL/{index}/func_{address:08X}.c',
            f'ovl{index}_func_{address:08X}')


def symbol_addresses(root=ROOT):
    """Link descriptive C symbols to the original overlay ABI addresses."""
    result = {}
    for line in (root / 'configs/PAL/sym.main.txt').read_text().splitlines():
        m = re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m:
            result[m.group(1)] = int(m.group(2), 16)
    for path in (root / 'configs/PAL/overlays').glob('*.json'):
        config = json.loads(path.read_text())
        for function in config.get('functions', []):
            if function.get('symbol'):
                result[function['symbol']] = function['address']
        result.update(config.get('globals', {}))
    return result


def function_extent(source, symbol):
    """Find a complete C definition, ignoring braces inside comments/literals."""
    pattern = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
    code = pattern.sub(lambda match: re.sub(r'[^\n]', ' ', match[0]), source)
    definition = re.search(r'\b' + re.escape(symbol) + r'\s*\([^(){};]*\)\s*\{', code)
    if definition is None:
        raise ValueError(f'Missing function definition: {symbol}')
    start = code.rfind('\n', 0, definition.start()) + 1
    opening = code.index('{', definition.start())
    depth = 1
    position = opening + 1
    while depth and position < len(code):
        depth += (code[position] == '{') - (code[position] == '}')
        position += 1
    if depth:
        raise ValueError(f'Unclosed function definition: {symbol}')
    return start, position


def resolve_c_source_path(path):
    """Resolve a historical function path to its current translation unit."""
    path = Path(path)
    if path.exists():
        return path
    match = re.search(r'src/overlays/PAL/([^/]+)/func_([0-9A-Fa-f]{8})\.c$', str(path))
    if not match:
        main = re.search(r'src/main/PAL/(func_[0-9A-Fa-f]{8})\.c$', str(path))
        if main:
            owner = source_owners(ROOT / 'src/main/PAL').get(main[1])
            if owner is not None:
                return owner
        raise FileNotFoundError(path)
    return function_owner(overlay_index(match[1]), int(match[2], 16))[0]


def read_c_source(path):
    """Read fixtures from the real TU rather than maintaining duplicate C bodies.

    Existing fixtures name a function by its original PAL address. Once it has
    moved into a TU, return that definition; its fixture supplies the interface.
    Other paths retain normal Path.read_text behavior.
    """
    path = Path(path)
    if path.exists():
        return path.read_text()
    owner = resolve_c_source_path(path)
    match = re.search(r'src/main/PAL/(func_[0-9A-Fa-f]{8})\.c$', str(path))
    if match:
        symbol = match[1]
    else:
        match = re.search(r'src/overlays/PAL/([^/]+)/func_([0-9A-Fa-f]{8})\.c$', str(path))
        _, symbol = function_owner(overlay_index(match[1]), int(match[2], 16))
    source = owner.read_text()
    start, end = function_extent(source, symbol)
    return source[start:end] + '\n'


def link_unit(index, source, functions, rodata_spans, output, validate_sections, root=ROOT):
    """Compile one actual TU once and link its complete text/rodata layout."""
    members = [(address, size, function_owner(index, address, root)[1])
               for overlay, address, size in functions
               if overlay == index and function_owner(index, address, root)[0] == source]
    members.sort()
    if not members:
        raise ValueError(f'No function ownership for {source}')
    for left, right in zip(members, members[1:]):
        if left[0] + left[1] != right[0]:
            raise ValueError(f'Noncontiguous TU: {source}')
    tables = [rodata_spans[index, address] for address, _, _ in members
              if (index, address) in rodata_spans]
    if len(tables) > 1:
        raise ValueError(f'Multiple separate rodata spans need an explicit layout: {source}')
    rodata = tables[0] if tables else None
    name = f'ovl{index}_{source.stem}'
    obj, elf, script = (output / (name + suffix) for suffix in ('.o', '.elf', '.ld'))
    subprocess.run([str((root / 'tools/scripts/cc.sh').resolve()), str(source), str(obj)],
                   check=True, capture_output=True)
    definitions = []
    addresses = symbol_addresses(root)
    with obj.open('rb') as stream:
        parsed = ELFFile(stream)
        validate_sections(parsed, source, rodata)
        for symbol in parsed.get_section_by_name('.symtab').iter_symbols():
            if symbol['st_shndx'] == 'SHN_UNDEF' and symbol.name:
                match = re.fullmatch(r'(?:ovl\d+_)?(?:D|func)_([0-9A-Fa-f]{8})', symbol.name)
                if match:
                    address = int(match[1], 16)
                elif symbol.name in addresses:
                    address = addresses[symbol.name]
                else:
                    raise ValueError(f'Unknown address of {symbol.name}')
                definitions.append(f'{symbol.name} = 0x{address:X};')
    sections = f'.text 0x{members[0][0]:X} : SUBALIGN(4) {{ *(.text) }}'
    if rodata:
        sections += f' .rodata 0x{rodata[0]:X} : SUBALIGN(4) {{ *(.rodata) }}'
    sections += ' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }'
    script.write_text('\n'.join(definitions) + '\nSECTIONS { ' + sections + ' }\n')
    subprocess.run([str(root / 'tools/toolchain/bin/mipsel-none-elf-ld'), '-EL',
                    '-T', str(script), '-o', str(elf), str(obj)],
                   check=True, capture_output=True)
    return elf, rodata
