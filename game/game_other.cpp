#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

template <typename T>
bool operator<=(const unordered_set<T>& a, const unordered_set<T>& b) {
  if (a.size() > b.size()) return false;
  for (const T& i : a)
    if (b.find(i) == b.end()) return false;
  return true;
}

template <typename T>
void operator-=(unordered_set<T>& a, const unordered_set<T>& b) {
  for (const T& i : b) a.erase(i);
}

template <typename T>
void operator+=(unordered_set<T>& a, const unordered_set<T>& b) {
  for (const T& i : b) a.insert(i);
}

struct game {
  int n;
  vector<vector<unordered_set<char>>> opts;

  vector<int> find(char tgt) const {
    vector<int> dist(n, -1);
    dist[tgt-'a'] = 0;

    unordered_set<char> seen{tgt};

    unordered_set<char> unseen;
    for (char i{}; i < n; ++i) unseen.insert(i + 'a');
    unseen.erase(tgt);

    bool updated{true};
    for (int rep{1}; updated; ++rep) {
      updated = false;
      unordered_set<char> found;
      for (char c : unseen) {
        for (const unordered_set<char>& opt : opts[c - 'a']) {
          if (opt <= seen) {
            updated = true;
            found.insert(c);
            dist[c-'a'] = rep;
            break;
          }
        }
      }
      unseen -= found;
      seen += found;
    }

    return dist;
  }

  vector<vector<int>> solve() const {
    vector<vector<int>> dists(n);
    for (vector<int>& row : dists) row.resize(n);

    for (char i{}; i < n; ++i) {
      vector<int> dist{find(i+'a')};
      for (char j{}; j < n; ++j)
        dists[j][i] = dist[j];
    }

    return dists;
  }

};

istream& operator>>(istream& in, game& g) {
  in >> g.n;
  g.opts.resize(g.n);
  for (vector<unordered_set<char>>& row : g.opts) {
    int m;
    in >> m;
    row.resize(m);
    for (unordered_set<char>& opt : row) {
      string s;
      in >> s;
      for (char c : s) opt.insert(c);
    }
  }
  return in;
}


ostream& operator<<(ostream& out, const game& g) {
  for (const vector<int>& row : g.solve()) {
    for (int d : row)
      out << d << " ";
    out << endl;
  }
  return out;
}

int main() {
  game g;
  cin >> g;
  cout << g;
}
