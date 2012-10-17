

int foo(int x, int y) {
   return x + y;
}

template <typename T>
struct Whip {
   T x;
};

Whip<int> bar(int x, int y) {
   Whip<int> whip;
   whip.x = x;
   return whip;
}

class Ship {
public:
   bool isFunc() { return true; }
};
