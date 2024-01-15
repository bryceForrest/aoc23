#include <iostream>
#include <fstream>
#include <cmath>
#include <utility>
#include <vector>
#include <tuple>

std::vector<size_t> read_line_p1(std::ifstream& infile);
size_t read_line_p2(std::ifstream& infile);
std::pair<double, double> find_roots(long long int b, long long int c);
size_t part_1(std::vector<size_t> time, std::vector<size_t> distance);
size_t count_possibilites(size_t time, size_t distance);

const double EPS = 1e-10;

int main() {
    std::string path = "/home/bforrest/aoc23/day6/input_p1.txt";
    std::ifstream infile(path);

    // auto time = read_line_p1(infile);
    // auto distance = read_line_p1(infile);

    // auto score = part_1(time, distance);

    auto time = read_line_p2(infile);
    auto distance = read_line_p2(infile);

    auto score = count_possibilites(time, distance);

    std::cout << score << std::endl;
    return 0;
}

std::vector<size_t> read_line_p1(std::ifstream& infile) {
    std::vector<size_t> vec;
    size_t value;

    infile.ignore(100, ' ');
    while ((infile.peek() != '\n') && (infile >> value)) {
        vec.push_back(value);
    }

    return vec;
}

size_t read_line_p2(std::ifstream& infile) {
    std::string value;
    std::string str_val;

    infile.ignore(100, ' ');
    while ((infile.peek() != '\n') && (infile >> value)) {
        str_val += value;
    }

    return std::stoull(str_val);
}

std::pair<double, double> find_roots(long long int b, long long int c) {
    double vertex = (double)(-b)/(2);
    double left_root = vertex - std::sqrt(std::pow(vertex, 2) - c);
    double right_root = vertex + std::sqrt(std::pow(vertex, 2) - c);

    return std::pair<double, double> {left_root, right_root};
}

size_t part_1(std::vector<size_t> time, std::vector<size_t> distance) {
    size_t score = 1;

    for (size_t i = 0; i < time.size(); ++i) {
        int possibilities = count_possibilites(time[i], distance[i]);

        score *= possibilities;
    }

    return score;
}

size_t count_possibilites(size_t time, size_t distance) {
    size_t possibilities;
    auto roots = find_roots(-(time), distance);
    size_t lower_bound = std::max(0, (int)std::ceil(roots.first + EPS));
    size_t upper_bound = std::max(0, (int)std::floor(roots.second - EPS));

    if (lower_bound == upper_bound) {
        possibilities = 0;
    } else {
        possibilities = upper_bound - lower_bound + 1;
    }

    return possibilities;
}