#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>

std::vector<std::string> parse(std::string path);
u_short hash(std::string str);
size_t part_1(std::vector<std::string> seq);
size_t part_2(std::vector<std::string> seq);

int main() {
    std::string path = "/home/bforrest/aoc23/day15/input.txt";
    auto seq = parse(path);
    auto score_1 = part_1(seq);
    auto score_2 = part_2(seq);

    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;

    return 0;
}


std::vector<std::string> parse(std::string path) {
    std::ifstream infile(path);
    std::string temp;
    std::vector<std::string> seq;

    std::getline(infile, temp);
    std::stringstream ss(temp);

    while(ss && std::getline(ss, temp, ',')) {
        seq.push_back(temp);
    }

    return seq;
}

u_short hash(std::string str) {
    u_short current_val = 0;

    for (auto& ch : str) {
        current_val += static_cast<int>(ch);
        current_val *= 17;
        current_val %= 256;
    }

    return current_val;
}

size_t part_1(std::vector<std::string> seq) {
    size_t score = 0;

    for (auto& s : seq) {
        score += hash(s);
    }

    return score;
}

size_t part_2(std::vector<std::string> seq) {
    using slot_pair = std::pair<std::string, u_short>;
    using box_data = std::vector<slot_pair>;

    size_t score = 0;
    std::vector<box_data> boxes(256);
    auto is_op = [](char ch) {return ((ch == '=') || (ch == '-'));};

    for (auto& s : seq) {
        auto pos = std::find_if(s.begin(), s.end(), is_op);
        auto key = std::string {s.begin(), pos};
        auto box_num = hash(key);
        auto& box = boxes[box_num];
        auto key_pos = std::find_if(
        box.begin(), box.end(), [&key](const slot_pair& sp) {return (sp.first == key);});
        u_short val;

        switch(*pos) {
        case '=':
            val = std::stoul(std::string {pos + 1, s.end()});

            if (key_pos == box.end()) {
                box.push_back(slot_pair {key, val});
            } else {
                (*key_pos).second = val;
            }

            break;
        case '-':
            if (key_pos != box.end()) {
                boxes[box_num].erase(key_pos);
            }

            break;
        default:
            break;
        }
    }

    for (size_t i = 0; i < boxes.size(); ++i) {
        for (size_t j = 0; j < boxes[i].size(); ++j) {
            score += (i + 1) * (j + 1) * boxes[i][j].second;
        }
    }

    return score;
}