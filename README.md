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
|  11 | [Monkey in the Middle][11]           |
|  12 | [Hill Climbing Algorithm][12]        |
|  13 | [Distress Signal][13]                |

## Solutions
### Day 1: 
```cpp
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

int acc(std::ranges::view auto backsacks) {
    auto value = [](auto chr) {return std::islower(chr) ? chr - 'a' + 1 : chr - 'A' + 27; };
    auto transformed = backsacks
        | std::views::transform([](auto c) {return intersects(c); })
        | std::views::transform(value);
    return std::ranges::fold_left(transformed, 0, std::plus{});
}

int main() {
    auto data = std::views::istream<std::string>(std::cin) | std::ranges::to<std::vector>();
    auto by_half = [](std::string s) {
        return std::vector{ s.substr(0, s.size() / 2u), s.substr(s.size() / 2u) }; };
        
    std::cout << acc(std::views::transform(data, by_half)) << "\n";
    std::cout << acc(std::views::chunk(data, 3)) << "\n";
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
    } 
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
### Day 11
```cpp
#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <vector>
#include <functional>
#include <cinttypes>
using int_t = std::uint64_t;
using func_t = std::function<int_t(int_t)>;
using func_bt = std::function<int_t(int_t, int_t)>;

struct Monkey {
    std::vector<int_t> items;
    func_t operation;
    int_t divisor;
    std::size_t target1;
    std::size_t target2;
    std::size_t inspections{};

    std::size_t choosing_catcher(int_t worry_level) {
        return worry_level % divisor == 0 ? target1 : target2;
    }

    int_t inspect(int_t item) {
        return ++inspections, operation(item);
    }
};

struct MonkeysPack {
    std::vector<Monkey> monkeys;
    func_t reduce;

    MonkeysPack(std::vector<Monkey> data, func_t&& func)
        :monkeys(data), reduce(func) {
    }

    int_t monkey_business(int rounds) {
        while (rounds--)
            perform_round();
        std::ranges::partial_sort(monkeys, monkeys.begin() + 2, std::greater{}, &Monkey::inspections);
        return monkeys[1].inspections * monkeys[0].inspections;
    }

    void perform_round() {
        for (auto& monkey : monkeys) {
            while (not monkey.items.empty()) {
                auto pop = monkey.items.back(); 
                auto worry_level = reduce(monkey.inspect(pop));
                auto catcher = monkey.choosing_catcher(worry_level);
                monkeys[catcher].items.push_back(worry_level);
                monkey.items.pop_back();
            }
        }
    }
};

auto split(std::string s_nums) {
    return s_nums
        | std::views::split(',')
        | std::views::transform(
            [](auto s) -> int_t { return std::stoi(std::string(s.begin(), s.end())); })
        | std::ranges::to<std::vector>();
}

func_t op_parse(std::string str) {
    using namespace std::placeholders;
    func_bt add = std::plus<int_t>();
    func_bt mul = std::multiplies<int_t>();

    auto opn = str.at(str.find(' ') + 1);
    auto rhs = str.substr(str.rfind(' ') + 1);

    auto op = opn == '+' ? add : mul;
    if (rhs == "old")
        return std::bind(op, _1, _1);
    else
        return std::bind(op, _1, std::stoi(rhs));
}

