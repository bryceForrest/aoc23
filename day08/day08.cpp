#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <numeric>
#include <algorithm>

using Network =
    std::unordered_map<std::string, std::pair<std::string, std::string>>;

size_t part_1(std::string path);
size_t part_2(std::string path);
std::istream& operator>>(std::istream& is, Network& network);
size_t traverse(const Network& network, std::vector<char> instructions,
                const std::string& key, const size_t& index) ;
size_t ghost_traverse(const Network& network, std::vector<char> instructions,
                      const std::string& key, const size_t& index);

int main() {
    std::string path = "/home/bforrest/aoc23/day8/input.txt";
    auto steps_1 = part_1(path);
    auto steps_2 = part_2(path);

    std::cout << "The solution for part 1 is: " << steps_1 << std::endl;
    std::cout << "The solution for part 2 is: " << steps_2 << std::endl;
    return 0;
}

size_t part_1(std::string path) {
    std::ifstream infile(path);
    std::vector<char> instructions;
    Network network;
    size_t steps;
    while (infile.peek() != '\n') {
        instructions.push_back(infile.get());
    }

    while (infile >> network) {}

    steps = traverse(network, instructions, "AAA", 0);

    infile.close();

    return steps;
}

size_t part_2(std::string path) {
    std::ifstream infile(path);
    std::vector<char> instructions;
    std::vector<std::string> keys;
    Network network;
    size_t steps;

    while (infile.peek() != '\n') {
        instructions.push_back(infile.get());
    }

    while (infile >> network) {}

    for (auto& node : network) {
        if (node.first.back() == 'A') {
            keys.push_back(node.first);
        }
    }

    std::vector<size_t> path_steps;
    auto lcm = [](size_t n, size_t m) {return std::lcm(n, m);};

    for (auto& key : keys) {
        path_steps.push_back(ghost_traverse(network, instructions, key, 0));
    }

    infile.close();
    steps = std::accumulate(path_steps.begin(), path_steps.end(),
                            path_steps.front(), lcm);

    return steps;
}

std::istream& operator>>(std::istream& is, Network& network) {
    std::string key;
    std::string left;
    std::string right;
    std::string temp;

    is >> key;

    std::getline(is, temp, '(');
    std::getline(is, left, ',');
    is >> std::ws;
    std::getline(is, right, ')');

    if (is.good()) {
        network[key] = std::pair<std::string, std::string> {left, right};
    }

    return is;
}

size_t traverse(const Network& network, std::vector<char> instructions,
                const std::string& key, const size_t& index) {
    if (key == "ZZZ") {
        return 0;
    }

    char current_instr = instructions[index];

    std::string next;

    switch (current_instr) {
    case 'L':
        next = network.at(key).first;
        break;
    case 'R':
        next = network.at(key).second;
        break;
    }

    return 1 + traverse(network, instructions, next,
                        (index + 1) % instructions.size());
}

size_t ghost_traverse(const Network& network, std::vector<char> instructions,
                      const std::string& key, const size_t& index) {
    if (key.back() == 'Z') {
        return 0;
    }

    char current_instr = instructions[index];

    std::string next;

    switch (current_instr) {
    case 'L':
        next = network.at(key).first;
        break;
    case 'R':
        next = network.at(key).second;
        break;
    }

    return 1 + ghost_traverse(network, instructions, next,
                              (index + 1) % instructions.size());
}