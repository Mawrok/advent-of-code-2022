#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
    std::string backpack1, backpack2, backpack3;
    int sum{};
    while (std::cin >> backpack1 >> backpack2 >> backpack3) {
        using namespace std::ranges;
        std::string intersect, result;
        sort(backpack1); sort(backpack2); sort(backpack3);
        set_intersection(backpack1, backpack2, std::back_inserter(intersect));
        set_intersection(intersect, backpack3, std::back_inserter(result));
        char chr = result.front();
        sum += std::islower(chr) ? chr - 'a' + 1 : chr - 'A' + 27;
    }
    std::cout << sum << "\n";
}