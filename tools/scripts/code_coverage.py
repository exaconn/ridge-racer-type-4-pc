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
from analyze_overlays import BASE, SPANS
from c_functions import source_owners, function_definitions

ROOT = Path(__file__).resolve().parents[2]


def coverage(ledger_path, target_percent=40):
    if not isinstance(target_percent, int) or not 1 <= target_percent <= 100:
        raise ValueError("Target percentage must be an integer from1 to100")
    ledger = json.loads(ledger_path.read_text())
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
    overlay_c=0
    intervals={}
    for row in report['functions']:
        index=row['overlay'];a=row['address'];size=row['original_size'];start,end=SPANS[index]
        assert BASE+start <= a and a+size <= BASE+end
        symbol=row['symbol'];source=ROOT/f'src/overlays/PAL/{index}/func_{a:08X}.c'
        names=function_definitions(__import__('re').sub(r'\bovl\d+_(?=func_)','',source.read_text()))
        assert f'func_{a:08X}' in names
        intervals.setdefault(index,[]).append((a,a+size))
        overlay_c+=size
    for values in intervals.values():
        values.sort();assert all(a[1] <= b[0] for a,b in zip(values,values[1:]))
    total=resident_total+sum(b-a for a,b in SPANS.values())
    recovered=resident_c+overlay_c
    threshold=(total*target_percent+99)//100
    return dict(scope='PAL resident .text plus nine unique candidate overlay text spans; aliases excluded',
                denominator_provisional=True, denominator_source='configs/PAL/main.yaml and analyze_overlays.SPANS',
                total_code_bytes=total,resident_text_bytes=resident_total,
                resident_c_bytes=resident_c,verified_overlay_c_bytes=overlay_c,
                recovered_c_bytes=recovered,coverage_percent=100*recovered/total,
                target_percent=target_percent,target_c_bytes=threshold,remaining_c_bytes=max(0,threshold-recovered),
                main_functions=len(owners),verified_overlay_functions=len(verified),
                overlay_ledger=str(ledger_path.relative_to(ROOT)))


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--ledger',type=Path,default=ROOT/'build/goal_1401/part61/verified.json')
    p.add_argument('--output',type=Path)
    p.add_argument('--target-percent',type=int,default=40)
    args=p.parse_args();result=coverage(args.ledger.resolve(),args.target_percent)
    text=json.dumps(result,indent=2)+'\n'
    if args.output:args.output.write_text(text)
    print(text,end='')

if __name__=='__main__':main()
