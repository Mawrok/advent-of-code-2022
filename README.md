# Advent of Code 2022

My solutions for [Advent of Code 2022](https://adventofcode.com/2022/) in C++23

| Day |            Quest             |
| :-: | :--------------------------: |
|  1  | [Calorie Counting][1]        |
|  2  | [Rock Paper Scissors][2]     |
|  3  | [Rucksack Reorganization][3] |
|  4  | [Camp Cleanup][4]            |
|  5  | [Supply Stacks][5]           |
|  6  | [Tuning Trouble][6]          |

## Solutions
### Day 1: 
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

    std::cout << vec.front() << "\n";
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
### Day 3
#### Part 1
```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
  std::string backpack;
  int sum{};
  while (std::cin >> backpack) {
    std::string intersection;
    auto half = backpack.begin() + backpack.size() / 2;
    std::sort(backpack.begin(), half);
    std::sort(half, backpack.end());
    std::set_intersection(backpack.begin(), half, half, backpack.end(), std::back_inserter(intersection));
    char chr = intersection.front();
    sum += std::islower(chr) ? chr - 'a' + 1 : chr - 'A' + 27;
  }
  std::cout << sum << "\n";
}
```
#### Part 2
```cpp
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
```
---
### Day 6
```cpp
#include <iostream>
#include <algorithm>
#include <ranges>
#include <set>
#include <iterator>
#include <string>

int find_start_marker(std::string_view sequence, int size) {
    auto slide = sequence | std::views::slide(size);
    auto unique = [&](auto view) { return std::ranges::to<std::set>(view).size() == size; };
    auto found = std::ranges::find_if(slide, unique);
    return size + std::distance(slide.begin(), found);
}

int main() {
    std::string sequence;
    std::cin >> sequence;
    std::cout << find_start_marker(sequence, 4) << "\n";
    std::cout << find_start_marker(sequence, 14) << "\n";
}    
```
---
### How to pipe
```
< input.txt > output.txt
```

[1]: #day-1 
[2]: #day-2
[3]: #day-3


[6]: #day-6
