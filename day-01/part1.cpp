#include <iostream>
#include <string>
#include <algorithm>

int main() {
	int calories{}, maxCalories{};
	for (std::string line; std::getline(std::cin, line);) {
		if (line.empty()) {
			maxCalories = std::max(maxCalories, calories);
			calories = 0;
		} else {
			calories += std::stoi(line);
		}
	}
	std::cout << maxCalories << "\n";
}
