import collections as co
import itertools as it
import pprint as pp


def sublists(a, indices):
    return (a[i:j] for i, j in zip([0] + indices, indices + [len(a)]))


def get(s, i):
    if i < 0 or i >= len(s):
        return ""
    return s[i]


def neighbors(parent, gap):
    for i in it.chain(range(gap - 2), range(gap + 3, len(parent) - 1)):
        p = parent[i : i + 2]
        before = [
            get(parent, gap - 1) + get(parent, gap + 2),
            get(parent, i - 1) + get(parent, i),
            get(parent, i + 1) + get(parent, i + 2),
        ]
        after = [
            get(parent, gap - 1) + p[0],
            p[1] + get(parent, gap + 2),
            get(parent, i - 1) + get(parent, i + 2),
        ]

        if after.count("BA") != before.count("BA") - 1:
            continue

        m, n = min(i, gap), max(i, gap)
        left, a, mid, b, right = sublists(parent, [m, m + 2, n, n + 2])
        child = "".join([left, b, mid, a, right])

        # print(parent, child, gap, i, before, after)
        # after = [gap_pair(child, i), pair(child, gap), pair(child, gap + 2)]

        # assert after.count("BA") - before.count("BA") == child.replace("_", "").count(
        #    "BA"
        # ) - parent.replace("_", "").count("BA")

        yield child, i


def show(s, gap):
    return "".join(["_" * (len(s) + 2), s[:gap], "__", s[gap:]])


def follow(parents, child):
    yield child
    while child in parents:
        child = parents[child]
        yield child


def search(size):
    start = "__" + "BA" * size
    frontier = co.deque([(start, 0)])
    seen = {start: 0}
    parents = {}
    while frontier:
        parent, gap = frontier.popleft()
        # print(len(frontier), end="\r")
        for child, g in neighbors(parent, gap):
            if child in seen:
                continue
            seen[child] = seen[parent] + 1
            parents[child, g] = parent, gap

            if seen[child] == size and g == 2 * size:
                return list(follow(parents, (child, g)))[::-1]

            frontier.append((child, g))


for size in range(4, 16):
    print("n={}:".format(size))
    path = search(size)
    print(path[0][0])
    for (_, gap), (child, fill) in zip(path, path[1:]):
        print("{}: {} to {}".format(child, fill, gap))
    print()
