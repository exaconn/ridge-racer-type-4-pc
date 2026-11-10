"""Use the function registry for automatic aliases left in generated assembly.

Splat may name a definition in a grouped hasm segment correctly while references
from another segment still use func_ADDRESS. Resolve those aliases to the same
registered symbol, including references with an interior offset.
"""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[2]
ALIAS = re.compile(r'\bfunc_([0-9A-Fa-f]{8})\b')


def normalize(source, functions):
    return ALIAS.sub(lambda m: functions.get(int(m[1], 16), m[0]), source)


def main():
    version = sys.argv[1] if len(sys.argv) > 1 else 'PAL'
    functions = {int(address, 16): name for name, address, size in
                 map(str.split, (ROOT/f'configs/{version}/functions.txt').read_text().splitlines())}
    changed = 0
    for path in sorted((ROOT/f'asm/{version}/main').glob('*.s')):
        source = path.read_text();updated = normalize(source, functions)
        if updated != source:
            path.write_text(updated);changed += 1
    print('Assembly files normalized to registered function names:', changed)


if __name__ == '__main__':
    main()
