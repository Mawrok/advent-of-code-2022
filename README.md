# Advent of Code 2022

My solutions for [Advent of Code 2022](https://adventofcode.com/2022/) in C++23

| Day |            Quest                     |
| :-: | :----------------------------------: |
|  1  | [Calorie Counting][1]                |
|  2  | [Rock Paper Scissors][2]             |
|  3  | [Rucksack Reorganization][3]         |
|  4  | Camp Cleanup                         |
|  5  | [Supply Stacks][5]                   |
|  6  | [Tuning Trouble][6]                  |
|  7  | [No Space Left On Device Trouble][7] |
|  8  | [Treetop Tree House][8]              |
|  9  | [Rope Bridge][9]                     |
|  10 | [Cathode-Ray Tube][10]               |

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
### Part 1 + 2 = 4
```cpp
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>

auto intersects(std::ranges::range auto v) {
    std::string tmp;
    std::string acc{ v[0] };
    std::ranges::sort(acc);
    for (auto next : v | std::views::drop(1)) {
        tmp.clear();
        std::ranges::sort(next);
        std::ranges::set_intersection(acc, next, std::back_inserter(tmp));
        std::swap(acc, tmp);
    }
    return acc.front();
}

int value(std::ranges::view auto backsacks) {
    auto value = [](auto chr) {return std::islower(chr) ? chr - 'a' + 1 : chr - 'A' + 27; };
    auto transformed = backsacks
        | std::views::transform([](auto c) {return intersects(c); })
        | std::views::transform(value);
    return std::ranges::fold_left(transformed, 0, std::plus{});
}

int main() {
    auto data = std::views::istream<std::string>(std::cin) | std::ranges::to<std::vector>();

    auto chunk3 = data | std::views::chunk(3);
    auto halves = [](std::string s) { 
        return std::vector{ s.substr(0, s.size() / 2u), s.substr(s.size() / 2u) };};
    auto halved = data | std::views::transform(halves);

    std::cout << value(halved) << "\n";
    std::cout << value(chunk3) << "\n";
}
```
---
### Day 5
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <iterator>
#include <functional>
using Crates = std::vector<std::vector<char>>;
using Stack = std::vector<char>;

auto load_crates() {
    Crates crates(10);
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) break;
        for (int i{ 1 }; i < line.size(); i += 4) {
            if (std::isalpha(line[i])) {
                crates[i / 4].push_back(line[i]);
            }
        }
    } std::cin.ignore(1, '\n');
    for (auto& crate : crates) { std::ranges::reverse(crate); }
    return crates;
}

void move(Stack& src, Stack& dest, int count) {
    dest.resize(dest.size() + count);
    std::move(src.end() - count, src.end(), 
        dest.end() - count);
    src.erase(src.end() - count, src.end());
}

void move_backward(Stack& src, Stack& dest, int count) {
    dest.resize(dest.size() + count);
    std::move_backward(src.end() - count, src.end(), 
        std::make_reverse_iterator(dest.end() - count));
    src.erase(src.end() - count, src.end());
}

void show(Crates const& crates) {
    auto c = crates | std::views::filter(std::not_fn(&Stack::empty));
    std::ranges::transform(c, 
        std::ostream_iterator<char>(std::cout), 
        [](Stack const& c) { return c.back(); }
    );
    std::cout << "\n";
}

int main() {
    auto crates = load_crates();
    auto crates_backward = crates;

    std::string _;
    int count, source, destination;
    while (std::cin >> _ >> count >> _ >> source >> _ >> destination) {
        move(crates[source - 1], crates[destination - 1], count);
        move_backward(crates_backward[source - 1], crates_backward[destination - 1], count);
    }

    show(crates);
    show(crates_backward);
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
auto rate_position(T trees, char value) {
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
### Day 9
```cpp
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <vector>
#include <ranges>

struct Vector2 {
    long hash() { return 1000l * x + y; } 
    int x{}, y{};
};

int sgn(int x) { return (x > 0) - (x < 0); }

void follow(Vector2& prev, Vector2& next) {
    auto dx = prev.x - next.x, dy = prev.y - next.y;
    bool move = std::abs(dx) == 2 or std::abs(dy) == 2;
    next.x += move * sgn(dx);
    next.y += move * sgn(dy);
}

void move(Vector2& pos, char dir) {
    pos.x += (dir == 'R') - (dir == 'L');
    pos.y += (dir == 'U') - (dir == 'D');
}

int main() {
    std::vector<Vector2> rope(10);
    std::unordered_set<long> traceTail;
    std::unordered_set<long> traceOneAfterHead;

    char direction;
    int moves;
    while (std::cin >> direction >> moves) {
        while (moves--) {
            move(rope.front(), direction);
            for (auto [prev, next] : rope | std::views::pairwise) {
                follow(prev, next);
            }
            traceOneAfterHead.insert(rope[1].hash());
            traceTail.insert(rope.back().hash());
        }
    }

    std::cout << traceOneAfterHead.size() << "\n";
    std::cout << traceTail.size() << "\n";
}
```
---
### Day 10
```cpp
#include <iostream>
#include <string>
#include <ranges>
#include <iterator>
#include <algorithm>
#include <cmath>

struct Computer {
    std::string screen;
    int x = 1, acc = 0, it = 0;

    void cycle() {
        if ((it + 21) % 40 == 0)
            acc += (it + 1) * x;   
        screen.append(1, std::abs(x - it % 40) < 2 ? '#' : '.');
        it++;
    }
};

int main() {
    Computer comp;
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "noop") {
            comp.cycle();
        } else {
            comp.cycle();
            comp.cycle();
            comp.x += std::stoi(line.substr(line.rfind(' ')));
        }
    }

    std::cout << comp.acc << "\n";
    for (auto row : comp.screen | std::views::chunk(40)) {
        std::ostream_iterator<char> out(std::cout << "\n");
        std::ranges::copy(row, out);
    }
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

[5]: #day-5
[6]: #day-6
[7]: #day-7
[8]: #day-8
[9]: #day-9
[10]: #day-10
