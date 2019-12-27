#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>

static const double prec{100000};

using namespace std;

struct pt {
  double x, y;
  pt() : x{0}, y{0} {}
  pt(double x, double y) : x{x}, y{y} {}
  pt operator-() { return pt{this->x, this->y}; }
  void operator+=(pt b) { this->x += b.x; this->y += b.y; }
  void operator-=(pt b) { this->x -= b.x; this->y -= b.y; }
  void operator*=(double s) { this->x *= s; this->y *= s; }
  void operator/=(double s) { this->x /= s; this->y /= s; }
};

pt operator+(pt a, pt b) { return pt{a.x+b.x, a.y+b.y}; }
pt operator-(pt a, pt b) { return pt{a.x-b.x, a.y-b.y}; }
pt operator*(double s, pt a) { return pt{s*a.x, s*a.y}; }
pt operator*(pt a, double s) { return s * a; }
pt operator/(pt a, double s) { return pt{a.x/s, a.y/s}; }
double cross(pt a, pt b) { return a.x*b.y - b.x*a.y; }
pt moment(pt a, pt b) { return cross(a, b)/2 * (a+b)/3; }

struct interval {
  double low, high;
  interval(double low, double high) : low{low}, high{high} {}
};

interval intersect(interval a, interval b) {
  return interval{max(a.low, b.low), min(a.high, b.high)};
}

double area(vector<pt> pts) {
  double a{};
  for (size_t i{0}; i <= pts.size(); ++i)
    a += cross(pts[i], pts[(i+1)%pts.size()])/2;
  return a;
}

pt centroid(vector<pt> pts) {
  pt m{};
  for (size_t i{}; i <= pts.size(); ++i)
    m += moment(pts[i], pts[(i+1)%pts.size()]);
  return m/area(pts);
}

interval solve_inequality(double a, double b) {
  return a == 0
    ? (0 <= b
       ? interval{0, numeric_limits<double>::infinity()}
       : interval{0, -1})
    : (a > 0
       ? interval{0, b/a}
       : interval{max(0., b/a), numeric_limits<double>::infinity()}
       );

}

interval solve(vector<pt> pts) {
  double x{pts[0].x};
  double l{numeric_limits<double>::infinity()};
  double r{-numeric_limits<double>::infinity()};
  double m{abs(area(pts))};
  double c{centroid(pts).x};

  for (pt p : pts) {
    if (p.y) continue;
    if (p.x < l) l = p.x;
    if (p.x > r) r = p.x;
  }

  return intersect(solve_inequality(x-r, m*(r-c)),
                   solve_inequality(l-x, m*(c-l)));
}

int main() {
  vector<pt> pts;
  size_t count;
  cin >> count;
  for (size_t i{}; i < count; ++i) {
    pt p;
    cin >> p.x >> p.y;
    pts.push_back(p);
  }

  interval i{solve(pts)};
  if (i.low > i.high) {
    cout << "unstable" << endl;
  } else {
    cout
      << setprecision(0) << fixed
      << floor(round(i.low*prec)/prec) << " .. "
      << ceil(round(i.high*prec)/prec) << endl;
  }
}
