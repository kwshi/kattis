import collections as co
import pprint as pp


def parse():
    rooms = int(input())
    graph = {
        k: list(map(int, input().strip().split()))[1:]
        for k in range(1, rooms + 1)
    }
    return graph


def make_indices(graph):
    return {
        a: {b: i for i, b in enumerate(neighbors)}
        for a, neighbors in graph.items()
    }


def make_dfa(graph, start, offset):
    indices = make_indices(graph)

    dfa = {}
    front = [(start, offset)]
    seen = {(start, offset)}
    while front:
        parent_state = parent, offset = front.pop()
        children = graph[parent]
        transitions = [None] * len(children)
        for i, child in enumerate(children):
            child_state = child, indices[child][parent]
            transitions[(i - offset) % len(children)] = child_state
            if child_state in seen:
                continue
            front.append(child_state)
            seen.add(child_state)
        dfa[parent_state] = transitions

    return dfa


pp.pprint(make_dfa(parse(), 1, 0))
