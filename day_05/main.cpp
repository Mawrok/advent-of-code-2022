#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <iterator>
#include <functional>
using Crates = std::vector<std::vector<char>>;
using Stack = std::vector<char>;

auto load_crates() {
    Crates crates(10);
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) break;
        for (int i{ 1 }; i < line.size(); i += 4) {
            if (std::isalpha(line[i])) {
                crates[i / 4].push_back(line[i]);
            }
        }
    } std::cin.ignore(1, '\n');
    for (auto& crate : crates) { std::ranges::reverse(crate); }
    return crates;
}

void move(Stack& src, Stack& dest, int count) {
    dest.resize(dest.size() + count);
    std::move(src.end() - count, src.end(),
        dest.end() - count);
    src.erase(src.end() - count, src.end());
}

void move_backward(Stack& src, Stack& dest, int count) {
    dest.resize(dest.size() + count);
    std::move_backward(src.end() - count, src.end(),
        std::make_reverse_iterator(dest.end() - count));
    src.erase(src.end() - count, src.end());
}

void show(Crates const& crates) {
    auto c = crates | std::views::filter(std::not_fn(&Stack::empty));
    std::ranges::transform(c,
        std::ostream_iterator<char>(std::cout),
        [](Stack const& c) { return c.back(); }
    );
    std::cout << "\n";
}

int main() {
    auto crates = load_crates();
    auto crates_backward = crates;

    std::string _;
    int count, source, destination;
    while (std::cin >> _ >> count >> _ >> source >> _ >> destination) {
        move(crates[source - 1], crates[destination - 1], count);
        move_backward(crates_backward[source - 1], crates_backward[destination - 1], count);
    }

    show(crates);
    show(crates_backward);
}