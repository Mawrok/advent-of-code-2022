#include <iostream>

int main() {
	char enemyChoice, playerChoice, result;
	int score{};
	while (std::cin >> enemyChoice >> result) {
		enemyChoice  -= 'A';
		result -= 'X';
		playerChoice = (3 + enemyChoice + result - 1) % 3; 
		score += 3 * result + playerChoice + 1;
	}
	std::cout << score << "\n";
}
