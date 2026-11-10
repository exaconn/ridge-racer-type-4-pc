"""Computer-slot constructor on full native descriptors and both MIPS images.

Reuse the bank fixture's complete owner protocol, service observers and guards.
The original player-slot cases remain in their own module unchanged.
"""
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]


def main():
    for version in ('original', 'linked'):
        env = os.environ.copy()
        env['RR_BANK_SETUP_FUNCTION'] = 'ComSlotSetupBank'
        env.pop('RR_COMPILED_ELF', None)
        if version == 'linked':
            env['RR_COMPILED_ELF'] = str(ROOT / 'build/PAL/main.elf')
        subprocess.run([sys.executable, str(ROOT / 'tools/scripts/test_audio_bank_setup_native.py')],
                       cwd=ROOT, env=env, check=True)


if __name__ == '__main__':
    main()
