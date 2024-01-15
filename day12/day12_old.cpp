#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

int part_1(std::string path);
int part_2(std::string path);
std::vector<int> split(std::string str, std::string delim);
void find_constraints(const std::string& input,
                      const std::vector<int>& values,
                      std::vector<int>& max_constraint,
                      std::vector<int>& min_constraint,
                      const size_t& upper_bound);
size_t valid_pos(const std::string& input, int valid_count);
void find_min_constraint(const std::string& input,
                         const std::vector<int>& values,
                         std::vector<int>& min_constraint);
void find_max_constraint(const std::string& input,
                         const std::vector<int>& values,
                         std::vector<int>& max_constraint,
                         std::vector<int>& min_constraint,
                         const size_t& upper_bound);
void find_arrangements(std::vector<std::string>& arrangements,
                       std::vector<int> current_tuple,
                       const std::vector<int>& values,
                       const int& k, const int& n,
                       const std::vector<int>& min_constraint,
                       const std::vector<int>& max_constraint) ;
int sum(std::vector<int> nums, int offset = 0);
int test_arrangements(std::vector<std::string>& arrangements,
                      const std::string& input);
std::string build_string(const std::vector<int> bucket,
                         const std::vector<int>& values);
bool check_string(const std::string& input,
                  const std::string& test_arrangement);


template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr);


int main() {
    std::string path = "/home/bforrest/aoc23/day12/sample_input_p1.txt";
    auto start = std::chrono::high_resolution_clock::now();
    int score = part_1(path);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (stop - start).count() / 1e9;
    std::cout << score << std::endl;
    std::cout << duration << std::endl;

    return 0;
}

int part_1(std::string path) {
    std::ifstream infile(path);
    std::string input_str;
    std::string group_str;
    int score = 0;

    while (infile >> input_str >> group_str) {
        std::vector<int> groups = split(group_str, ",");

        std::vector<int> buckets;
        int n_spaces = input_str.size() - sum(groups);
        int n_buckets = groups.size() + 1;

        std::vector<int> min_constraint(groups.size() + 1, 1);
        std::vector<int> max_constraint(groups.size() + 1, n_spaces);
        find_constraints(input_str, groups, max_constraint, min_constraint,
                         n_spaces);
        // find_min_constraint(input_str, groups, min_constraint);
        // find_max_constraint(input_str, groups, max_constraint, min_constraint,
        //                     n_spaces);

        std::cout << min_constraint;
        std::cout << max_constraint;

        std::vector<std::string> arrangements;
        find_arrangements(arrangements, buckets, groups,
                          n_buckets, n_spaces,
                          min_constraint, max_constraint);

        score += test_arrangements(arrangements, input_str);
    }

    return score;
}

int part_2(std::string path) {
    std::ifstream infile(path);
    std::string input_str_folded;
    std::string group_str_folded;
    int score = 0;

    while (infile >> input_str_folded >> group_str_folded) {
        std::vector<int> groups_folded = split(group_str_folded, ",");

        auto input_str = input_str_folded;
        auto groups = groups_folded;

        // unfold input
        for (size_t i = 0; i < 4; ++i) {
            input_str += "?" + input_str_folded;
            groups.insert(groups.end(), groups_folded.begin(), groups_folded.end());
        }

        std::vector<int> buckets;
        int n_spaces = input_str.size() - sum(groups);
        int n_buckets = groups.size() + 1;

        std::vector<int> min_constraint(groups.size() + 1, 1);
        std::vector<int> max_constraint(groups.size() + 1, n_spaces);

        find_min_constraint(input_str, groups, min_constraint);
        find_max_constraint(input_str, groups, max_constraint, min_constraint,
                            n_spaces);

        std::cout << input_str << std::endl;
        std::cout << min_constraint;
        std::cout << max_constraint;
        std::cout << n_spaces << " " << n_buckets << std::endl;

        std::vector<std::string> arrangements;
        find_arrangements(arrangements, buckets, groups,
                          n_buckets, n_spaces,
                          min_constraint, max_constraint);

        score += test_arrangements(arrangements, input_str);
    }

    return score;
}

std::vector<int> split(std::string str, std::string delim) {
    size_t pos = 0;
    size_t prev = 0;
    std::vector<int> result;

    do {
        pos = str.find(delim, prev);
        std::string token = str.substr(prev, pos - prev);
        prev = pos + delim.length();
        result.push_back(std::stoi(token));
    } while (pos != std::string::npos);

    return result;
}

