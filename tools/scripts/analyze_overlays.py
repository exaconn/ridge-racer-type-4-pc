#!/usr/bin/env python3
"""Inventory PAL R4.BIN; extract code overlays without modifying build inputs.

Text spans below are static-analysis candidates, not a finalized section map.
Return instructions are evidence for code, never a function-count metric.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import yaml

ROOT = Path(__file__).resolve().parents[2]
BASE = 0x80114878
# First instruction through final observed return delay slot (end exclusive).
SPANS = {
    659: (0x54, 0x1CB8), 660: (0x98, 0x2FA8),
    661: (0xAC, 0x3AE0), 662: (0x120, 0x1199C),
    663: (0x60, 0x1FF0), 665: (0xA0, 0x47A0),
    666: (0x5C, 0x2668), 667: (0xB4, 0x4588),
    # Last executable return is 0x80116CA0, with its delay slot at 0x80116CA4.
    # Later jr-ra-shaped words belong to the STR VLC lookup tables, not code.
    668: (0x98, 0x2430),
}
ALIASES = {669: 660, 670: 659, 671: 661, 672: 667}


def padding_spans(blob):
    """Read declared NOP gaps, checking their extent and original contents."""
    config = yaml.safe_load((ROOT / 'configs/PAL/code_layout.yaml').read_text())['overlay_padding']
    sectors = struct.unpack_from('<951I', blob, 4)
    result = {}
    for index, ranges in config.items():
        start, end = SPANS[index]
        result[index] = []
        previous = BASE + start
        for a, b in ranges:
            assert previous <= a < b <= BASE + end and a % 4 == b % 4 == 0
            for site in [index] + [alias for alias, canonical in ALIASES.items()
                                    if canonical == index]:
                offset = sectors[site] * 2048 + a - BASE
                data = blob[offset:offset + b - a]
                assert len(data) == b - a and not any(data), (index, hex(a))
            result[index].append((a, b))
            previous = b
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, default=ROOT / 'disc/PAL/R4.BIN')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/overlays')
    args = parser.parse_args()
    blob = args.input.read_bytes()
    count, = struct.unpack_from('<I', blob)
    if count != 950 or len(blob) < 4 + 4 * (count + 1):
        raise ValueError('Expected PAL directory with 950 entries')
    sectors = struct.unpack_from(f'<{count + 1}I', blob, 4)
    if sectors[0] * 2048 < 4 + 4 * (count + 1):
        raise ValueError('Directory overlaps payload')
    if any(a > b for a, b in zip(sectors, sectors[1:])) or sectors[-1] * 2048 != len(blob):
        raise ValueError('Invalid sector directory')
    chunks = [blob[a * 2048:b * 2048] for a, b in zip(sectors, sectors[1:])]
    args.output.mkdir(parents=True, exist_ok=True)
    entries = []
    for index, data in enumerate(chunks):
        words = struct.unpack(f'<{len(data) // 4}I', data)
        returns = sum(word == 0x03E00008 for word in words)
        row = dict(index=index, sector=sectors[index], size=len(data),
                   sha256=hashlib.sha256(data).hexdigest(), jr_ra_count=returns)
        if index in SPANS or index in ALIASES:
            canonical = ALIASES.get(index, index)
            if data != chunks[canonical]:
                raise ValueError(f'Overlay alias {index} changed')
            start, end = SPANS[canonical]
            if words[end // 4 - 2] != 0x03E00008:
                raise ValueError(f'Unexpected last return in {index}')
            row.update(kind='overlay', canonical=canonical,
                       module_id=words[0], load_address=BASE,
                       candidate_text_start=start, candidate_text_end=end,
                       candidate_text_bytes=end - start)
            path = args.output / f'{index}.bin'
            path.write_bytes(data)
            if canonical == index:
                result = subprocess.run([
                    str(ROOT / 'tools/toolchain/bin/mips-linux-gnu-objdump'),
                    '-D', '-b', 'binary', '-m', 'mips:3000', '-EL',
                    f'--adjust-vma={BASE:#x}',
                    f'--start-address={BASE + start:#x}',
                    f'--stop-address={BASE + end:#x}', str(path),
                ], capture_output=True, text=True, check=True)
                (args.output / f'{index}.text.s').write_text(result.stdout)
        elif returns:
            row['kind'] = 'unclassified_mips_patterns'
        entries.append(row)
    report = dict(input_sha256=hashlib.sha256(blob).hexdigest(),
                  entry_count=count, overlay_entries=len(SPANS) + len(ALIASES),
                  unique_overlays=len(SPANS),
                  unique_candidate_text_bytes=sum(b - a for a, b in SPANS.values()),
                  entries=entries)
    (args.output / 'inventory.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'entries'}, indent=2))


if __name__ == '__main__':
    main()
