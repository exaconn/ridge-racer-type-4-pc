import itertools
import unittest

from levenshtein import levenshtein


class LevenshteinTest(unittest.TestCase):
    def test_instruction_insert_delete_substitute(self):
        self.assertEqual(levenshtein([1, 2, 3], [1, 2, 3]), 0)
        self.assertEqual(levenshtein([1, 2, 3], [1, 4, 2, 3]), 1)
        self.assertEqual(levenshtein([1, 2, 3], [1, 3]), 1)
        self.assertEqual(levenshtein([1, 2, 3], [1, 4, 3]), 1)

    def test_against_reference_dynamic_programming(self):
        sequences = [p for n in range(5) for p in itertools.product(range(2), repeat=n)]
        for left in sequences:
            for right in sequences:
                row = list(range(len(right) + 1))
                for i, a in enumerate(left, 1):
                    previous, row = row, [i]
                    for j, b in enumerate(right, 1):
                        row.append(min(row[-1] + 1, previous[j] + 1,
                                       previous[j - 1] + (a != b)))
                self.assertEqual(levenshtein(left, right), row[-1])

    def test_long_sequences(self):
        for length in (65, 129, 1024):
            left = tuple(range(length))
            middle = length // 2
            right = left[:middle] + (length + 1,) + left[middle:]
            self.assertEqual(levenshtein(left, right), 1)
            self.assertEqual(levenshtein(left, left[1:]), 1)


if __name__ == '__main__':
    unittest.main()
