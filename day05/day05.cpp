#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <chrono>

struct Interval {
    uint64_t lower_bound = 0;
    uint64_t upper_bound = 0;

    Interval(uint64_t i, uint64_t j) {
        lower_bound = i;
        upper_bound = j;
    }

    bool contains(uint64_t number) {
        return (lower_bound <= number && number < upper_bound);
    }

    std::vector<Interval> sub_intervals(std::vector<Interval> intervals) {
        std::vector<Interval> result;
        std::vector<uint64_t> values {lower_bound, upper_bound};

        for (auto& interv : intervals) {
            values.push_back(interv.lower_bound);
            values.push_back(interv.upper_bound);
        }

        std::sort(values.begin(), values.end());

        auto start = values.begin();

        while (start != values.end() && lower_bound > *start) start++;

        auto stop = start;

        while (stop != values.end() && upper_bound > *stop) {stop++;}

        values = {start, stop};

        if (!values.empty()) {
            for (size_t i = 1; i < values.size(); ++i) {
                result.push_back(Interval(values[i - 1], values[i]));
            }
        }


        return result;
    }

    friend bool operator< (const Interval& i1, const Interval& i2);
    friend std::ostream& operator << (std::ostream& os, Interval i);

};

bool operator< (const Interval& i1, const Interval& i2) {
    return (i1.upper_bound <= i2.lower_bound);
}

std::ostream& operator << (std::ostream& os, Interval i) {
    std::cout << "[" << i.lower_bound << ", " << i.upper_bound << ")";
    return os;
}

struct Almanac {
    std::string source_str = "";
    std::string destination_str = "";
    Almanac* destination = nullptr;
    std::vector<Interval> source_vals;
    std::vector<Interval> destination_vals;

    Almanac() = default;

    Almanac(std::string input_source, std::string input_destination) {
        source_str = input_source;
        destination_str = input_destination;
    }

    void build_map(uint64_t dest_range_start, uint64_t source_range_start,
                   uint64_t range_length) {
        source_vals.push_back(Interval(source_range_start,
                                       source_range_start + range_length));
        destination_vals.push_back(Interval(dest_range_start,
                                            dest_range_start + range_length));
    }

    const size_t find(uint64_t num) {
        size_t i = 0;

        while ((i < source_vals.size()) && !(source_vals[i].contains(num))) {
            ++i;
        }

        return i;
    }

    const uint64_t map(uint64_t num) {
        size_t pos = find(num);
        uint64_t mapped_num = num;

        if (pos != source_vals.size()) {
            mapped_num = (num - source_vals[pos].lower_bound) +
                         destination_vals[pos].lower_bound;
        }

        return mapped_num;
    }

    const std::set<Interval> map(Interval interv) {
        std::vector<Interval> split_intervals;
        std::set<Interval> mapped_intervals;

        split_intervals = interv.sub_intervals(source_vals);

        if (split_intervals.empty()) split_intervals = {interv};

        for (auto& interv : split_intervals) {
            size_t pos = find(interv.lower_bound);
            uint64_t mapped_lower = interv.lower_bound;
            uint64_t mapped_upper = interv.upper_bound;

            if (pos != source_vals.size()) {
                mapped_lower = (mapped_lower - source_vals[pos].lower_bound) +
                               destination_vals[pos].lower_bound;
                mapped_upper = (mapped_upper - source_vals[pos].lower_bound) +
                               destination_vals[pos].lower_bound;
            }

            mapped_intervals.insert(Interval(mapped_lower, mapped_upper));
        }

        return mapped_intervals;
    }
};



std::vector<std::string> split(std::string str, std::string delim);
std::vector<uint64_t> get_seeds_p1(std::ifstream& infile);
std::vector<Interval> get_seeds_p2(std::ifstream& infile);
std::unordered_map<std::string, Almanac*> parse(std::ifstream& infile);
uint64_t traverse_p1(Almanac* alm, const std::vector<uint64_t>& values);
uint64_t traverse_p2(Almanac* alm, const std::vector<Interval>& values);

template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr);




