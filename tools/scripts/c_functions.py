"""Find game function definitions, including old-style K&R parameters.

Require an opening body brace so prototypes and calls are never counted.
"""
import re
from pathlib import Path

_NONCODE = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
_DEFINITION = re.compile(
    r"^[ \t]*(?:[A-Za-z_]\w*[ \t\n*]+)+"
    r"(func_[0-9A-F]{8})\s*\(([^(){};]*)\)\s*"
    r"((?:[A-Za-z_][^(){};]*;\s*)*)\{", re.M)


def function_definitions(source):
    source = _NONCODE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), source)
    names = []
    for match in _DEFINITION.finditer(source):
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
