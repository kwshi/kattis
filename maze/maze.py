import collections as co
import itertools as it
import pprint as pp
import string


def parse():
    rooms = int(input())
    graph = {}
    for i in range(1, rooms + 1):
        _, *neighbors = input().strip().split()
        graph[i] = list(map(int, neighbors))
    return graph


def can_assign(graph, assigned, taken, a, b):
    if (
        (a in assigned and assigned[a] != b)
        or (b in taken and taken[b] != a)
        or len(graph[a]) != len(graph[b])
    ):
        return False
    return True


def relabel(graph, assigned={}, taken={}):
    print("trying")
    pp.pprint(assigned)
    pp.pprint(taken)
    print()

    try:
        node = next(k for k in graph if k not in assigned)
    except StopIteration:
        yield assigned
        return

    neighbors = graph[node]
    doors = len(neighbors)

    for opt in graph:
        if opt in taken or len(graph[opt]) != doors:
            continue

        for rot in range(len(neighbors)):
            if not all(
                can_assign(
                    graph, assigned, taken, neighbors[i], graph[opt][(i + rot) % doors]
                )
                for i in range(doors)
            ):
                continue

            yield from relabel(
                graph,
                assigned={
                    **assigned,
                    node: opt,
                    **{
                        neighbors[i]: graph[opt][(i + rot) % doors]
                        for i in range(doors)
                    },
                },
                taken={
                    **taken,
                    opt: node,
                    **{
                        graph[opt][(i + rot) % doors]: neighbors[i]
                        for i in range(doors)
                    },
                },
            )

    pp.pprint("exit")


def equivalents(graph):
    eq = {k: set() for k in graph}
    for re in relabel(graph):
        for k, v in re.items():
            eq[k].add(v)
    return eq


def valid(graph, re):
    for a, b in it.product(graph, repeat=2):
        if (b in graph[a]) != (re[b] in graph[re[a]]):
            return False
    return True


# def relabel_naive(graph):
#    lens = co.defaultdict(list)
#    for node, neighbors in graph.items():
#        lens[len(neighbors)].append(node)
#
#    return lens.values()  # it.product(*lens.values())


# pp.pprint(list(relabel(parse(), assigned={7: 10}, taken={10: 7})))
# pp.pprint(list(relabel_naive(parse())))
pp.pprint(equivalents(parse()))
