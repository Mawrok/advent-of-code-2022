#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <unordered_set>

struct GridPos {
    int row, col;
    GridPos(int row = 0, int col = 0) : row(row), col(col) {}
    using hash = std::hash<long>;
    operator long() const { return row + 1000 * col; }
    static const std::vector<GridPos> directions;
};

const std::vector<GridPos> GridPos::directions{ GridPos(0, -1), GridPos(-1, 0), GridPos(0,  1), GridPos(1, 0) };

struct Grid {
    std::string data;
    int width, height;
    Grid(int width, int height, std::string s) :data(s), width(width), height(height) {}
    GridPos start() { auto i = data.find('S'); return access(i); }
    GridPos end() { auto i = data.find('E'); return access(i); };
    char operator()  (GridPos const& p) const { return data[p.col * width + p.row]; };
    char& operator() (GridPos const& p) { return data[p.col * width + p.row]; };
    GridPos access(std::size_t i) { return GridPos(i % width, i / width); }
    auto in_bounds(auto const& p) const { return p.row >= 0 && p.col >= 0 && p.row < width&& p.col < height; };
};

template <typename T, typename F>
int bfs(GridPos start, GridPos end, Grid const& grid, T& finish, F& not_passable) {
    std::unordered_set<GridPos, GridPos::hash> visited;
    std::queue<std::pair<GridPos, int>> queue;
    queue.emplace(start, 0);
    visited.emplace(start);
    int result{-1};
    while (not queue.empty()) {
        auto const [current, deep] = queue.front(); queue.pop();
        for (auto const& dir : GridPos::directions) {
            GridPos next(current.row + dir.row, current.col + dir.col);
            if (not grid.in_bounds(next))
                continue;
            if (visited.contains(next))
                continue;
            if (not_passable(current, next))
                continue;
            queue.emplace(next, deep + 1);
            visited.emplace(next);
        }

        if (finish(current)) {
            result = deep;
            break;
        }
    }

    return result;
}

int main() {
    std::string data;
    int width{}, height{};
    for (std::string line; std::getline(std::cin, line); ++height, width = line.size()) 
        std::ranges::copy(line, std::back_inserter(data));

    Grid grid(width, height, data);
    auto start = grid.start(); grid(start) = 'a';
    auto end = grid.end(); grid(end) = 'z';

    auto same_pos = [&](GridPos const& current) { return current == end; };
    auto same_val = [&grid](GridPos const& current) { return grid(current) == 'a'; };
    auto less = [&grid](GridPos const& lhs, GridPos const& rhs) { return grid(lhs) + 1 < grid(rhs); };
    auto greater = [&grid](GridPos const& lhs, GridPos const& rhs) { return grid(lhs) - 1 > grid(rhs); };

    std::cout << bfs(start, end, grid, same_pos, less) << "\n";
    std::cout << bfs(end, start, grid, same_val, greater) << "\n";
}