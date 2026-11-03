#!/usr/bin/env python3
"""Coverage of resident text plus unique candidate overlay text spans.

Coverage measures code recovered in C; instruction Levenshtein is the separate
fit metric. Overlay drafts are excluded unless present in the verified ledger.
The overlay denominator is provisional, as documented in docs/overlays.md.
"""
import argparse
import json
from pathlib import Path
import sys
import yaml
from analyze_overlays import BASE, SPANS, padding_spans
from c_functions import source_owners
from overlay_units import function_owner, function_extent

ROOT = Path(__file__).resolve().parents[2]


def coverage(ledger_path, target_percent=40):
    if not isinstance(target_percent, int) or not 1 <= target_percent <= 100:
        raise ValueError("Target percentage must be an integer from1 to100")
    ledger = json.loads(ledger_path.read_text())
    if 'functions' in ledger:  # a verify_overlay_samples.py report used directly
        report = ledger
        verified = {r['symbol'] for r in report['functions']}
    else:
        report = json.loads((ROOT / ledger['overlay_report']).read_text())
        verified = set(ledger['overlay_owners'])
    assert verified == {r['symbol'] for r in report['functions']}
    assert all(r['levenshtein'] == 0 for r in report['functions'])
    assert report['full_exe_identical'] and report['reconstructed_r4_identical']
    rows = [line.split() for line in (ROOT/'configs/PAL/functions.txt').read_text().splitlines()]
    spans = {n:(int(a,16),int(s,16)) for n,a,s in rows}
    owners = source_owners(ROOT/'src/main/PAL')
    assert owners.keys() <= spans.keys()
    config = yaml.safe_load((ROOT/'configs/PAL/main.yaml').read_text())
    image = next(s for s in config['segments'] if isinstance(s,dict) and s.get('name') == 'main')
    text_start = min(a for a,s in spans.values())
    data_offset = next(s[0] for s in image['subsegments'] if isinstance(s,list) and s[1] == 'data')
    text_end = image['vram'] + data_offset - image['start']
    resident_total = text_end-text_start
    last=text_start
    for a,size in sorted(spans.values()):
        assert last <= a and a+size <= text_end
        last=a+size
    resident_c = sum(spans[n][1] for n in owners)
    # Hand-written assembly (splat `hasm` segments) cannot be recovered as C.
    hasm = [l.split()[0] for l in (ROOT/'configs/PAL/hasm.txt').read_text().splitlines()
            if l.split('#',1)[0].strip()]
    assert any(isinstance(s,list) and s[1] == 'hasm' for s in image['subsegments']) or not hasm
    assert not set(hasm) & owners.keys()
    resident_hasm = sum(spans[n][1] for n in hasm)
    resident_total -= resident_hasm
    resident_padding = 0
    original = (ROOT/'assets/PAL/main.exe').read_bytes()
    layout = yaml.safe_load((ROOT/'configs/PAL/code_layout.yaml').read_text())
    subsegments = image['subsegments']
    for segment, following in zip(subsegments, subsegments[1:]):
        if not isinstance(segment,list) or segment[1] != 'pad':
            continue
        start, end = segment[0], following[0]
        a = image['vram'] + start - image['start']
        b = a + end - start
        assert text_start <= a < b <= text_end
        assert all(b <= fa or fa + size <= a for fa,size in spans.values())
        assert all(b <= da or db <= a for da,db in layout['resident_data'])
        data = original[start:end]
        assert len(data) == end - start and not any(data)
        resident_padding += end - start
    resident_total -= resident_padding
    overlay_c=0
    intervals={}
    for row in report['functions']:
        index=row['overlay'];a=row['address'];size=row['original_size'];start,end=SPANS[index]
        assert BASE+start <= a and a+size <= BASE+end
        source, symbol = function_owner(index, a)
        function_extent(source.read_text(), symbol)
        intervals.setdefault(index,[]).append((a,a+size))
        overlay_c+=size
    overlay_hasm=0
    for line in (ROOT/'configs/PAL/overlay_hasm.txt').read_text().splitlines():
        line=line.split('#',1)[0].split()
        if not line: continue
        index,a,size=int(line[0]),int(line[1],16),int(line[2],16);start,end=SPANS[index]
        assert BASE+start <= a and a+size <= BASE+end
        intervals.setdefault(index,[]).append((a,a+size))
        overlay_hasm+=size
    overlay_padding = 0
    for index, ranges in padding_spans((ROOT/'disc/PAL/R4.BIN').read_bytes()).items():
        intervals.setdefault(index,[]).extend(ranges)
        overlay_padding += sum(b-a for a,b in ranges)
    for values in intervals.values():
        values.sort();assert all(a[1] <= b[0] for a,b in zip(values,values[1:]))
    total=resident_total+sum(b-a for a,b in SPANS.values())-overlay_hasm-overlay_padding
    recovered=resident_c+overlay_c
    threshold=(total*target_percent+99)//100
    return dict(scope='PAL resident .text plus nine unique candidate overlay text spans, excluding hand-written hasm and declared NOP padding; aliases excluded',
                denominator_provisional=True, denominator_source='configs/PAL/main.yaml, configs/PAL/code_layout.yaml and analyze_overlays.SPANS',
                total_code_bytes=total,resident_text_bytes=resident_total,
                resident_hasm_bytes=resident_hasm,resident_hasm_functions=len(hasm),
                resident_padding_bytes=resident_padding,overlay_padding_bytes=overlay_padding,
                resident_c_bytes=resident_c,verified_overlay_c_bytes=overlay_c,
                overlay_hasm_bytes=overlay_hasm,recovered_c_bytes=recovered,coverage_percent=100*recovered/total,
                target_percent=target_percent,target_c_bytes=threshold,remaining_c_bytes=max(0,threshold-recovered),
                main_functions=len(owners),verified_overlay_functions=len(verified),
                overlay_ledger=str(ledger_path.relative_to(ROOT)))


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--ledger',type=Path,default=ROOT/'build/overlays/samples/verified.json',
                   help='verify_overlay_samples.py report (default) or an older ledger pointing at one')
    p.add_argument('--output',type=Path)
    p.add_argument('--target-percent',type=int,default=40)
    args=p.parse_args();result=coverage(args.ledger.resolve(),args.target_percent)
    text=json.dumps(result,indent=2)+'\n'
    if args.output:args.output.write_text(text)
    print(text,end='')

if __name__=='__main__':main()
