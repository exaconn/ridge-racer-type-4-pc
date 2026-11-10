"""Find game function definitions, including old-style K&R parameters.

Require an opening body brace so prototypes and calls are never counted.
"""
import json
import re
from pathlib import Path

_NONCODE = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
_DEFINITION = re.compile(
    r"^[ \t]*(?:[A-Za-z_]\w*[ \t\n*]+)+"
    r"([A-Za-z_]\w*)\s*\(([^(){};]*)\)\s*"
    r"((?:[A-Za-z_][^(){};]*;\s*)*)\{", re.M)


_ROOT = Path(__file__).resolve().parents[2]
_INDEX_KEY = None
_INDEX = None


def _symbol_index():
    """Index one registry revision; reload when either input file changes."""
    global _INDEX_KEY, _INDEX
    listing = _ROOT/'configs/PAL/functions.txt'
    registry = _ROOT/'configs/PAL/sym.main.txt'
    overlays = sorted((_ROOT/'configs/PAL/overlays').glob('*.json'))
    revisions = [(path, path.stat()) for path in (listing, registry, *overlays)]
    key = tuple((str(path), stat.st_dev, stat.st_ino, stat.st_mtime_ns, stat.st_size)
                for path, stat in revisions)
    if key != _INDEX_KEY:
        functions, symbols, names = {}, {}, {}
        for line in listing.read_text().splitlines():
            fields = line.split()
            if fields:
                functions.setdefault(fields[0], int(fields[1], 16))
        for line in registry.read_text().splitlines():
            match = re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
            if not match:
                continue
            name, address = match[1], int(match[2], 16)
            symbols.setdefault(name, address)
            if re.match(r';\s*//\s*type:func', line[match.end()-1:]) and not re.fullmatch(r'func_[0-9A-F]{8}', name):
                names.setdefault(address, name)
        for path in overlays:
            config = json.loads(path.read_text())
            for fn in config.get('functions', []):
                if fn.get('symbol'):
                    symbols.setdefault(fn['symbol'], fn['address'])
            for sym, addr in config.get('globals', {}).items():
                symbols.setdefault(sym, addr)
        _INDEX = functions, symbols, names
        _INDEX_KEY = key
    return _INDEX


def known_functions():
    """every function the game defines: auto names (func_XXXXXXXX) and names registered in sym.main.txt"""
    return _symbol_index()[0].keys()


def function_definitions(source):
    source = _NONCODE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), source)
    names = []
    known = known_functions()
    for match in _DEFINITION.finditer(source):
        if match[1] not in known and not re.fullmatch(r'func_[0-9A-F]{8}', match[1]):
            continue
        # Declarations between ')' and '{' are valid only for K&R name lists.
        if match[3] and not re.fullmatch(r"\s*[A-Za-z_]\w*(?:\s*,\s*[A-Za-z_]\w*)*\s*", match[2]):
            continue
        names.append(match[1])
    return names


def source_owners(directory):
    """Index functions by their owning TU, independently of the filename."""
    owners = {}
    for source in sorted(Path(directory).rglob("*.c")):
        for name in function_definitions(source.read_text()):
            if name in owners:
                raise ValueError(f"{name}: defined in both {owners[name]} and {source}")
            owners[name] = source
    return owners


def definition_source(source, name):
    """Extract one real definition, including K&R parameters, without TU aliases.

    Native fixtures supply their own public types and service declarations;
    unrelated function bodies or scoped macros from the merged TU stay out.
    """
    clean = _NONCODE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), source)
    for match in _DEFINITION.finditer(clean):
        if match[1] != name:
            continue
        if match[3] and not re.fullmatch(r"\s*[A-Za-z_]\w*(?:\s*,\s*[A-Za-z_]\w*)*\s*", match[2]):
            continue
        depth, end = 1, match.end()
        while end < len(clean) and depth:
            depth += (clean[end] == '{') - (clean[end] == '}')
            end += 1
        if depth:
            raise ValueError(f'Unclosed definition: {name}')
        return source[match.start():end] + '\n'
    raise ValueError(f'Missing definition: {name}')


def address_of(name):
    """address of a function by auto name (func_XXXXXXXX) or registered name; lets tests keep working when a name is registered later"""
    if re.fullmatch(r'func_[0-9A-Fa-f]{8}', name):
        return int(name[-8:], 16)
    functions, symbols, _ = _symbol_index()
    if name in functions:
        return functions[name]
    if re.fullmatch(r'(?:ovl\d+_)?(?:func|D)_[0-9A-Fa-f]{8}', name):
        return int(name[-8:], 16)   # overlay names such as ovl659_func_80123456
    if name in symbols:
        return symbols[name]
    raise KeyError(name)


def name_at(address):
    """current name of the function at an address (hex string or int): the registered name, else func_XXXXXXXX"""
    a = int(address, 16) if isinstance(address, str) else address
    return _symbol_index()[2].get(a, 'func_%08X' % a)
