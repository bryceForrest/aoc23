#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

int part_1(std::string path);
int part_2(std::string path);
int points(std::string game);
int play(std::vector<std::set<int>> winning_numbers,
         std::vector<std::set<int>> card_numbers);
int sum(std::vector<int> vec);
std::vector<std::string> split(std::string str, std::string delim);
std::set<int> make_set(std::string str);



template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr);
template<class T>
std::ostream& operator << (std::ostream& os, const std::set<T>& arr);



int main() {
    std::string path = "/home/bforrest/aoc23/day4/input_p1.txt";
    int score_p1 = part_1(path);
    int score_p2 = part_2(path);
    std::cout << "The solution to part 1 is: " << score_p1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_p2 << std::endl;

    return 0;
}

int part_1(std::string path) {
    std::ifstream infile (path);
    std::string line = "";

    int score = 0;

    while (getline(infile, line)) {
        score += points(line);
    }

    infile.close();

    return score;
}

int part_2(std::string path) {
    std::ifstream infile (path);
    std::string line = "";
    std::vector<std::set<int>> winning_numbers;
    std::vector<std::set<int>> card_numbers;
    int score = 0;

    while (getline(infile, line)) {
        // remove card info
        size_t pos = line.find(':');
        auto game_data = line.substr(pos + 1);

        auto card_data = split(game_data, "|");
        winning_numbers.push_back(make_set(card_data[0]));
        card_numbers.push_back(make_set(card_data[1]));
    }

    infile.close();

    score = play(winning_numbers, card_numbers);
    return score;
}

int points(std::string game) {
    // remove card info
    size_t pos = game.find(':');
    auto game_data = game.substr(pos + 1);

    auto card_data = split(game_data, "|");
    auto winning_numbers = make_set(card_data[0]);
    auto card_numbers = make_set(card_data[1]);

    std::vector<int> intersection;
    std::set_intersection(winning_numbers.begin(), winning_numbers.end(),
                          card_numbers.begin(), card_numbers.end(),
                          std::back_inserter(intersection));

    int matches = intersection.size();
    int score = static_cast<int>(matches > 0) * std::exp2(matches - 1);

    return score;
}

int play(std::vector<std::set<int>> winning_numbers,
         std::vector<std::set<int>> card_numbers) {
    std::vector<int> card_count(winning_numbers.size(), 1);

    for (size_t i = 0; i < winning_numbers.size(); ++i) {
        std::vector<int> intersection;
        std::set_intersection(winning_numbers[i].begin(), winning_numbers[i].end(),
                              card_numbers[i].begin(), card_numbers[i].end(),
                              std::back_inserter(intersection));
        int matches = intersection.size();

        for (size_t j = 0; j < matches; ++j) {
            card_count[i + j + 1] += card_count[i];
        }
    }

    return sum(card_count);
}

int sum(std::vector<int> vec) {
    int total = 0;

    for (auto& v : vec) {
        total += v;
    }

    return total;
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

std::set<int> make_set(std::string str) {
    std::stringstream ss (str);
    std::set<int> str_set;
    int temp = 0;

    while (ss >> temp) {
        str_set.insert(temp);
    }

    return str_set;
}


///////////////////////////////////////////////////////////////////////////////
// Helper prints
///////////////////////////////////////////////////////////////////////////////

template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr) {
    for (auto& col : arr) {
        std::cout << col << "\n";
    }

    return os;
}

template<class T>
std::ostream& operator << (std::ostream& os, const std::set<T>& arr) {
    for (auto& col : arr) {
        std::cout << col << " ";
    }

    return os;
}