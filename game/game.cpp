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

//bool operator<=(const opt_t& a, const opt_t& b) {
//  if (a.size() > b.size()) return false;
//  for (char c : a)
//    if (b.find(c) == b.end()) return false;
//  return true;
//}

// filters away any option that is a superset of another
//vector<opt_t> filter_supsets(const vector<opt_t>& opts) {
//  vector<bool> keep(opts.size(), true);
//  for (size_t i{}; i < opts.size(); ++i) {
//    const opt_t& opt{opts[i]};
//
//    // check if any other option is a subset
//    for (size_t j{}; j < opts.size(); ++j) {
//      const opt_t& other{opts[j]};
//
//      if (i == j || !(other <= opt)) continue;
//
//      // if two options are equal, keep the earlier one
//      if (other.size() == opt.size() && i < j) continue;
//
//      keep[i] = false;
//    }
//  }
//
//  vector<opt_t> kept;
//  for (size_t i{}; i < opts.size(); ++i)
//    if (keep[i]) kept.push_back(opts[i]);
//  return kept;
//}

char show(char c) {
  return c + 'a';
}

string show(string s) {
  for (char& c : s) c += 'a';
  return s;
}

struct game {

  // number of nodes
  size_t n;

  // mapping from option to set of nodes
  vector<unordered_set<char>> contents;

  // mapping from node to options containing that node
  vector<unordered_set<size_t>> containers;

  // mapping from option to nodes providing that option
  vector<unordered_set<char>> providers;

  // mapping from node to options provided by that node
  //vector<unordered_set<size_t>> available;

  // mapping from option index to string
  //vector<string> options;

  // mapping from string to index
  //unordered_map<string, size_t> index;

  // parsing
  game(istream& cin) {
    cin >> n;
    containers.resize(n);
    //available.resize(n);

    size_t idx{};

    for (size_t i{}; i < n; ++i) {
      size_t m{};
      cin >> m;

      for (size_t j{}; j < m; ++j) {
        string opt;
        cin >> opt;

        //if (index.find(opt) == index.end()) {
        //  index[opt] = index.size();
        //  //options.push_back(opt);
        //  contents.emplace_back();
        //  providers.emplace_back();
        //  if (contents.size() != providers.size() || contents.size() != index.size())
        //    throw invalid_argument{"aaa"};
        //}
        //size_t idx{index[opt]};

        contents.emplace_back();
        providers.emplace_back();

        for (char& c : opt) c -= 'a';

        //cout << contents.size() << " " << idx << endl;

        for (char c : opt) {
          containers[c].insert(idx);
          contents[idx].insert(c);
        }

        providers[idx].insert(i);
        //available[i].insert(idx);

        ++idx;
      }
    }

    //for (size_t i{}; i < options.size(); ++i) {
    //  cout << i << " " << options[i] << endl;
    //}
    //cout << endl;
    //for (auto p : index) {
    //  cout << p.first << " " << p.second << endl;
    //}
  }

  vector<int> propagate(char tgt) const {
    auto remaining{contents};

    deque<char> resolved{tgt};
    vector<int> dist(n, -1);

    dist[tgt] = 0;

    while (!resolved.empty()) {
      char res{resolved.front()};
      //cout << "resolve " << static_cast<char>(res + 'a') << " " << dist[res] << endl;
      resolved.pop_front();
      int d{dist[res]};

      // resolve res by removing it from all options containing it
      for (size_t opt : containers[res]) {
        //cout << "container " << show(opt) << endl;
        remaining[opt].erase(res);

        // if option is not empty, do nothing; otherwise resolve all
        // nodes providing that option
        if (!remaining[opt].empty()) continue;

        for (char c : providers[opt]) {
          // node is already resolved
          if (dist[c] != -1) continue;

          dist[c] = d+1;
          //for (const string& other : options[c]) {
          //  if (other == opt) continue;
          //  providers[other].erase(c);
          //}
          resolved.push_back(c);
        }
      }
    }
    return dist;
  }

  vector<vector<int>> solve() const {
    vector<vector<int>> dist(n);
    for (auto& d : dist) {
      d.resize(n);
    }

    for (char i{}; i < n; ++i) {
      vector<int> d{propagate(i)};
      for (int j{}; j < n; ++j) {
        dist[j][i] = d[j];
      }
    }
    return dist;
  }

};

void show(const vector<vector<int>>& dist) {
  for (const auto& row : dist) {
    for (int d : row)
      cout << d << " ";
    cout << endl;
  }
}

int main() {
  //game g{cin};
  show(game{cin}.solve());
}