void find_constraints(const std::string& input,
                      const std::vector<int>& values,
                      std::vector<int>& max_constraint,
                      std::vector<int>& min_constraint,
                      const size_t& upper_bound) {

    int total_chars = sum(values);
    int running_total = 0;
    int front_offset = 0;


    for (size_t i = 1; i < min_constraint.size(); ++i) {
        running_total += values[i - 1];
        int needed_chars = total_chars - running_total;

        size_t back_offset = valid_pos(input, needed_chars);

        std::string current (input.begin() + front_offset, input.begin() + back_offset);
        int min_pos = std::max((int)current.find_first_not_of('.'), 0) + (int)(i > 1);
        int max_pos = std::max((int)current.find_last_not_of('.'), 0) + (int)(i > 1);
        bool singleton = min_pos == max_pos;

        // min_pos = (i == 0) ? std::max(min_pos, 0) : std::max(min_pos, 1);
        // max_pos = (max_pos == -1 || singleton) ? min_pos : max_pos;
        min_constraint[i - 1] = min_pos;
        max_constraint[i - 1] = max_pos;

        front_offset += values[i] + min_constraint[i - 1] + 1;
    }

    max_constraint.back() = (input.size() - 1) - input.rfind('#');
    min_constraint.back() = (input.size() - 1) - std::max(input.rfind('#'),
                            input.rfind('?'));
}

size_t valid_pos(const std::string& input, int valid_count) {
    int count = 0;
    auto it = input.rbegin();

    while (it != input.rend() && count < valid_count) {
        count += ((*it == '#') || (*it == '?'));
        ++it;
    }

    return input.rend() - it;
}

void find_min_constraint(const std::string& input,
                         const std::vector<int>& values,
                         std::vector<int>& min_constraint) {
    size_t pos = std::min(input.find('#'), input.find('?'));
    min_constraint.front() = pos;

    pos = std::min(input.rfind('#'), input.rfind('?'));
    min_constraint.back() = pos;
}

void find_max_constraint(const std::string& input,
                         const std::vector<int>& values,
                         std::vector<int>& max_constraint,
                         std::vector<int>& min_constraint,
                         const size_t& upper_bound) {
    int running_total = 0;
    size_t prev_pos = 0;

    for (size_t i = 0; i < values.size(); ++i) {
        std::string current = input.substr(running_total + i);
        auto pos = input.find(std::string(values[i], '#'), running_total + i);


        if ((pos - running_total - i) <= values[i]) {
            auto last_unknown = input.find_last_not_of('.', std::max((size_t)0,
                                (size_t)((int)pos - 2)));
            // min_constraint[i] = pos - last_unknown;
            max_constraint[i] = pos - last_unknown;
        } else {
            auto last_unknown = current.find_last_of('?',
                                std::max((int)0, (int)pos - 2));
            max_constraint[i] = std::min(upper_bound, running_total + i + last_unknown);
        }

        running_total += values[i];
    }

    max_constraint.back() = (input.size() - 1) - input.find_last_of('#');

    // for (size_t i = 0; i < max_constraint.size(); ++i) {
    //     auto temp = min_constraint;
    //     temp[i] = 0;
    //     auto total = sum(temp);
    //     max_constraint[i] = upper_bound - total;
    // }
}

void find_arrangements(std::vector<std::string>& arrangements,
                       std::vector<int> current_tuple,
                       const std::vector<int>& values,
                       const int& k, const int& n,
                       const std::vector<int>& min_constraint,
                       const std::vector<int>& max_constraint) {
    if (k == 0 && n == 0) {
        arrangements.push_back(build_string(current_tuple, values));
        return;
    }

    if (k == 0) {
        return;
    }

    size_t bucket_idx = current_tuple.size();


    for (size_t i = min_constraint[bucket_idx];
            i <= std::min(n, max_constraint[bucket_idx]); ++i) {

        auto temp = current_tuple;
        temp.push_back(i);

        find_arrangements(arrangements, temp, values,
                          k - 1, n - i,
                          min_constraint, max_constraint);
    }

}

int sum(std::vector<int> nums, int offset) {
    int total = 0;

    for (int i = offset; i < nums.size(); ++i) {
        total += nums[i];
    }

    return total;
}

int test_arrangements(std::vector<std::string>& arrangements,
                      const std::string& input) {
    int count = 0;

    for (auto& arr : arrangements) {
        count += static_cast<int>(check_string(input, arr));

    }

    return count;
}

std::string build_string(const std::vector<int> bucket,
                         const std::vector<int>& values) {
    std::string arrangement(bucket[0], '.');

    for (size_t i = 0; i < values.size(); ++i) {
        arrangement += std::string(values[i], '#');
        arrangement += std::string(bucket[i + 1], '.');
    }

    return arrangement;
}

bool check_string(const std::string& input,
                  const std::string& test_arrangement) {
    int i = 0;

    while (i < input.size() &&
            !(input[i] == '.' && test_arrangement[i] == '#') &&
            !(input[i] == '#' && test_arrangement[i] == '.')) {
        ++i;
    }

    return (i == input.size());
}

template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr) {
    for (auto& col : arr) {
        std::cout << col << ", ";
    }

    std::cout << std::endl;

    return os;
}