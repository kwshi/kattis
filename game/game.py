import collections as co
import itertools as it
import pprint as pp
import math
import string


def parse():
    size = int(input())
    parents = [co.defaultdict(list) for _ in range(size)]
    for parent in range(size):
        for opt in input().strip().split()[1:]:
            opt = [ord(c) - ord("a") for c in opt]
            for child in opt:
                parents[child][parent].append(opt)

    return list(map(dict, parents))


def rev_search(parents, target):
    front = co.deque([target])
    seen = {target: 0}
    while front:
        child = front.popleft()
        for parent, opts in parents[child].items():
            if parent in seen:
                continue

            dist = 1 + min(
                max(seen.get(c, math.inf) for c in opt) for opt in opts
            )
            if dist == math.inf or dist != seen[child] + 1:
                continue

            seen[parent] = dist
            front.append(parent)

    return seen


def solve(parents):
    dist = [[-1] * len(parents) for _ in parents]
    for target in range(len(parents)):
        for src, d in rev_search(parents, target).items():
            dist[src][target] = d
    return dist


def show(dist):
    for row in dist:
        print(" ".join(str(-1 if d == math.inf else d) for d in row))


show(solve(parse()))
# pp.pprint(parse())
