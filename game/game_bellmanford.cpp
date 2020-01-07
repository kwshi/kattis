#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

using std::vector;
using std::string;

bool operator<=(const string& a, const string& b) {
  if (a.size() > b.size()) return false;
  size_t i{}, j{};
  while (i < a.size() && j < b.size()) {
    if (a[i] < b[j]) return false;
    if (a[i] > b[j]) {
      ++j;
      continue;
    }
    ++i;
    ++j;
  }
  if (j == b.size()) return false;
  return true;
}

vector<string> filter(const vector<string>& all) {
  vector<string> kept;
  for (size_t i{}; i < all.size(); ++i) {
    bool keep{true};
    for (size_t j{}; j < all.size(); ++j) {
      if (i == j
          || !(all[j] <= all[i])
          || (all[i].size() == all[j].size() && i < j))
        continue;
      keep = false;
      break;
    }
    if (keep) kept.push_back(all[i]);
  }
  return kept;
}

struct game {
  int n;
  vector<vector<string>> opts;

  game(std::istream& cin) {
    cin >> n;
    opts.resize(n);

    for (char i{}; i < n; ++i) {
      int m;
      cin >> m;
      vector<string> all(m);
      for (string& opt : all) {
        cin >> opt;
        for (char& c : opt) c -= 'a';
      }
      opts[i] = all;//filter(all);
    }
  }

  vector<int> propagate(char tgt) const {
    vector<int> dist(n, -1);
    dist[tgt] = 0;
    bool updated{true};
    int steps{1};
    while (updated) {
      updated = false;
      for (char i{}; i < n; ++i) {
        if (dist[i] != -1) continue;
        for (const string& opt : opts[i]) {
          bool reach{true};
          for (char j : opt) {
            if (dist[j] == -1 || dist[j] >= steps) {
              reach = false;
              break;
            }
          }
          if (reach) {
            dist[i] = steps;
            updated = true;
            break;
          }
        }
      }
      ++steps;
    }
    return dist;
  }

  vector<vector<int>> solve() const {
    vector<vector<int>> dists(n);
    for (vector<int>& dist : dists) dist.resize(n);

    for (char i{}; i < n; ++i) {
      vector<int> dist{propagate(i)};
      for (char j{}; j < n; ++j) {
        dists[j][i] = dist[j];
      }
    }
    return dists;
  }

  void show(std::ostream& cout, const vector<vector<int>>& dists) const {
    for (const vector<int>& dist : dists) {
      for (int d : dist) {
        cout << d << " ";
      }
      cout << std::endl;
    }
  }
};


int main() {
  game g{std::cin};
  g.show(std::cout, g.solve());
}
