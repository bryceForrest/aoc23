#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <chrono>

struct condition_record {
    condition_record() = default;

    condition_record(std::string input_state, std::vector<size_t> input_group) {
        state = input_state;
        group = input_group;
    }

    void unfold(size_t n) {
        auto temp_group = group;
        auto temp_state = state;
        // unfold input
        for (size_t i = 0; i < (n - 1); ++i) {
            state += "?" + temp_state;
            group.insert(group.end(), temp_group.begin(), temp_group.end());
        }
    }

    friend std::istream& operator >> (std::istream& is, condition_record& rec);
    friend std::ostream& operator << (std::ostream& os,
                                      const condition_record& rec);

    std::string state;
    std::vector<size_t> group;

};

std::istream& operator >> (std::istream& is, condition_record& rec) {
    rec.state = {};
    rec.group = {};

    std::string group_str;

    if (is >> rec.state >> group_str) {
        size_t pos = 0;
        size_t next;

        do {
            next = group_str.find(',', pos);
            std::string token = group_str.substr(pos, next - pos);
            pos = next + 1;
            rec.group.push_back((size_t)std::stoi(token));
        } while (next != std::string::npos);
    } else {
        is.setstate(std::ios::eofbit);
    }

    return is;
}

std::ostream& operator << (std::ostream& os, const condition_record& rec) {
    os << rec.state << " -- ";
    std::string sep = "";
    for (auto& num : rec.group) {
        os << sep << num;
        sep = ", ";
    }

    return os;
}

std::vector<condition_record> parse(std::string path);
size_t arrangement(condition_record rec);
std::string to_key(const std::string& state, const std::vector<size_t>& group);

int main() {
    std::string path = "/home/bforrest/aoc23/day12/input_p1.txt";
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<condition_record> records = parse(path);

    size_t score = 0;

    for (auto& rec : records) {
        // part 2
        rec.unfold(5);
        score += arrangement(rec);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (stop - start).count() / 1e9;

    std::cout << score << std::endl;
    std::cout << duration << std::endl;

    return 0;
}

std::vector<condition_record> parse(std::string path) {
    std::ifstream infile(path);
    condition_record rec;
    std::vector<condition_record> records;

    while (infile >> rec) {
        records.push_back(rec);
    }

    return records;
}

size_t arrangement(condition_record rec) {
    static std::unordered_map<std::string, size_t> memo;
    std::string key = to_key(rec.state, rec.group);

    if (rec.group.empty() && rec.state.empty()) return 1;

    if (!rec.group.empty() && rec.state.empty()) return 0;

    if (rec.group.empty() && !rec.state.empty()) {
        if (rec.state.find('#') == std::string::npos) {
            memo[key] = 1;
            return 1;
        }

        memo[key] = 0;
        return 0;
    }

    if (memo.find(key) != memo.end()) return memo[key];

    size_t score = 0;
    char current = rec.state.front();
    size_t value = rec.group.front();

    bool possible = (rec.state.size() >= value);

    if (possible) {
        switch (current) {
        case '#':
            for (size_t i = 0; possible && i < value; ++i) {
                possible = (rec.state[i] != '.');
            }

            possible = possible && (rec.state.size() == value || rec.state[value] != '#');

            if (!possible) {
                memo[key] = 0;
                return 0;
            }

            score += arrangement(condition_record(
                                     rec.state.substr(std::min(value + 1, rec.state.size())),
                                     std::vector<size_t> {rec.group.begin() + 1, rec.group.end()})
                                );
            break;

        case '?':
            score += arrangement(condition_record("#" + rec.state.substr(1), rec.group));
            score += arrangement(condition_record("." + rec.state.substr(1), rec.group));
            break;

        case '.':
            score += arrangement(condition_record(rec.state.substr(1), rec.group));
            break;

        default:
            break;
        }
    }

    memo[key] = score;

    return score;
}

std::string to_key(const std::string& state, const std::vector<size_t>& group) {
    std::string delim = "_";
    std::string group_str;

    for (auto& num : group) {
        group_str += delim + std::to_string(num);
    }

    return state + group_str;
}