#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <complex>
#include <algorithm>

struct instr {
    std::complex<int64_t> dir;
    int64_t count;
    std::string color;

    friend std::istream& operator>>(std::istream& is, instr& i);

    static inline const std::unordered_map<char, std::complex<int64_t>> MAP {
        {'U', {0, -1}}, {'D', {0, 1}}, {'L', {-1, 0}}, {'R', {1, 0}}
    };
};

std::istream& operator>>(std::istream& is, instr& i) {
    // read obj from stream
    if (is >> std::ws && is.peek() == EOF) {
        is.setstate(std::ios::failbit);
    } else {
        char temp_dir;
        is >> temp_dir >> i.count;
        is.ignore(100, '(');
        std::getline(is, i.color, ')');
        i.dir = i.MAP.at(temp_dir);
    }

    return is;
}


std::vector<instr> parse(std::string path);
size_t part_1(const std::vector<instr>& plan);
size_t part_2(const std::vector<instr>& plan);
std::vector<instr> extract_correct(std::vector<instr> instructions);
size_t shoelace_pick(std::vector<std::complex<int64_t>> vertices);


int main() {
    std::string path = "/home/bforrest/aoc23/day18/input.txt";
    auto plan = parse(path);
    auto score_1 = part_1(plan);
    auto score_2 = part_2(plan);

    std::cout << "The solution for part 1 is: " << score_1 << std::endl;
    std::cout << "The solution for part 2 is: " << score_2 << std::endl;

    return 0;
}


std::vector<instr> parse(std::string path) {
    std::ifstream infile(path);
    std::vector<instr> result;
    instr temp;

    while (infile >> temp) {
        result.push_back(temp);
    }

    return result;
}


size_t part_1(const std::vector<instr>& plan) {
    std::complex<int64_t> pos = {0, 0};
    std::vector<std::complex<int64_t>> vertices = {pos};

    for (auto& p : plan) {
        pos += p.count * p.dir;
        vertices.push_back(pos);
    }

    return shoelace_pick(vertices);
}


size_t part_2(const std::vector<instr>& plan) {
    std::complex<int64_t> pos = {0, 0};
    std::vector<std::complex<int64_t>> vertices = {pos};

    for (auto& p : extract_correct(plan)) {
        pos += p.count * p.dir;
        vertices.push_back(pos);
    }

    return shoelace_pick(vertices);
}


std::vector<instr> extract_correct(std::vector<instr> instructions) {
    static const std::vector<char> val_to_dir = {
        'R', 'D', 'L', 'U'
    };

    std::vector<instr> corrected;

    for (auto& i : instructions) {
        instr temp;
        std::string hex_dist = {i.color.begin() + 1, i.color.begin() + 6};
        size_t direction = i.color.back() - '0';

        temp.count = std::stoull(hex_dist, nullptr, 16);
        temp.dir = temp.MAP.at(val_to_dir[direction]);

        corrected.push_back(temp);
    }

    return corrected;
}


size_t shoelace_pick(std::vector<std::complex<int64_t>> vertices) {
    int64_t area = 0;
    int64_t interior = 0;
    int64_t perimeter = 0;

    for (size_t i = 1; i < vertices.size(); ++i) {
        perimeter += abs(vertices[i] - vertices[i - 1]);
        area += (vertices[i - 1].real() * vertices[i].imag()) -
                (vertices[i].real() * vertices[i - 1].imag());
    }

    area = (llabs(area) + 1) / 2;
    interior = area - (perimeter / 2) + 1;

    return perimeter + interior;
}