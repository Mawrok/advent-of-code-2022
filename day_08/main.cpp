#include <iostream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <utility>
#include <string>
#include <functional>

class ForestView {
    std::string_view data;
    std::size_t width;
public:
    ForestView(std::string_view sv) :data(sv), width(std::sqrt(sv.size())) {}

    auto rows(std::size_t i) { return data | std::views::drop(i / width * width) | std::views::take(width); }
    auto cols(std::size_t i) { return data | std::views::drop(i % width) | std::views::stride(width); }

    auto left(std::size_t i) { return rows(i) | std::views::take(i % width) | std::views::reverse; }
    auto right(std::size_t i) { return rows(i) | std::views::drop(1 + i % width); }
    auto up(std::size_t i) { return cols(i) | std::views::take(i / width) | std::views::reverse; }
    auto down(std::size_t i) { return cols(i) | std::views::drop(1 + i / width); }
};

template <typename T>
auto rate_position(T trees, char value) {
    auto it = std::ranges::find_if(trees, std::bind_back(std::greater_equal{}, value));
    bool visibility = it == trees.end();
    std::size_t scenic_score = 1 + std::distance(trees.begin(), it) - visibility;
    return std::tuple(visibility, scenic_score);
}

int main() {
    auto data = std::views::istream<char>(std::cin) | std::ranges::to<std::string>();
    std::size_t count{}, max{};
    ForestView forest{ data };

    for (auto i : std::views::iota(0u, data.size())) {
        auto [visibility1, score1] = rate_position(forest.left(i), data[i]);
        auto [visibility2, score2] = rate_position(forest.right(i), data[i]);
        auto [visibility3, score3] = rate_position(forest.up(i), data[i]);
        auto [visibility4, score4] = rate_position(forest.down(i), data[i]);
        count += (visibility1 | visibility2 | visibility3 | visibility4);
        max = std::max(max, score1 * score2 * score3 * score4);
    }

    std::cout << count << "\n";
    std::cout << max << "\n";
}