#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>

using Position = std::pair<int, int>;

struct Grid {
    std::vector<std::vector<char>> grid;

    Grid() = default;

    std::pair<size_t, size_t> shape() {
        return {grid.size(), grid[0].size()};
    }

    void push_back(std::vector<char> row) {
        grid.push_back(row);
    }

    const char operator[](const Position pos) const {
        return grid[pos.first][pos.second];
    }
};


Position parse_grid(std::string path, Grid& grid);
bool valid_pos(Grid& grid, const Position& pos);
bool valid_move(Grid& grid, const Position& pos, const Position& move);
size_t part_1(Grid& grid, Position start);
size_t part_2(Grid& grid, Position start);
size_t shoelace(std::vector<Position> vertices);
Position operator+(const Position pos_1, const Position pos_2);
bool operator==(const Position pos_1, const Position pos_2);
bool operator!=(const Position pos_1, const Position pos_2);
size_t dfs(Grid& grid, Position pos, Position prev,
           std::vector<Position>& path);

int main() {
    std::string path = "/home/bforrest/aoc23/day10/input.txt";
    Grid grid;
    auto start = parse_grid(path, grid);

    size_t score_1 = part_1(grid, start);
    size_t score_2 = part_2(grid, start);

    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;

    return 0;
}

Position parse_grid(std::string path, Grid& grid) {
    std::ifstream infile(path);
    size_t i = 0;
    Position start;

    while (infile) {
        char temp;
        std::vector<char> row;
        size_t j = 0;

        while (infile.get(temp) && temp != '\n') {
            row.push_back(temp);

            if (temp == 'S') {
                start = Position {i, j};
            }

            ++j;
        }

        grid.push_back(row);
        ++i;
    }

    infile.close();

    return start;
}

bool valid_pos(Grid& grid, const Position& pos) {
    return ((pos.first >= 0)
            && (pos.second >= 0)
            && (pos.first < grid.shape().first)
            && (pos.second < grid.shape().second)
            && (grid[pos] != '.'));
}

bool valid_move(Grid& grid, const Position& pos, const Position& move) {
    static const std::unordered_map<char, std::vector<Position>> map {
        {'|', std::vector<Position> {{1, 0}, {-1, 0}}},
        {'-', std::vector<Position> {{0, 1}, {0, -1}}},
        {'L', std::vector<Position> {{-1, 0}, {0, 1}}},
        {'J', std::vector<Position> {{-1, 0}, {0, -1}}},
        {'7', std::vector<Position> {{1, 0}, {0, -1}}},
        {'F', std::vector<Position> {{1, 0}, {0, 1}}}
    };

    auto next_tile = grid[pos + move];
    auto valid_moves = map.at(next_tile);
    return (std::find(
                valid_moves.begin(),
                valid_moves.end(),
                Position {-move.first, -move.second}) != valid_moves.end());
}

size_t part_1(Grid& grid, Position start) {
    const std::vector<Position> start_directions {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};
    std::vector<Position> path {start};
    size_t dist = -1;

    for (const auto& dir : start_directions) {
        if (
            (dist == -1)
            && (valid_pos(grid, start + dir))
            && (valid_move(grid, start, dir))
        ) {
            dist = dfs(grid, start + dir, start, path);
        }
    }

    return (dist + 1) / 2;
}

size_t part_2(Grid& grid, Position start) {
    const std::vector<Position> start_directions {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};
    std::vector<Position> path {start};
    size_t dist = -1;

    for (const auto& dir : start_directions) {
        if (
            (dist == -1)
            && (valid_pos(grid, start + dir))
            && (valid_move(grid, start, dir))
        ) {
            dist = dfs(grid, start + dir, start, path);
        }
    }

    auto area = shoelace(path);
    size_t path_length = (dist + 1) / 2;

    return area - path_length + 1;
}

size_t shoelace(std::vector<Position> vertices) {
    vertices.push_back(vertices.front());
    int result = 0;

    for (size_t i = 1; i < vertices.size(); ++i) {
        result += (vertices[i - 1].first * vertices[i].second) -
                  (vertices[i].first * vertices[i - 1].second);
    }

    size_t area = result < 0 ? -((result - 1) / 2) : ((result + 1) / 2);

    return area;
}

Position operator+(const Position pos_1, const Position pos_2) {
    return Position {pos_1.first + pos_2.first, pos_1.second + pos_2.second};
}

bool operator==(const Position pos_1, const Position pos_2) {
    return ((pos_1.first == pos_2.first) && (pos_1.second == pos_2.second));
}

bool operator!=(const Position pos_1, const Position pos_2) {
    return !(pos_1 == pos_2);
}

size_t dfs(Grid& grid, Position pos, Position prev,
           std::vector<Position>& path) {
    static const std::unordered_map<char, std::pair<Position, Position>> map {
        {'|', std::pair<Position, Position> {{1, 0}, {-1, 0}}},
        {'-', std::pair<Position, Position> {{0, 1}, {0, -1}}},
        {'L', std::pair<Position, Position> {{-1, 0}, {0, 1}}},
        {'J', std::pair<Position, Position> {{-1, 0}, {0, -1}}},
        {'7', std::pair<Position, Position> {{1, 0}, {0, -1}}},
        {'F', std::pair<Position, Position> {{1, 0}, {0, 1}}}
    };

    size_t dist = 0;
    bool no_move = false;

    while (grid[pos] != 'S' && !(no_move)) {
        auto [move_1, move_2] = map.at(grid[pos]);

        auto next_pos = (pos + move_1 == prev) ? pos + move_2 : pos + move_1;
        prev = pos;

        if (valid_pos(grid, next_pos)) {
            path.push_back(pos);
            pos = next_pos;
            dist++;
        } else {
            no_move = true;
        }

    }

    return no_move ? static_cast<size_t>(-1) : dist;
}