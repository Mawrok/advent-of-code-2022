#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
    std::string backpack;
    int sum{};
    while (std::cin >> backpack) {
        std::string intersection;
        auto mid = backpack.begin() + backpack.size() / 2;
        std::sort(backpack.begin(), mid);
        std::sort(mid, backpack.end());
        std::set_intersection(backpack.begin(), mid, mid, backpack.end(), std::back_inserter(intersection));
        char chr = intersection.front();
        sum += std::islower(chr) ? chr - 'a' + 1 : chr - 'A' + 27;
    }
    std::cout << sum << "\n";
}