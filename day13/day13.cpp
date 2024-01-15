#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using Pattern = std::vector<std::string>;
using Patterns = std::vector<Pattern>;

Patterns parse(std::string path);
size_t part_1(Patterns patterns);
size_t part_2(Patterns patterns);
size_t reflection(Pattern pattern, size_t height, size_t width,
                  char (*get)(Pattern, int, int));
size_t smudge_reflection(Pattern pattern, size_t height, size_t width,
                         char (*get)(Pattern, int, int));

int main() {
    std::string path = "/home/bforrest/aoc23/day13/input.txt";
    auto patterns = parse(path);
    auto score_1 = part_1(patterns);
    auto score_2 = part_2(patterns);

    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;

    return 0;
}

Patterns parse(std::string path) {
    std::ifstream infile(path);
    Patterns patterns;

    while (infile) {
        Pattern pattern;
        std::string line = "";

        while (std::getline(infile, line) && line != "") {
            pattern.push_back(line);
        }

        patterns.push_back(pattern);
    }

    return patterns;
}

size_t part_1(Patterns patterns) {
    size_t score = 0;
    auto col_wise = [](Pattern p, int i, int j) {return p[i][j];};
    auto row_wise = [](Pattern p, int i, int j) {return p[j][i];};

    for (auto& pattern : patterns) {
        size_t row = 0;
        size_t col = 0;

        row = reflection(pattern, pattern.front().size(),
                         pattern.size(), row_wise);

        if (row == 0) {
            col = reflection(pattern, pattern.size(),
                             pattern.front().size(), col_wise);
        }

        score += col + 100 * row;
    }

    return score;
}

size_t part_2(Patterns patterns) {
    size_t score = 0;
    auto col_wise = [](Pattern p, int i, int j) {return p[i][j];};
    auto row_wise = [](Pattern p, int i, int j) {return p[j][i];};

    for (auto& pattern : patterns) {
        size_t row = 0;
        size_t col = 0;

        row = smudge_reflection(pattern, pattern.front().size(),
                                pattern.size(), row_wise);

        if (row == 0) {
            col = smudge_reflection(pattern, pattern.size(),
                                    pattern.front().size(), col_wise);
        }

        score += col + 100 * row;
    }

    return score;
}

size_t reflection(Pattern pattern, size_t height, size_t width,
                  char (*get)(Pattern, int, int)) {
    size_t pos = 0;
    bool valid_pos = false;

    for (int i = 1; (!valid_pos) && (i < width); ++i) {
        valid_pos = true;
        for (int j = 0; valid_pos && j < height; ++j) {
            valid_pos = (get(pattern, j, i) == get(pattern, j, i - 1));

            if (valid_pos) {
                auto l = i - 2;
                auto r = i + 1;

                while ((l >= 0) && (r < width)
                        && (get(pattern, j, l) == get(pattern, j, r))) {
                    --l;
                    ++r;
                }

                valid_pos = (l == -1 || r == width);
            }
        }

        if (valid_pos) pos = i;
    }

    return pos;
}

size_t smudge_reflection(Pattern pattern, size_t height, size_t width,
                         char (*get)(Pattern, int, int)) {
    size_t pos = 0;
    bool valid_pos = false;

    for (int i = 1; (!valid_pos) && (i < width); ++i) {
        valid_pos = true;
        int errors = 0;

        for (int j = 0; valid_pos && j < height; ++j) {
            errors += static_cast<int>(get(pattern, j, i) != get(pattern, j, i - 1));
            valid_pos = errors <= 1;

            if (valid_pos) {
                auto l = i - 2;
                auto r = i + 1;
                while ((l >= 0) && (r < width) && (errors <= 1)) {
                    errors += static_cast<int>(get(pattern, j, l) != get(pattern, j, r));
                    --l;
                    ++r;
                }

                valid_pos = (l == -1 || r == width) && (errors <= 1);
            }
        }

        valid_pos = (valid_pos) && (errors == 1);

        if (valid_pos) pos = i;
    }

    return pos;
}