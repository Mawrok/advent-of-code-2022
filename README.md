# Advent of Code 2022

My solutions for [Advent of Code 2022](https://adventofcode.com/2022/) in C++23

| Day |            Quest                     |
| :-: | :----------------------------------: |
|  1  | [Calorie Counting][1]                |
|  2  | [Rock Paper Scissors][2]             |
|  3  | [Rucksack Reorganization][3]         |
|  4  | Camp Cleanup                         |
|  5  | Supply Stacks                        |
|  6  | [Tuning Trouble][6]                  |
|  7  | [No Space Left On Device Trouble][7] |
|  8  | [Treetop Tree House][8]              |

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
        auto mid = backpack.begin() + backpack.size() / 2;
        std::sort(backpack.begin(), mid);
        std::sort(mid, backpack.end());
        std::set_intersection(backpack.begin(), mid, mid, backpack.end(), std::back_inserter(intersection));
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
### Day 7
```cpp
#include <iostream>     
#include <ranges> 
#include <string> 
#include <algorithm> 
#include <vector> 
#include <unordered_map>
#include <functional> 

int main() { 
    std::unordered_map<std::string, int> dir_size;
    std::vector<std::string> dirs_stack;
    
    for (std::string line, path; std::getline(std::cin, line); path.clear())
        if (line.starts_with("$ cd .."))
            dirs_stack.pop_back();
        else if (line.starts_with("$ cd"))
            dirs_stack.push_back(line.substr(5));
        else if (line.front() != '$' and not line.starts_with("dir"))
            for (auto size = std::stoi(line.substr(0, line.find(' '))); auto& dir : dirs_stack)
                dir_size[path += dir] += size;

    auto values = std::views::values(dir_size);

    int sum = std::ranges::fold_left(
        std::views::filter(values, std::bind_back(std::less{}, 100'000)), 0, std::plus{});
    std::cout << sum << "\n";
    
    int disk_size = 70'000'000;
    int update_size = 30'000'000;
    int occupied_size = dir_size["/"];

    int minimal_capacity = update_size + occupied_size - disk_size;
    int min = std::ranges::min(
        std::views::filter(values, std::bind_back(std::greater{}, minimal_capacity)));
    std::cout << min << '\n';
}
```
---
### Day 8
```cpp
#include <iostream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <utility>
#include <string>
#include <functional>

class ForestView {
    std::string_view data;
    std::size_t width;
public:
    ForestView(std::string_view sv) :data(sv), width(std::sqrt(sv.size())) {}

    auto rows(std::size_t i) { return data | std::views::drop(i / width * width) | std::views::take(width); }
    auto cols(std::size_t i) { return data | std::views::drop(i % width) | std::views::stride(width); }

    auto left(std::size_t i) { return rows(i) | std::views::take(i % width) | std::views::reverse; }
    auto right(std::size_t i) { return rows(i) | std::views::drop(1 + i % width); }
    auto up(std::size_t i) { return cols(i) | std::views::take(i / width) | std::views::reverse; }
    auto down(std::size_t i) { return cols(i) | std::views::drop(1 + i / width); }
};

template <typename T>
auto rate_position(T trees, int value) {
    auto it = std::ranges::find_if(trees, std::bind_back(std::greater_equal{}, value));
    bool visibility = it == trees.end();
    std::size_t scenic_score = 1 + std::distance(trees.begin(), it) - visibility;
    return std::tuple(visibility, scenic_score);
}

int main() {
    auto data = std::views::istream<char>(std::cin) | std::ranges::to<std::string>();
    std::size_t count{}, max{};
    ForestView forest{ data };

    for (auto i : std::views::iota(0uz, data.size())) {
        auto [visibility1, score1] = rate_position(forest.left(i), data[i]);
        auto [visibility2, score2] = rate_position(forest.right(i), data[i]);
        auto [visibility3, score3] = rate_position(forest.up(i), data[i]);
        auto [visibility4, score4] = rate_position(forest.down(i), data[i]);
        count += (visibility1 | visibility2 | visibility3 | visibility4);
        max = std::max(max, score1 * score2 * score3 * score4);
    }

    std::cout << count << "\n";
    std::cout << max << "\n";
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
[7]: #day-7
[8]: #day-8
