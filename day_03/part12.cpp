#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cctype>

auto intersects(std::ranges::range auto v) {
    std::string tmp;
    std::string acc{ v[0] };
    std::ranges::sort(acc);
    for (auto next : v | std::views::drop(1)) {
        tmp.clear();
        std::ranges::sort(next);
        std::ranges::set_intersection(acc, next, std::back_inserter(tmp));
        std::swap(acc, tmp);
    }
    return acc.front();
}

int acc(std::ranges::view auto backsacks) {
    auto value = [](auto chr) {return std::islower(chr) ? chr - 'a' + 1 : chr - 'A' + 27; };
    auto transformed = backsacks
        | std::views::transform([](auto c) {return intersects(c); })
        | std::views::transform(value);
    return std::ranges::fold_left(transformed, 0, std::plus{});
}

int main() {
    auto data = std::views::istream<std::string>(std::cin) | std::ranges::to<std::vector>();
    auto by_half = [](std::string s) {
        return std::vector{ s.substr(0, s.size() / 2u), s.substr(s.size() / 2u) }; };

    std::cout << acc(std::views::transform(data, by_half)) << "\n";
    std::cout << acc(std::views::chunk(data, 3)) << "\n";
}