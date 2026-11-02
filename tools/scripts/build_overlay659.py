#!/usr/bin/env python3
"""Compatibility entry point for the complete C build of PAL overlay 659/670."""
import argparse
from build_overlay import build

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--require-complete', action='store_true')
    parser.parse_args()
    build(659)
