#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char str[26];

typedef struct {
  size_t m;
  str *opts;
} row_t;

typedef struct {
  size_t n;
  row_t opts[26];
} game_t;

game_t parse() {
  game_t g;
  scanf("%zd", &g.n);
  for (int i = 0; i < g.n; ++i) {
    row_t *row = &g.opts[i];
    scanf("%zd", &row->m);
    row->opts = (str *) malloc(row->m * sizeof(str));
    for (int j = 0; j < row->m; ++j) {
      scanf("%s", row->opts[j]);
    }
  }
  return g;
}

void find(game_t g, int dist[], size_t tgt) {
  for (size_t i = 0; i < g.n; ++i) dist[i] = -1;
  dist[tgt] = 0;

  bool updated = true;
  char steps = 1;
  for (; updated; ++steps) {
    updated = false;
    for (size_t src = 0; src < g.n; ++src) {
      if (dist[src] != -1) continue;
      row_t row = g.opts[src];
      for (size_t i = 0; i < row.m; ++i) {
        char *opt = row.opts[i];
        bool reached = true;
        for (size_t j = 0; opt[j] != 0; ++j) {
          int d = dist[opt[j] - 'a'];
          if (d == -1 || d == steps) {
            reached = false;
            break;
          }
        }
        if (reached) {
          dist[src] = steps;
          updated = true;
          break;
        }
      }
    }
  }
}

void solve(game_t g, int dists[]) {
  for (size_t tgt = 0; tgt < g.n; ++tgt) {
    find(g, &dists[tgt*g.n], tgt);
  }
}

void clean(game_t g) {
  for (size_t i = 0; i < g.n; ++i) {
    free(g.opts[i].opts);
  }
}

void show(game_t g) {
  int dists[g.n*g.n];
  solve(g, dists);
  for (size_t src = 0; src < g.n; ++src) {
    for (size_t tgt = 0; tgt < g.n; ++tgt) {
      printf("%d ", dists[tgt*g.n+src]);
    }
    printf("\n");
  }
}

int main() {
  game_t g = parse();
  show(g);
  clean(g);
  return 0;
}
