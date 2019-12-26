import collections as co
import functools as ft
import itertools as it
import pprint as pp
import string


def parse():
    rooms = int(input())
    graph = {}
    indices = {}
    for k in range(1, rooms + 1):
        _, *neighbors = input().strip().split()
        graph[k] = list(map(int, neighbors))
        indices[k] = {v: i for i, v in enumerate(graph[k])}
    return graph, indices


graph, indices = parse()


def equivalent_at_max(a, b, rot, depth=5):
    if not depth:
        return True

    l = len(graph[a])
    for i in range(l):
        x, y = graph[a][i], graph[b][(i + rot) % l]

        if len(graph[x]) != len(graph[y]):
            return False

        k = len(graph[x])
        r = (indices[y][b] - indices[x][a] + k) % k

        if not equivalent_at_max(x, y, r, depth=depth - 1):
            return False

    return True


def equivalent_at(a, b, rot, seen=frozenset()):
    # print("comparing", a, b, rot)
    if (a, b, rot) in seen:
        return True

    s = seen | {(a, b, rot)}

    l = len(graph[a])
    for i in range(l):
        x, y = graph[a][i], graph[b][(i + rot) % l]

        if len(graph[x]) != len(graph[y]):
            return False

        k = len(graph[x])
        r = (indices[y][b] - indices[x][a]) % k

        if not equivalent_at(x, y, r, seen=s):
            return False

    return True


@ft.lru_cache(None)
def equivalent_max(a, b):
    if len(graph[a]) != len(graph[b]):
        return False
    l = len(graph[a])
    return any(equivalent_at_max(a, b, rot) for rot in range(l))


@ft.lru_cache(None)
def equivalent(a, b):
    if len(graph[a]) != len(graph[b]):
        return False
    l = len(graph[a])
    return any(equivalent_at(a, b, rot) for rot in range(l))


def class_of(start, remaining):
    front = co.deque([start])
    seen = {start}
    while front:
        par = front.pop()
        for child in remaining:
            if child in seen or not equivalent(par, child):
                continue
            seen.add(child)
            front.append(child)
    return seen


def classes():
    remaining = set(graph.keys())
    while remaining:
        start = remaining.pop()
        cl = class_of(start, remaining)
        yield cl
        remaining -= cl


def solve():
    cls = sorted((sorted(cl) for cl in classes() if len(cl) > 1), key=lambda cl: cl[0])
    if not cls:
        print("none")
        return
    for cl in cls:
        print(" ".join(map(str, cl)))


def test():
    for k, neighbors in graph.items():
        assert all(k in graph[n] for n in neighbors)

    for cl in classes():
        for a, b in it.combinations(cl, 2):
            assert equivalent(a, b) and equivalent(b, a)

    for cl1, cl2 in it.combinations(classes(), 2):
        for a, b in it.product(cl1, cl2):
            assert not equivalent(a, b) and not equivalent(b, a)


solve()
