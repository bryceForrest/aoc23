#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using Grid = std::vector<std::vector<char>>;

Grid parse(std::string path);
size_t part_1(Grid grid);
size_t part_2(Grid grid, size_t cycles);
Grid rotate(Grid& grid);
bool roll(Grid& grid);
size_t hash(const Grid& grid);

int main() {
    std::string path = "/home/bforrest/aoc23/day14/input.txt";
    auto grid = parse(path);

    size_t score_1 = part_1(grid);
    size_t score_2 = part_2(grid, 1000000000);

    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;

    return 0;
}

Grid parse(std::string path) {
    std::ifstream infile(path);
    Grid grid;

    while(infile && infile.peek() != EOF) {
        char temp;
        std::vector<char> row;

        while(infile.get(temp) && temp != '\n') {
            row.push_back(temp);
        }

        grid.push_back(row);
    }

    infile.close();

    return grid;
}

size_t part_1(Grid grid) {
    size_t score = 0;
    while (roll(grid)) {}

    for (size_t i = 0; i < grid.size(); ++i) {
        size_t load = grid.size() - i;
        size_t rocks = std::count(grid[i].begin(), grid[i].end(), 'O');

        score += rocks * load;
    }

    return score;
}

size_t part_2(Grid grid, size_t cycles) {
    static std::unordered_map<size_t, std::pair<bool, size_t>> visited;
    size_t score = 0;
    bool cycle_found = false;

    for (size_t i = 0; i < cycles; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            while (roll(grid)) {}
            grid = rotate(grid);
        }

        auto key = hash(grid);
        auto try_insert = visited.emplace(key, std::pair<bool, size_t> {true, i});

        if (!cycle_found && (try_insert.second == false))  {
            size_t repeat_len = i - visited[key].second;
            size_t remaining = cycles - i;
            i = cycles - (remaining % repeat_len);
            cycle_found = true;
        }
    }


    for (size_t i = 0; i < grid.size(); ++i) {
        size_t load = grid.size() - i;
        size_t rocks = std::count(grid[i].begin(), grid[i].end(), 'O');

        score += rocks * load;
    }

    return score;
}

bool roll(Grid& grid) {
    bool change = false;

    for (size_t i = 0; i < grid.size() - 1; ++i) {
        for (size_t j = 0; j < grid.front().size(); ++j) {
            if (grid[i + 1][j] == 'O' && grid[i][j] == '.') {
                change = true;
                std::swap(grid[i][j], grid[i + 1][j]);
            }
        }
    }

    return change;
}

Grid rotate(Grid& grid) {
    std::vector<char> col (grid.size(), '\0');
    Grid transposed (grid.front().size(), col);

    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid.front().size(); ++j) {
            transposed[j][transposed.front().size() - 1 - i] = grid[i][j];
        }
    }

    return transposed;
}

size_t hash(const Grid& grid) {
    size_t h = 0;
    size_t p_i = 1399;
    size_t p_j = 3853;

    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid.front().size(); ++j) {
            if (grid[i][j] == 'O') {
                h ^= (p_i * i) ^ (p_j * j);
            }
        }
    }

    return h;
}