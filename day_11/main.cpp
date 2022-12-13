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
