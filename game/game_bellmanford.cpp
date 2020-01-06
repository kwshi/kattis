#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

bool lt(int a, int b) {
  if (a == -1) return false;
  return b == -1 || a < b;
}

int min(int a, int b) {
  return lt(a, b) ? a : b;
}

int max(int a, int b) {
  return lt(a, b) ? b : a;
}

int add(int a, int b) {
  if (a == -1 || b == -1) return -1;
  return a + b;
}

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
      opts[i] = filter(all);
    }
  }

  vector<int> propagate(char tgt) {
    vector<int> dist(n, -1);
    vector<bool> done(n);
    dist[tgt] = 0;
    done[tgt] = true;
    bool updated{true};
    while (updated) {
    //for (int reps{}; reps < n-1; ++reps) {
      updated = false;
      //for (int twice{}; twice < 2; ++twice) {
      for (char i{}; i < n; ++i) {
        //if (dist[i] != -1) continue;
        //if (done[i]) continue;
        for (const string& opt : opts[i]) {
          int d{};
          for (char j : opt) {
            d = max(d, dist[j]);
          }
          if (lt(add(d, 1), dist[i])) {
            dist[i] = add(d, 1);
            updated = true;
            //dist[i] = min(dist[i], add(d, 1));
          }
        }
      }
      //}
      //for (char i{}; i < n; ++i) {
      //  done[i] = dist[i] != -1;
      //}
    }
    return dist;
  }

  vector<vector<int>> solve() {
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

  void show(std::ostream& cout, vector<vector<int>> dists) {
    for (const vector<int>& dist : solve()) {
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
