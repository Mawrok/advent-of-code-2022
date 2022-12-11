#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> vec(1, 0);
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            vec.emplace_back(0);
        } else {
            vec.back() += std::stoi(line);
        }
    }
    auto three = vec.begin() + 3;
    std::ranges::nth_element(vec, three, std::greater{});

    std::cout << vec.front() << "\n";
    std::cout << std::reduce(vec.begin(), three) << "\n";
}