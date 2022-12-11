#include <iostream>
#include <cmath>
#include <unordered_set>
#include <vector>
#include <ranges>

struct Vector2 {
    long hash() { return 1000l * x + y; }
    int x{}, y{};
};

int sgn(int x) { return (x > 0) - (x < 0); }

void follow(Vector2& prev, Vector2& next) {
    auto dx = prev.x - next.x, dy = prev.y - next.y;
    bool move = std::abs(dx) == 2 or std::abs(dy) == 2;
    next.x += move * sgn(dx);
    next.y += move * sgn(dy);
}

void move(Vector2& pos, char dir) {
    pos.x += (dir == 'R') - (dir == 'L');
    pos.y += (dir == 'U') - (dir == 'D');
}

int main() {
    std::vector<Vector2> rope(10);
    std::unordered_set<long> traceTail;
    std::unordered_set<long> traceOneAfterHead;

    char direction;
    int moves;
    while (std::cin >> direction >> moves) {
        while (moves--) {
            move(rope.front(), direction);
            for (auto [prev, next] : rope | std::views::pairwise) {
                follow(prev, next);
            }
            traceOneAfterHead.insert(rope[1].hash());
            traceTail.insert(rope.back().hash());
        }
    }

    std::cout << traceOneAfterHead.size() << "\n";
    std::cout << traceTail.size() << "\n";
}