import collections as co
import functools as ft
import itertools as it
import pprint as pp
import string


def parse():
    rooms = int(input())
    graph = {
        k: list(map(int, input().strip().split()))[1:]
        for k in range(1, rooms + 1)
    }
    return maze(graph)


class maze:
    def __init__(self, graph):
        self.graph = graph
        self.indices = {
            k: {v: i for i, v in enumerate(neighbors)}
            for k, neighbors in graph.items()
        }
        self.memo = {}

    def equivalent_at(self, a, b, rot, memo={}):
        key = a, b, rot
        if key in memo:
            return memo[key]

        memo[key] = True

        l = len(self.graph[a])
        for i in range(l):
            x, y = self.graph[a][i], self.graph[b][(i + rot) % l]

            if len(self.graph[x]) != len(self.graph[y]):
                memo[key] = False
                break

            k = len(self.graph[x])
            r = (self.indices[y][b] - self.indices[x][a]) % k

            if not self.equivalent_at(x, y, r, memo):
                memo[key] = False
                break

        return memo[key]

    @ft.lru_cache(None)
    def equivalent(self, a, b):
        # pp.pprint(self.graph)
        # print(a, b)
        if len(self.graph[a]) != len(self.graph[b]):
            return False
        l = len(self.graph[a])
        return any(self.equivalent_at(a, b, rot) for rot in range(l))

    def class_of(self, start, remaining):
        front = co.deque([start])
        seen = {start}
        while front:
            par = front.pop()
            for child in remaining:
                if child in seen or not self.equivalent(par, child):
                    continue
                seen.add(child)
                front.append(child)
        return seen

    def classes(self):
        remaining = set(self.graph.keys())
        while remaining:
            start = remaining.pop()
            cl = self.class_of(start, remaining)
            yield cl
            remaining -= cl

    def solve(self):
        cls = sorted(
            (sorted(cl) for cl in self.classes() if len(cl) > 1),
            key=lambda cl: cl[0],
        )
        return cls


if __name__ == "__main__":
    cls = parse().solve()

    if not cls:
        print("none")

    for cl in cls:
        print(" ".join(map(str, cl)))
