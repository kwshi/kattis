import collections as co
import itertools as it
import pprint as pp
import math
import string


class Game:
    def __init__(self):
        self.n = int(input())
        self.opts = [
            [[ord(c) - ord("a") for c in opt] for opt in input().split()[1:]]
            for _ in range(self.n)
        ]

    def propagate(self, tgt):
        dist = [-1] * self.n
        dist[tgt] = 0
        updated = True
        steps = 1
        while updated:
            updated = False
            for i in range(self.n):
                if dist[i] != -1:
                    continue

                if any(
                    all(dist[c] != -1 and dist[c] < steps for c in opt)
                    for opt in self.opts[i]
                ):
                    updated = True
                    dist[i] = steps

            steps += 1
        return dist

    def solve(self):
        dists = [[0] * self.n for _ in range(self.n)]
        for tgt in range(self.n):
            for src, dist in enumerate(self.propagate(tgt)):
                dists[src][tgt] = dist
        return dists


def show(dists):
    print("\n".join(" ".join(map(str, row)) for row in dists))


show(Game().solve())
