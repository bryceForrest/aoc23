#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

int parse_line_p1(std::string line);
int parse_line_p2(std::string line);
std::vector<std::string> split(std::string str, std::string delim);
int power(std::unordered_map<std::string, int> cube_count);
bool valid_round(std::unordered_map<std::string, int> cube_count);
void print(std::vector<std::string> vec);

const int RED = 12;
const int GREEN = 13;
const int BLUE = 14;

int main() {
    std::ifstream infile("input_1.txt");
    std::string line = "";

    int score_p1 = 0;
    int score_p2 = 0;

    while (std::getline(infile, line)) {
        score_p1 += parse_line_p1(line);
        score_p2 += parse_line_p2(line);
    }

    std::cout << "The solution for part 1: " << score_p1 << std::endl;
    std::cout << "The solution for part 2: " << score_p2 << std::endl;

    return 0;
}

int parse_line_p1(std::string line) {
    std::vector<std::string> game_data = split(line, ": ");
    int game_num = std::stoi(split(game_data[0], " ")[1]);

    std::vector<std::string> round_data = split(game_data[1], "; ");
    int round_count = round_data.size();
    int valid_rounds = 0;

    for (auto round : round_data) {
        std::unordered_map<std::string, int> cube_count = {
            {"blue", 0},
            {"red", 0},
            {"green", 0}
        };

        std::vector<std::string> cube_data = split(round, ", ");

        for (auto cube : cube_data) {
            std::vector<std::string> colors = split(cube, " ");
            cube_count[colors[1]] = std::stoi(colors[0]);
        }

        valid_rounds += static_cast<int>(valid_round(cube_count));
    }

    return static_cast<int>(valid_rounds == round_count) * game_num;
}

int parse_line_p2(std::string line) {
    std::vector<std::string> game_data = split(line, ": ");
    int game_num = std::stoi(split(game_data[0], " ")[1]);

    std::vector<std::string> round_data = split(game_data[1], "; ");

    std::unordered_map<std::string, int> cube_count = {
        {"blue", 0},
        {"red", 0},
        {"green", 0}
    };

    for (auto round : round_data) {
        std::vector<std::string> cube_data = split(round, ", ");

        for (auto cube : cube_data) {
            std::vector<std::string> colors = split(cube, " ");
            cube_count[colors[1]] = std::max(
                                        cube_count[colors[1]],
                                        std::stoi(colors[0])
                                    );
        }
    }

    return power(cube_count);
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


bool valid_round(std::unordered_map<std::string, int> cube_count) {
    return (cube_count["blue"] <= BLUE &&
            cube_count["red"] <= RED &&
            cube_count["green"] <= GREEN);
}

int power(std::unordered_map<std::string, int> cube_count) {
    return (cube_count["blue"] *
            cube_count["red"] *
            cube_count["green"]);
}

void print(std::vector<std::string> vec) {
    for (auto i : vec) {
        std::cout << i << "~";
    }
    std::cout << std::endl;
}