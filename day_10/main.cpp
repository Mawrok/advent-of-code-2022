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