#include <iostream>
#include <sstream>
#include <variant>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <iterator>
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
