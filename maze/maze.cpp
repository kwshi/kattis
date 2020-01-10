#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <list>

using namespace std;

struct room {
  size_t k;
  vector<size_t> neighbors;
  unordered_map<size_t, size_t> sources;
};

struct maze {
  size_t n;
  vector<room> rooms;
};

istream& operator>>(istream& in, room& r) {
  in >> r.k;
  r.neighbors.resize(r.k);
  for (size_t i{}; i < r.k; ++i) {
    size_t& neighbor{r.neighbors[i]};
    in >> neighbor;
    --neighbor;
    r.sources[neighbor] = i;
  }
  return in;
}

istream& operator>>(istream& in, maze& m) {
  in >> m.n;
  m.rooms.resize(m.n);
  for (room& r : m.rooms) in >> r;
  return in;
}

using state = pair<size_t, size_t>;

ostream& operator<<(ostream& out, state s) {
  return out << s.first << "[" << s.second << "]";
}

struct dfa {
  map<state, vector<state>> transitions;
  vector<state> states;
  const maze& m;

  dfa(const maze& m) : m{m} {
    for (size_t i{}; i < m.n; ++i) {
      const room& r{m.rooms[i]};
      for (size_t offset{}; offset < r.k; ++ offset) {
        const state s{i, offset};
        states.push_back(s);
        vector<state>& neighbors{transitions[s]};
        neighbors.resize(r.k);
        for (size_t j{}; j < r.k; ++j) {
          size_t other{r.neighbors[(j + offset) % r.k]};
          state tgt{other, m.rooms[other].sources.at(i)};
          neighbors[j] = tgt;
        }
      }
    }
  }
};

struct partitions {
  map<state, size_t> labels;
  unordered_map<size_t, vector<state>> parts;
  const dfa& d;
  const maze& m;

  partitions(const dfa& d) : d{d}, m{d.m} {
    for (const pair<state, vector<state>>& entry : d.transitions) {
      const size_t size{entry.second.size()};
      labels[entry.first] = size;
      parts[size].push_back(entry.first);
    }

    for (;;) {
      //cout << "labels" << endl;
      //for (const auto& p : labels)
      //  cout << "  " << p.first << ": " << p.second << endl;
      //cout << "parts" << endl;
      //for (const auto& p : parts) {
      //  cout << "  " << p.first << ":";
      //  for (const state& s : p.second)
      //    cout << " " << s;
      //  cout << endl;
      //}
      //cout << endl;

      map<pair<size_t, vector<size_t>>, size_t> key;
      map<state, size_t> new_labels;
      unordered_map<size_t, vector<state>> new_parts;
      for (const pair<state, vector<state>>& entry : d.transitions) {
        const state& s{entry.first};
        const vector<state>& neighbors{entry.second};

        vector<size_t> neighbor_labels(neighbors.size());

        for (size_t i{}; i < neighbors.size(); ++i)
          neighbor_labels[i] = labels[neighbors[i]];

        const pair<size_t, vector<size_t>> k{labels[s], neighbor_labels};

        if (key.find(k) == key.end())
          key[k] = key.size();

        const size_t label{key[k]};
        new_labels[s] = label;
        new_parts[label].push_back(s);
      }

      if (new_parts.size() == parts.size()) break;
      parts.swap(new_parts);
      labels.swap(new_labels);
    }
  }

  set<size_t> equivalents(const size_t src) {
    vector<size_t> front{src};
    set<size_t> seen{src};
    while (!front.empty()) {
      const size_t parent{front.back()};
      front.pop_back();
      for (size_t offset{}; offset < m.rooms[parent].k; ++offset) {
        for (const state& child : parts[labels[state{parent, offset}]]) {
          if (seen.find(child.first) != seen.end()) continue;
          front.push_back(child.first);
          seen.insert(child.first);
        }
      }
    }
    return seen;
  }

  vector<set<size_t>> solve() {
    vector<set<size_t>> classes;
    set<size_t> unseen;
    for (size_t i{}; i < m.n; ++i) unseen.insert(i);

    while (!unseen.empty()) {
      const size_t src{*unseen.begin()};
      const set<size_t> cls{equivalents(src)};
      if (cls.size() > 1) classes.push_back(cls);
      for (const size_t s : cls) unseen.erase(s);
    }

    return classes;
  }

};

ostream& operator<<(ostream& out, const dfa& d) {
  for (const auto& entry : d.transitions) {
    out << entry.first << ":";
    for (const auto& neighbor : entry.second)
      out << " " << neighbor;
    out << endl;
  }
  return out;
}

int main() {
  maze m;
  cin >> m;

  dfa d{m};
  //cout << d;

  //cout << endl;

  partitions p{d};
  //for (const auto& p : p.parts) {
  //  for (state s : p.second)
  //    cout << s << " ";
  //  cout << endl;
  //}

  const vector<set<size_t>> classes{p.solve()};
  if (classes.empty())
    cout << "none" << endl;
  for (const set<size_t>& cls : classes) {
    for (const size_t r : cls)
      cout << r+1 << " ";
    cout << endl;
  }

}
