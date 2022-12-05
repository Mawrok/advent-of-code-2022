#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
	std::vector<int> vec;
	int calories{};
	for (std::string line; std::getline(std::cin, line);) {
		if (line.empty()) {
			vec.emplace_back(calories);
			calories = 0;
		} else {
			calories += std::stoi(line);
		}
  }
	auto three = vec.begin() + 3;
	std::ranges::nth_element(vec, three, std::ranges::greater{});
	std::cout << std::accumulate(vec.begin(), three, 0) << "\n"; 
}
