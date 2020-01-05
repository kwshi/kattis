#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <deque>
#include <string>
#include <limits>

using namespace std;
using opt_t = unordered_set<char>;
using options_t = vector<vector<opt_t>>;
using parents_t = vector<unordered_set<char>>;
using dist_t = vector<vector<int>>;

bool operator<=(const opt_t& a, const opt_t& b) {
  if (a.size() > b.size()) return false;
  for (char c : a)
    if (b.find(c) == b.end()) return false;
  return true;
}

// filters away any option that is a superset of another
vector<opt_t> filter_supsets(const vector<opt_t>& opts) {
  vector<bool> keep(opts.size(), true);
  for (size_t i{}; i < opts.size(); ++i) {
    const opt_t& opt{opts[i]};

    // check if any other option is a subset
    for (size_t j{}; j < opts.size(); ++j) {
      const opt_t& other{opts[j]};

      if (i == j || !(other <= opt)) continue;

      // if two options are equal, keep the earlier one
      if (other.size() == opt.size() && i < j) continue;

      keep[i] = false;
    }
  }

  vector<opt_t> kept;
  for (size_t i{}; i < opts.size(); ++i)
    if (keep[i]) kept.push_back(opts[i]);
  return kept;
}

struct game {
  // number of nodes
  int n;

  // maps each node to its options
  options_t options;

  // maps each node to all the nodes that point to it
  parents_t parents;

  // parsing
  game(istream& cin) {
    cin >> n;
    options.resize(n);
    parents.resize(n);

    char parent{};
    for (vector<opt_t>& opts : options) {
      size_t m; cin >> m;
      opts.resize(m);

      for (opt_t& opt : opts) {
        string s; cin >> s;

        for (char c : s) {
          c -= 'a';
          opt.insert(c);
          parents[c].insert(parent);
        }
      }

      auto filtered{filter_supsets(opts)};
      opts.swap(filtered);

      ++parent;
    }
  }

  // reverse BFS: minimum distance from each node to a fixed target
  vector<int> rev_search(int tgt) const {
    // We copy the options and mutate it: when a path is found from
    // some node to the target, we remove that node from all options
    // pointing to it.  If an option becomes empty, then all nodes
    // originally belonging to that option have been "resolved", and
    // the worst-case distance from that option to the target node is
    // the distance of the last node removed from that option.  When
    // that happens, the node referring to that option is also
    // resolved (Alice may simply choose that option, ignoring the
    // rest), so we remove that node from parent sets for other
    // unresolved nodes.
    options_t simp_options{options};
    parents_t simp_parents{parents};

    deque<int> front{tgt};

    // maps each node to its distance to target
    vector<int> seen(n, -1);
    seen[tgt] = 0;

    for (const opt_t& opt : simp_options[tgt]) {
      for (char child : opt) {
        //if (simp_parents[child].find(tgt) == simp_parents[child].end()) continue;
        simp_parents[child].erase(tgt);
      }
    }

    while (!front.empty()) {
      int child{front.front()};
      front.pop_front();

      for (char parent : simp_parents[child]) {
        if (seen[parent] != -1) continue;

        // child reaches target with a distance seen[child], so
        // eliminate child from all options containing child
        bool found{};
        for (opt_t& opt : simp_options[parent]) {
          if (opt.find(child) == opt.end()) continue;
          opt.erase(child);

          // if an option is now empty, then a path has been found
          // from parent to target, with distance seen[child]+1.
          if (opt.empty()) {
            found = true;
            break;
          }
        }

        if (!found) continue;

        // remove parent from other parent sets, to avoid redundantly
        // traversing parent again from other nodes.
        for (const opt_t& opt : simp_options[parent]) {
          for (char other : opt) {
            // avoid invalidating current iterator
            if (other == child) continue;
            simp_parents[other].erase(parent);
          }
        }

        seen[parent] = seen[child] + 1;
        front.push_back(parent);
      }
    }
    return seen;
  }

  dist_t solve() const {
    dist_t dist(n);
    for (auto& d : dist) {
      d.resize(n);
    }

    for (char i{}; i < n; ++i) {
      vector<int> d{rev_search(i)};
      for (int j{}; j < n; ++j) {
        dist[j][i] = d[j];
      }
    }
    return dist;
  }

};

void show(const dist_t& dist) {
  for (const auto& row : dist) {
    for (int d : row)
      cout << d << " ";
    cout << endl;
  }
}

int main() {
  show(game{cin}.solve());
}
