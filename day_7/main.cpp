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