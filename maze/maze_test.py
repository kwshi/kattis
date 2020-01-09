import hypothesis.strategies as hs
import hypothesis as hy
import collections as co
import itertools as it
import maze


def make_graph(edges):
    graph = co.defaultdict(set)
    for a, b in edges:
        graph[a].add(b)
        graph[b].add(a)
    return {k: list(v) for k, v in graph.items()}


graphs = (
    hs.integers(1, 100)
    .flatmap(
        lambda size: hs.sets(
            hs.tuples(hs.integers(1, size), hs.integers(1, size)).filter(
                lambda p: p[0] != p[1]
            )
        )
    )
    .map(make_graph)
)


@hy.given(graphs)
def test_reflexive(g):
    m = maze.maze(g)
    for a in g:
        assert m.equivalent(a, a)


@hy.given(graphs)
def test_symmetric(g):
    m = maze.maze(g)
    for a, b in it.product(g, repeat=2):
        assert m.equivalent(a, b) == m.equivalent(b, a)


@hy.given(graphs)
def test_transitive(g):
    m = maze.maze(g)
    for a, b, c in it.product(g, repeat=3):
        if m.equivalent(a, b) and m.equivalent(b, c):
            assert m.equivalent(a, c)


def test_example1():
    g = {1: [5, 2, 3, 4], 2: [1, 3], 3: [1, 2], 4: [1], 5: [1]}
    m = maze.maze(g)
    # assert not m.equivalent(4, 5)
    # assert not m.equivalent(5, 4)
    for a, b in it.product(g, repeat=2):
        assert m.equivalent(a, b) == m.equivalent(b, a), (a, b)
