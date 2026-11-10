"""Repair real-word transfers in LP64 builds of the original PSX GCCs.

RTL unions contain host pointers and can be wider than HOST_WIDE_INT. Transfer
logical words individually, preserving the ILP32 compiler's representation.
The target backend and optimization settings remain unchanged.
"""
import argparse
from pathlib import Path

REAL_TRANSFERS = r'''/* RTL fields may contain padding on an LP64 host with 32-bit host words.
   Transfer each logical word through its own field, as on an ILP32 host. */
#define REAL_VALUE_FROM_CONST_DOUBLE(to, from)                         \
do { union real_extract rr_real_words;                                \
     struct rtx_def *rr_real_from = (from);                           \
     int rr_real_index;                                              \
     for (rr_real_index = 0;                                          \
          rr_real_index < sizeof rr_real_words.i / sizeof rr_real_words.i[0]; \
          rr_real_index++)                                           \
       rr_real_words.i[rr_real_index] = XWINT (rr_real_from, 2 + rr_real_index); \
     (to) = rr_real_words.d; } while (0)

#define REAL_VALUE_TO_CONST_DOUBLE(from, to)                           \
do { union real_extract rr_real_words;                                \
     struct rtx_def *rr_real_to = (to);                               \
     int rr_real_index;                                              \
     rr_real_words.d = (from);                                       \
     for (rr_real_index = 0;                                          \
          rr_real_index < sizeof rr_real_words.i / sizeof rr_real_words.i[0]; \
          rr_real_index++)                                           \
       XWINT (rr_real_to, 2 + rr_real_index) = rr_real_words.i[rr_real_index]; \
   } while (0)

'''


def replace_one(text, old, new):
    if text.count(old) != 1:
        raise ValueError('Unknown GCC real-constant source layout: ' + old.splitlines()[0])
    return text.replace(old, new)


def repair(source):
    source = Path(source)
    current = {name: (source / name).read_text() for name in ['real.h', 'emit-rtl.c', 'varasm.c']}
    if '#define REAL_VALUE_TO_CONST_DOUBLE' in current['real.h']:
        required = [('emit-rtl.c', 'REAL_VALUE_TO_CONST_DOUBLE (u.d, tem);'),
                    ('varasm.c', 'REAL_VALUE_TO_CONST_DOUBLE (u.d, r);'),
                    ('varasm.c', 'REAL_VALUE_FROM_CONST_DOUBLE (value->un.du.d, x);'),
                    ('varasm.c', 'REAL_VALUE_FROM_CONST_DOUBLE (u.d, x);')]
        if not all(value in current[name] for name, value in required):
            raise ValueError('Partial host-real repair; use a fresh compiler source tree')
        return False
    updates = {}
    text = current['real.h']
    start = text.index('#define REAL_VALUE_FROM_CONST_DOUBLE(to, from)')
    end = text.index('/* Return a CONST_DOUBLE with value R and mode M.  */', start)
    updates['real.h'] = text[:start] + REAL_TRANSFERS + text[end:]
    updates['emit-rtl.c'] = replace_one(current['emit-rtl.c'],
        'bcopy ((char *) &u, (char *) &CONST_DOUBLE_LOW (tem), sizeof u);',
        'REAL_VALUE_TO_CONST_DOUBLE (u.d, tem);')
    text = replace_one(current['varasm.c'],
        '  for (r = const_double_chain; r; r = CONST_DOUBLE_CHAIN (r))\n'
        '    if (! bcmp ((char *) &CONST_DOUBLE_LOW (r), (char *) &u, sizeof u)\n'
        '\t&& GET_MODE (r) == mode)\n'
        '      return r;',
        '  for (r = const_double_chain; r; r = CONST_DOUBLE_CHAIN (r))\n'
        '    {\n'
        '      union real_extract previous;\n'
        '      REAL_VALUE_FROM_CONST_DOUBLE (previous.d, r);\n'
        '      if (! bcmp ((char *) &previous, (char *) &u, sizeof u)\n'
        '          && GET_MODE (r) == mode)\n'
        '        return r;\n'
        '    }')
    text = replace_one(text, 'bcopy ((char *) &u, (char *) &CONST_DOUBLE_LOW (r), sizeof u);',
                       'REAL_VALUE_TO_CONST_DOUBLE (u.d, r);')
    text = replace_one(text, 'bcopy ((char *) &CONST_DOUBLE_LOW (x),\n'
                       '\t\t (char *) &value->un.du, sizeof value->un.du);',
                       'REAL_VALUE_FROM_CONST_DOUBLE (value->un.du.d, x);')
    updates['varasm.c'] = replace_one(text,
        'bcopy ((char *) &CONST_DOUBLE_LOW (x), (char *) &u, sizeof u);',
        'REAL_VALUE_FROM_CONST_DOUBLE (u.d, x);')
    # Validate every source fragment before replacing any file.
    for name, text in updates.items():
        path = source / name
        temporary = path.with_name(path.name + '.host-real.tmp')
        temporary.write_text(text)
        temporary.chmod(path.stat().st_mode | 0o200)
        temporary.replace(path)
    return True


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    args = parser.parse_args()
    print('Repaired logical RTL real-word transfers.' if repair(args.directory) else 'Host real-word transfers already repaired.')
