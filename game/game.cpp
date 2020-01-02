#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <limits>

using namespace std;
using opt_t = unordered_set<int>;
using opts_t = vector<vector<opt_t>>;
using dist_t = vector<vector<int>>;

static const int inf{numeric_limits<int>::max()>>1};

bool operator<(const opt_t& a, const opt_t& b) {
  if (a.size() >= b.size()) return false;
  for (auto i : a)
    if (b.find(i) == b.end())
      return false;
  return true;
}

opts_t parse() {
  int size; cin >> size;
  opts_t opts(size);

  for (int i{}; i < size; ++i) {
    int os; cin >> os;
    opts[i].resize(os);

    for (int j{}; j < os; ++j) {
      string s; cin >> s;
      opt_t opt;

      for (char c : s)
        opt.insert(c - 'a');

      opts[i][j] = opt;
    }

    vector<bool> keep(os);
    for (int j{}; j < os; ++j) {

      bool useless{};
      for (int k{}; k < os; ++k) {
        if (j == k) continue;

        if (opts[i][k] < opts[i][j]) {
          useless = true;
          break;
        }
      }

      keep[j] = !useless;
    }

    vector<opt_t> kept;
    for (int j{}; j < os; ++j) {
      if (keep[j]) kept.push_back(opts[i][j]);
    }
    opts[i] = kept;
  }
  return opts;
}

dist_t solve(const opts_t& opts) {
  dist_t dist(opts.size());
  for (int i{}; i < opts.size(); ++i) {
    dist[i].resize(opts.size(), inf);
    dist[i][i] = 0;
  }

  for (int i{}; i < opts.size(); ++i) {
    for (int rep{}; rep < opts.size()-1; ++rep) {
      for (int j{}; j < opts.size(); ++j) {
        for (const auto& opt : opts[j]) {
          int d{};
          for (int k : opt)
            d = max(d, dist[k][i]);
          dist[j][i] = min(dist[j][i], 1 + d);
        }
      }
    }
  }

  return dist;
}

void show(const dist_t& dist) {
  for (const auto& row : dist) {
    for (int d : row)
      cout << (d == inf ? -1 : d) << " ";
    cout << endl;
  }
}

int main() {
  show(solve(parse()));
}
