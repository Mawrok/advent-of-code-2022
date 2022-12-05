# Advent of Code 2022

My solutions for [Advent of Code 2022](https://adventofcode.com/2022/) in C++

| Day |            Quest             |
| :-: | :--------------------------: |
|  1  | [Calorie Counting][1]        |
|  2  | [Rock Paper Scissors][2]     |

## Solutions

### Day 1: 
#### Part 1:
```cpp
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
```
#### Part 2:
```cpp
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
```
---

### Day 2:
#### Part 1:
```cpp
#include <iostream>

int main() {
    char enemyChoice, playerChoice;
    int score{}, result;
    while (std::cin >> enemyChoice >> playerChoice) {
        enemyChoice  -= 'A';
        playerChoice -= 'X';
        result = (4 + playerChoice - enemyChoice) % 3;
        score += 3 * result + playerChoice + 1;
    }
    std::cout << score << "\n";
}
```
#### Part 2:
```cpp
#include <iostream>

int main() {
    char enemyChoice, playerChoice, result;
    int score{};
    while (std::cin >> enemyChoice >> result) {
        enemyChoice  -= 'A';
        result       -= 'X';
        playerChoice = (2 + result + enemyChoice) % 3; 
        score += 3 * result + playerChoice + 1;
    }
    std::cout << score << "\n";
}
```
---
### How to pipe
```
< input.txt > output.txt
```

[1]: #day-1 
[2]: #day-2
[3]: #day-2
[4]: #day-2
