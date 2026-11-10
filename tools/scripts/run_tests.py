#!/usr/bin/env python3
"""Run native script checks and unittest modules, propagating every failure."""
import argparse
import ast
from concurrent.futures import ThreadPoolExecutor, as_completed
import json
import os
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[2]
DIRECTORY = ROOT/'tools/scripts'


def command(path):
    tree = ast.parse(path.read_text())
    has_cases = any(isinstance(node, ast.ClassDef) and
                    any(ast.unparse(base).split('.')[-1] == 'TestCase' for base in node.bases)
                    for node in ast.walk(tree))
    relative = str(path.relative_to(ROOT))
    return [sys.executable, '-m', 'unittest', relative] if has_cases else [sys.executable, relative]


def run(path, output):
    args = command(path)
    env = os.environ.copy()
    env['PYTHONPATH'] = str(DIRECTORY) + os.pathsep + env.get('PYTHONPATH', '')
    start = time.monotonic()
    result = subprocess.run(args, cwd=ROOT, env=env, capture_output=True, text=True, errors='replace')
    log = output/(path.stem+'.log')
    text = result.stdout+result.stderr
    exit_code = result.returncode
    if '-m' in args and 'Ran 0 tests' in text and exit_code == 0:
        exit_code = 1
        text += '\nNo unittest cases were executed.\n'
    log.write_text(text)
    return dict(test=str(path.relative_to(ROOT)), exit_code=exit_code,
                seconds=round(time.monotonic()-start, 2), log=str(log.relative_to(ROOT)),
                runner='unittest' if '-m' in args else 'script')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('tests', type=Path, nargs='*', help='Defaults to all tools/scripts/test_*.py')
    parser.add_argument('--jobs', type=int, default=4)
    parser.add_argument('--output', type=Path, default=ROOT/'build/tests/run')
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error('--jobs must be positive')
    paths = sorted({p.resolve() for p in (args.tests or DIRECTORY.glob('test_*.py'))})
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = {pool.submit(run, path, output): path for path in paths}
        for future in as_completed(futures):
            row = future.result()
            rows.append(row)
            if row['exit_code']:
                print(f"FAIL {row['test']} ({row['log']})", flush=True)
            if len(rows) % 25 == 0 or len(rows) == len(paths):
                print(f'Completed {len(rows)}/{len(paths)} test modules', flush=True)
            report = dict(total=len(paths), completed=len(rows), results=sorted(rows, key=lambda r:r['test']))
            (output/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    failed = sum(row['exit_code'] != 0 for row in rows)
    print(f'{len(rows)} test modules; {failed} failed.', flush=True)
    return bool(failed)


if __name__ == '__main__':
    sys.exit(main())
