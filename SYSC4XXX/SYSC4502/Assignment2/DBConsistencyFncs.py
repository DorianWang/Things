# This was made after this paper, even though it is 40 years old and now ancient by CS standards.
# https://www.sciencedirect.com/science/article/pii/0022000081900337
# New Hash Functions and Their Use in Authentication and Set Equality
# hash(int) returns value % 2^31 - 1 on 32 bit python, or value % 2^61 - 1 on 64 bit python
# This only works for ints, and not long integers, but I shouldn't hit up against 2^31 with the current inputs.
# I will currently ignore the difference, but it could become an issue in heterogeneous server networks.

def add_value(current_hash: int, int_in: int) -> int:
    """Takes the hash of the set that is being added to and the value, and returns the new hash value."""
    return current_hash ^ hash(int_in)


def remove_value(current_hash: int, int_in: int) -> int:
    """Takes the hash of the set that is being removed from and the value, and returns the new hash value."""
    return current_hash ^ hash(int_in)


def calc_hash(set_in: set):
    """Takes a set with integer values and then returns the hash value."""
    temp_hash = 0
    for value in set_in:
        temp_hash = add_value(temp_hash, value)
    return temp_hash


