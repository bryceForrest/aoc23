#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


using History = std::vector<int>;

int part_1(std::string path);
int part_2(std::string path);
std::istream& operator>>(std::istream& is, History& history);
void diff_step(std::vector<History>& all_history, History history);
void extrapolate(std::vector<History>& all_history);
void reverse_extrapolate(std::vector<History>& all_history);

int main() {
    std::string path = "/home/bforrest/aoc23/day9/input.txt";
    int score_1 = part_1(path);
    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    int score_2 = part_2(path);
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;
    return 0;
}

int part_1(std::string path) {
    std::ifstream infile(path);
    History history;
    int score = 0;

    while (infile >> history) {
        std::vector<History> all_history;
        diff_step(all_history, history);
        extrapolate(all_history);
        score += all_history.front().back();
    }

    infile.close();

    return score;
}

int part_2(std::string path) {
    std::ifstream infile(path);
    History history;
    int score = 0;

    while (infile >> history) {
        std::vector<History> all_history;
        diff_step(all_history, history);
        reverse_extrapolate(all_history);
        score += all_history.front().front();
    }

    infile.close();

    return score;
}

std::istream& operator>>(std::istream& is, History& history) {
    int temp;
    history.clear();

    if (is.peek() == EOF) {
        is.setstate(std::ios::failbit);
    } else {
        while (is.good() && is.peek() != '\n') {
            is >> temp;
            history.push_back(temp);
        }

        is >> std::ws;
    }

    return is;
}

void diff_step(std::vector<History>& all_history, History history) {
    all_history.push_back(history);
    if (std::all_of(history.cbegin(), history.cend(), [](int i) {return i == 0;})) {
        return;
    }

    std::adjacent_difference(history.begin(), history.end(), history.begin());
    diff_step(all_history, History {history.begin() + 1, history.end()});
}

void extrapolate(std::vector<History>& all_history) {
    all_history.back().push_back(0);
    for (auto it = all_history.rbegin() + 1; it != all_history.rend(); it++) {
        it->push_back((it - 1)->back() + it->back());
    }
}

void reverse_extrapolate(std::vector<History>& all_history) {
    all_history.back().insert(all_history.back().begin(), 0);
    for (auto it = all_history.rbegin() + 1; it != all_history.rend(); it++) {
        it->insert(it->begin(), it->front() - (it - 1)->front());
    }
}