int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string path = "/home/bforrest/aoc23/day5/input_p1.txt";
    std::ifstream infile(path);

    // auto seeds = get_seeds_p1(infile);
    auto seeds = get_seeds_p2(infile);
    auto map = parse(infile);

    infile.close();

    auto seed_to_soil = map["seed-to-soil"];

    // auto result = traverse_p1(seed_to_soil, seeds);
    auto result = traverse_p2(seed_to_soil, seeds);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (stop - start).count() / 1e9;


    std::cout << "The solution for part 1 is: " << result << std::endl;
    // std::cout << "The solution for part 2 is: " << result << std::endl;
    std::cout << "Runtime: " << duration << std::endl;

    return 0;
}

std::vector<std::string> split(std::string str, std::string delim) {
    size_t pos = 0;
    size_t prev = 0;
    std::vector<std::string> result;

    do {
        pos = str.find(delim, prev);
        std::string token = str.substr(prev, pos - prev);
        prev = pos + delim.length();
        result.push_back(token);
    } while (pos != std::string::npos);

    return result;
}

std::vector<uint64_t> get_seeds_p1(std::ifstream& infile) {
    std::vector<uint64_t> seeds;
    std::string temp_str;
    char temp_ch;
    uint64_t temp_int;

    infile >> temp_str >> std::ws;
    temp_ch = infile.peek();

    while (isdigit(temp_ch)) {
        infile >> temp_int >> std::ws;
        seeds.push_back(temp_int);
        temp_ch = infile.peek();
    }

    return seeds;
}

std::vector<Interval> get_seeds_p2(std::ifstream& infile) {
    std::vector<Interval> seeds;
    std::string temp_str;
    char temp_ch;
    uint64_t temp_int;
    uint64_t temp_range;

    infile >> temp_str >> std::ws;
    temp_ch = infile.peek();

    while (isdigit(temp_ch)) {
        infile >> temp_int >> temp_range >> std::ws;
        seeds.push_back(Interval(temp_int, temp_int + temp_range));
        temp_ch = infile.peek();
    }

    return seeds;
}

std::unordered_map<std::string, Almanac*> parse(std::ifstream& infile) {
    std::unordered_map<std::string, Almanac*> map;
    std::string temp_str;
    char temp_ch;
    Almanac* prev_alm = nullptr;

    while(infile >> temp_str) {
        auto to_from = split(temp_str, "-to-");
        infile.ignore(100, '\n');
        infile >> std::ws;
        temp_ch = infile.peek();

        Almanac* temp_alm = new Almanac(to_from[0], to_from[1]);

        if (prev_alm != nullptr) {
            prev_alm->destination = temp_alm;
        }

        uint64_t temp_source;
        uint64_t temp_dest;
        uint64_t temp_range;

        while (isdigit(temp_ch)) {
            infile >> temp_dest >> temp_source >> temp_range >> std::ws;
            (*temp_alm).build_map(temp_dest, temp_source, temp_range);
            temp_ch = infile.peek();
        }

        map[temp_str] = temp_alm;
        prev_alm = temp_alm;
    }

    return map;
}

uint64_t traverse_p1(Almanac* alm, const std::vector<uint64_t>& values) {
    if (alm == nullptr) {
        return *std::min_element(values.begin(), values.end());
    }

    std::vector<uint64_t> mapped_values;

    for (auto& val : values) {
        auto mapped_val = (*alm).map(val);
        mapped_values.push_back(mapped_val);
    }

    auto next_alm = (*alm).destination;

    return traverse_p1(next_alm, mapped_values);
}


uint64_t traverse_p2(Almanac* alm, const std::vector<Interval>& intervs) {
    if (alm == nullptr) {
        return (*std::min_element(intervs.begin(), intervs.end())).lower_bound;
    }

    std::vector<Interval> mapped_intervs;

    for (auto& interv : intervs) {
        auto mapped_interv = (*alm).map(interv);
        mapped_intervs.insert(mapped_intervs.end(),
                              mapped_interv.begin(),
                              mapped_interv.end());
    }

    auto next_alm = (*alm).destination;

    return traverse_p2(next_alm, mapped_intervs);
}


template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr) {
    for (auto& col : arr) {
        std::cout << col << ", ";
    }

    std::cout << std::endl;

    return os;
}