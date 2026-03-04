#include <bits/stdc++.h>

using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point(T _x=0, T _y=0) : x(_x), y(_y) {}

    Point operator + (Point o) const { return Point(x + o.x, y + o.y); }
    Point operator - (Point o) const { return Point(x - o.x, y - o.y); }
    Point operator * (T k) const { return Point(k * x, k * y); } // Scalar's product
    Point operator / (T k) const { return Point(x / k, y / k); }

    T operator * (Point o) const { return x * o.y - y * o.x; } // Cross product
    T operator ^ (Point o) const { return x * o.y + y * o.x; } // Dot product

    double len()  const {return hypot(x, y); };

    bool operator == (Point o) const { return tie(x, y) == tie(o.x, o.y); }
    bool operator < (Point o) const { return tie(x, y) < tie(o.x, o.y); }

    friend ostream &operator << (ostream &out, const Point<T> &p) {
        return out << "(" << p.x << ", " << p.y << ")";
    }
    friend istream &operator >> (istream &in, Point<T> &p) {
        return in >> p.x >> p.y;
    }

    friend T dist(Point a, Point b) { return hypot(a.y - b.y, a.x - b.x); }
};