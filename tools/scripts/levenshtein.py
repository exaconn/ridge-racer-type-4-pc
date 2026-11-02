"""Unit-cost Levenshtein distance for instruction sequences."""


def levenshtein(left, right):
    # Myers' bit-vector algorithm; Python integers handle arbitrary lengths.
    if len(left) > len(right):
        left, right = right, left
    if not left:
        return len(right)
    positions = {}
    for index, value in enumerate(left):
        positions[value] = positions.get(value, 0) | (1 << index)
    positive, negative = ~0, 0
    distance = len(left)
    last_bit = 1 << (len(left) - 1)
    for value in right:
        equal = positions.get(value, 0)
        vertical = equal | negative
        horizontal = (((equal & positive) + positive) ^ positive) | equal
        plus = negative | ~(horizontal | positive)
        minus = positive & horizontal
        if plus & last_bit:
            distance += 1
        elif minus & last_bit:
            distance -= 1
        plus = (plus << 1) | 1
        minus <<= 1
        positive = minus | ~(vertical | plus)
        negative = plus & vertical
    return distance
