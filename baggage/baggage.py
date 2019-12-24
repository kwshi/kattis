import collections as co
import pprint as pp


def search(size):
    offset = 2 * size - 1
    start = "_" * 2 * size + "BA" * size
    frontier = co.deque([start])
    gap = {start: -1}
    prev = {}
    seen = {start: 0}
    end = "_" * 2 * (size - 1) + "A" * size + "B" * size + "__"
    while frontier:
        parent = frontier.popleft()
        g = gap[parent]
        s = seen[parent]

        for i in range(-1, 2 * size):
            if g - 2 < i < g + 2:
                continue
            m, n = offset + min(i, g), offset + max(i, g)
            left, a, mid, b, right = (
                parent[:m],
                parent[m : m + 2],
                parent[m + 2 : n],
                parent[n : n + 2],
                parent[n + 2 :],
            )
            child = left + b + mid + a + right

            if child in seen:
                continue

            seen[child] = s + 1
            prev[child] = parent
            gap[child] = i

            if child == end:
                steps = seen[child]
                path = []
                p = child
                while p != start:
                    path.append(p)
                    p = prev[p]
                path.append(p)
                return (
                    steps,
                    path[::-1],
                    [gap[p] for p in reversed(path)],
                )

            if seen[child] == size:
                continue

            frontier.append(child)


for size in range(12, 20):
    count, path, gaps = search(size)
    print("n = {}: {} steps".format(size, count))
    print(path[0])
    for p, i, j in zip(path[1:], gaps[1:], gaps):
        print("{}: {} to {}".format(p, i, j))
    print()
