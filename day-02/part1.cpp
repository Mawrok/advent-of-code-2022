#include <iostream>

int main() {
	char enemyChoice, playerChoice;
	int score{};
	while (std::cin >> enemyChoice >> playerChoice) {
		enemyChoice  -= 'A';
		playerChoice -= 'X';
		int result = (3 + playerChoice - enemyChoice + 1) % 3;
		score += 3 * result + playerChoice + 1;
	}
	std::cout << score << "\n";
}
