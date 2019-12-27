import itertools as it
import functools as ft
import operator as op
import collections as co
import fractions as fr
import math

vec = co.namedtuple("vec", "x y")

make_vec_op = lambda o: lambda self, other: vec(
    *it.starmap(o, zip(self, other))
)
make_scalar_op = lambda o: lambda self, other: vec(
    *map(ft.partial(o, other), self)
)

vec.__add__ = make_vec_op(op.add)
vec.__sub__ = make_vec_op(op.sub)
vec.__mul__ = make_scalar_op(op.mul)
vec.__truediv__ = make_scalar_op(lambda a, b: b / a)
vec.__rmul__ = vec.__mul__
cross = lambda a, b: a.x * b.y - a.y * b.x
area3 = lambda a, b, c: cross(b - a, c - a) / 2
centroid3 = lambda a, b, c: (a + b + c) / 3

area = lambda poly: sum(
    cross(poly[i], poly[(i + 1) % len(poly)]) / 2
    for i in range(len(poly))
)

area_relative = lambda poly: sum(
    area3(poly[0], poly[i], poly[i + 1])
    for i in range(1, len(poly) - 1)
)

centroid = lambda poly: ft.reduce(
    op.add,
    (
        cross(poly[i], poly[(i + 1) % len(poly)])
        / 2
        * (poly[i] + poly[(i + 1) % len(poly)])
        / 3
        for i in range(len(poly))
    ),
) / area(poly)


centroid_relative = lambda poly: ft.reduce(
    op.add,
    (
        area3(poly[0], poly[i], poly[i + 1])
        * centroid3(poly[0], poly[i], poly[i + 1])
        for i in range(1, len(poly) - 1)
    ),
) / area_relative(poly)


def min_max(l):
    l = list(l)
    return min(l), max(l)


solve_inequality = lambda a, b: (
    ((0, math.inf) if 0 <= b else (0, -1))
    if a == 0
    else (0, b / a)
    if a > 0
    else (max(b / a, 0), math.inf)
)


intersect = lambda i, j: (max(i[0], j[0]), min(i[1], j[1]))


def solve(poly):
    x = poly[0].x
    l, r = min_max(p.x for p in poly if p.y == 0)
    c = centroid(poly).x
    m = abs(area(poly))

    a, b = intersect(
        solve_inequality(x - r, m * (r - c)),
        solve_inequality(l - x, m * (c - l)),
    )

    return (
        (math.floor(a), math.inf if b == math.inf else math.ceil(b))
        if a <= b
        else None
    )


def parse():
    pts = int(input())
    return [
        vec(*map(fr.Fraction, input().strip().split()))
        for _ in range(pts)
    ]


if __name__ == "__main__":
    s = solve(parse())
    if s is None:
        print("unstable")
    else:
        print("{} .. {}".format(*s))