int main() {
    std::vector<Monkey> data;
    for (std::string line; std::getline(std::cin, line); std::cin.get()) {
        static auto trim_after = [&line](auto chr) { return
            std::getline(std::cin, line), line.substr(line.rfind(chr) + 2); };
        auto& monkey = data.emplace_back();
        monkey.items = split(trim_after(':'));
        monkey.operation = op_parse(trim_after('='));
        monkey.divisor = std::stoi(trim_after('y'));
        monkey.target1 = std::stoi(trim_after('y'));
        monkey.target2 = std::stoi(trim_after('y'));
    }

    int_t mod = std::ranges::fold_left(std::views::transform(data, &Monkey::divisor), int_t{ 1 }, std::multiplies{});
    MonkeysPack monkeys1(data, std::bind_back(std::divides{}, 3));
    MonkeysPack monkeys2(data, std::bind_back(std::modulus{}, mod));

    std::cout << monkeys1.monkey_business(20) << '\n';
    std::cout << monkeys2.monkey_business(10'000) << '\n';
}
```
---
### Day 12
```cpp
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>

struct GridPos {
    int row, col;
    GridPos(int row = 0, int col = 0) : row(row), col(col) {}
    using hash = std::hash<long>;
    operator long() const { return row + 1000 * col; }
    static const std::vector<GridPos> directions;
};

const std::vector<GridPos> GridPos::directions{ GridPos(0, -1), GridPos(-1, 0), GridPos(0,  1), GridPos(1, 0) };

struct Grid {
    std::string data;
    int width, height;
    Grid(int width, int height, std::string s) :data(s), width(width), height(height) {}
    GridPos start() { auto i = data.find('S'); return access(i); }
    GridPos end() { auto i = data.find('E'); return access(i); };
    char operator()  (GridPos const& p) const { return data[p.col * width + p.row]; };
    char& operator() (GridPos const& p) { return data[p.col * width + p.row]; };
    GridPos access(std::size_t i) { return GridPos(i % width, i / width); }
    auto in_bounds(auto const& p) const { return p.row >= 0 && p.col >= 0 && p.row < width&& p.col < height; };
};

template <typename T, typename F>
int bfs(GridPos start, GridPos end, Grid const& grid, T& finish, F& not_passable) {
    std::unordered_set<GridPos, GridPos::hash> visited;
    std::queue<std::pair<GridPos, int>> queue;
    queue.emplace(start, 0);
    visited.emplace(start);
    int result{ -1 };
    while (not queue.empty()) {
        auto const [current, deep] = queue.front(); queue.pop();
        for (auto const& dir : GridPos::directions) {
            GridPos next(current.row + dir.row, current.col + dir.col);
            if (not grid.in_bounds(next))
                continue;
            if (visited.contains(next))
                continue;
            if (not_passable(current, next))
                continue;
            queue.emplace(next, deep + 1);
            visited.emplace(next);
        }

        if (finish(current)) {
            result = deep;
            break;
        }
    }

    return result;
}

int main() {
    std::string data;
    int width{}, height{};
    for (std::string line; std::getline(std::cin, line); ++height, width = line.size())
        std::ranges::copy(line, std::back_inserter(data));

    Grid grid(width, height, data);
    auto start = grid.start(); grid(start) = 'a';
    auto end = grid.end(); grid(end) = 'z';

    auto same_pos = [&](GridPos const& current) { return current == end; };
    auto same_val = [&grid](GridPos const& current) { return grid(current) == 'a'; };
    auto less = [&grid](GridPos const& lhs, GridPos const& rhs) { return grid(lhs) + 1 < grid(rhs); };
    auto greater = [&grid](GridPos const& lhs, GridPos const& rhs) { return grid(lhs) - 1 > grid(rhs); };

    std::cout << bfs(start, end, grid, same_pos, less) << "\n";
    std::cout << bfs(end, start, grid, same_val, greater) << "\n";
}
```
---
### Day 13
```cpp
#include <iostream>
#include <sstream>
#include <variant>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <cctype>
#include <functional>

struct TreeNodeV : public std::variant<int, std::vector<TreeNodeV>> {
    using vec = std::vector<TreeNodeV>;
    TreeNodeV() : variant(0) {}
    TreeNodeV(int val) : variant(val) {}
    TreeNodeV(vec const& val) : variant(val) {}
    bool operator<(const TreeNodeV& rhs) const;
};

struct Visitor {
    bool operator()(int lhs, int rhs) const {
        return lhs < rhs;
    }
    bool operator()(int lhs, TreeNodeV::vec const& rhs) const {
        return operator()(TreeNodeV::vec{ lhs }, rhs);
    }
    bool operator()(TreeNodeV::vec const& lhs, int rhs) const {
        return operator()(lhs, TreeNodeV::vec{ rhs });
    }
    bool operator()(TreeNodeV::vec const& lhs, TreeNodeV::vec const& rhs) const {
        return std::ranges::lexicographical_compare(lhs, rhs);
    }
} compareVisitor;

bool TreeNodeV::operator<(TreeNodeV const& rhs) const {
    return std::visit(compareVisitor, *this, rhs);
}

TreeNodeV parse(std::istream& in) {
    if (std::isdigit(in.peek())) {
        int value;
        in >> value;
        return value;
    }

    if (in.get() == '[' and in.get() == ']') {
        return TreeNodeV::vec{};
    }

    in.unget(); in.unget();
    std::vector<TreeNodeV> vec;
    while (in.peek() != ']') {
        in.get();
        vec.push_back(parse(in));
    }
    in.get();
    return vec;
}

int main() {
    auto lines = std::views::istream<std::string>(std::cin) | std::ranges::to<std::vector>();

    TreeNodeV::vec main;
    for (auto& line : lines) {
        std::istringstream iss(line);
        main.emplace_back(parse(iss));
    }

    int sum{};
    for (int i = 1; auto pair : main | std::views::chunk(2)) { 
        sum += (pair.front() < pair.back()) * i++;           
    }

    TreeNodeV value2 = main.emplace_back(2);
    TreeNodeV value6 = main.emplace_back(6);
    auto count2 = 1 + std::ranges::count_if(main, std::bind_back(std::less{}, value2));
    auto count6 = 1 + std::ranges::count_if(main, std::bind_back(std::less{}, value6));

    std::cout << sum << "\n";
    std::cout << count2 * count6 << "\n";
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
[11]: #day-11
[12]: #day-12
[13]: #day-13
