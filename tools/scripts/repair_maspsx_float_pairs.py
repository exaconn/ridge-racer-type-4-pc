"""Add the omitted temporary/saved GPR pairs used by soft-float li.d."""
import argparse
import ast
from pathlib import Path


def repair(directory):
    path = Path(directory) / 'maspsx/__init__.py'
    text = path.read_text()
    tree = ast.parse(text)
    function = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'get_next_register')
    entries = {}
    for node in ast.walk(function):
        if isinstance(node, ast.Dict):
            entries.update(ast.literal_eval(node))
    expected = {'$12': '$13', '$14': '$15', '$16': '$17', '$20': '$21', '$22': '$23', '$24': '$25',
                '$t4': '$t5', '$t6': '$t7', '$t8': '$t9', '$s4': '$s5', '$s6': '$s7'}
    for key in entries.keys() & expected.keys():
        if entries[key] != expected[key]:
            raise ValueError(f'Unexpected register pair in {path}: {key} -> {entries[key]}')
    missing = {key: value for key, value in expected.items() if key not in entries}
    if not missing:
        return False
    anchor = '    next_reg = lut.get(reg)\n'
    if text.count(anchor) != 1:
        raise ValueError(f'Unknown MASPSX register lookup layout in {path}')
    addition = '    # Soft-float constants may also occupy these temporary/saved pairs.\n    lut.update({\n'
    addition += ''.join(f'        {key!r}: {value!r},\n' for key, value in missing.items())
    addition += '    })\n'
    path.write_text(text.replace(anchor, addition + anchor))
    return True


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    args = parser.parse_args()
    print('Added missing GPR pairs.' if repair(args.directory) else 'GPR pairs already supported.')
