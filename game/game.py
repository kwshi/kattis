import collections as co
import itertools as it
import pprint as pp
import math
import string


def parse():
    size = int(input())
    return [
        [
            {ord(c) - ord("a") for c in opt}
            for opt in input().strip().split()[1:]
        ]
        for _ in range(size)
    ]


def solve(options):
    # nodes = [opt for opts in options for opt in opts]

    for i, opts in enumerate(options):
        acc = []
        for opt in opts:
            if any(o.issubset(opt) for o in acc):
                continue
            acc.append(opt)
        assert len(acc) < 500
        options[i] = acc

    dist = [[math.inf] * len(options) for _ in options]
    for i, opts in enumerate(options):
        dist[i][i] = 0

    for i in range(len(options)):
        for _ in range(len(options) - 1):
            for j in range(len(options)):
                dist[j][i] = min(
                    dist[j][i],
                    1 + min(max(dist[k][i] for k in opt) for opt in options[j]),
                )

    # for k, i, j in it.product(range(len(options)), repeat=3):
    #    # best route from i to j using 0..k
    #    # min(max(for l in opt) for opt in options[i])
    #    # dist[i]
    #    dist[i][j] = min(dist[i][j], )
    #    pass

    return dist


def show(dist):
    for row in dist:
        print(" ".join(str(-1 if d == math.inf else d) for d in row))


show(solve(parse()))
