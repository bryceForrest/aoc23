#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <complex>
#include <algorithm>

using Part = std::unordered_map<char, size_t>;
using Workflows =
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;


std::istream& operator>>(std::istream& is, Workflows& w);
std::istream& operator>>(std::istream& is, Part& p);
std::vector<Part> parse(std::string path);

int main() {
    std::string path = "/home/bforrest/aoc23/day19/sample_input.txt";
    auto parts = parse(path);

    return 0;
}

std::istream& operator>>(std::istream& is, Workflows& w) {
    std::string temp_str;

    if (is >> std::ws && is.peek() == EOF) {
        is.setstate(std::ios::failbit);
    } else if (std::getline(is, temp_str) && (temp_str != "\n")) {
        auto start_pos = temp_str.find('{');
        auto cur_key = temp_str.substr(0, start_pos);
        auto& cur = w[cur_key];

        while (start_pos != std::string::npos) {
            std::string condition;
            std::string send;
            auto end_pos = temp_str.find(':', start_pos);

            if (end_pos == std::string::npos) {
                condition = "else";

                send = {
                    temp_str.begin() + start_pos + 1, temp_str.end() - 1
                };
            } else {
                condition = {
                    temp_str.begin() + start_pos + 1, temp_str.begin() + end_pos
                };

                start_pos = temp_str.find(',', end_pos);

                send = {
                    temp_str.begin() + end_pos, temp_str.begin() + start_pos
                };
            }

            cur[condition] = send;
        }

    }

    return is;
}

std::istream& operator>>(std::istream& is, Part& p) {
    // read obj from stream
    if (is >> std::ws && is.peek() == EOF) {
        is.setstate(std::ios::failbit);
    } else {
        std::string temp_str;

        is.ignore(100, '=');
        std::getline(is, temp_str, ',');
        p['x'] = std::stoull(temp_str);

        is.ignore(100, '=');
        std::getline(is, temp_str, ',');
        p['m'] = std::stoull(temp_str);

        is.ignore(100, '=');
        std::getline(is, temp_str, ',');
        p['a'] = std::stoull(temp_str);

        is.ignore(100, '=');
        std::getline(is, temp_str, '}');
        p['s'] = std::stoull(temp_str);

        is >> std::ws;
    }

    return is;
}

std::vector<Part> parse(std::string path) {
    std::ifstream infile(path);
    std::vector<Part> result;
    Part temp;

    while (infile >> temp) {
        result.push_back(temp);
    }

    return result;
}