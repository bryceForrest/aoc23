#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <complex>
#include <utility>
#include <functional>

using Grid = std::vector<std::vector<char>>;
using Coord = std::complex<int>;
using History = std::vector<std::vector<std::pair<bool, Coord>>>;

Grid parse(std::string path);
size_t part_1(Grid grid);
size_t part_2(Grid grid);
size_t traverse(const Grid& grid, History& visited, Coord pos, Coord prev);

int main() {
    std::string path = "/home/bforrest/aoc23/day16/input.txt";
    auto grid = parse(path);
    auto score_1 = part_1(grid);
    auto score_2 = part_2(grid);

    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;
    return 0;
}

Grid parse(std::string path) {
    std::ifstream infile(path);
    Grid grid;

    while ((infile) && (infile.peek() != EOF)) {
        char ch;
        std::vector<char> row;

        while ((infile.get(ch)) && (ch != '\n')) {
            row.push_back(ch);
        }

        grid.push_back(row);
    }

    return grid;
}

size_t part_1(Grid grid) {
    History visited(grid.size(),
                    std::vector<std::pair<bool, Coord>>(grid.front().size(), {false, Coord{-1,-1}}));

    return traverse(grid, visited, Coord(0, 0), Coord(-1, 0));
}


size_t part_2(Grid grid) {
    History visited(grid.size(),
                    std::vector<std::pair<bool, Coord>>(grid.front().size(), {false, Coord{-1,-1}}));
    std::vector<size_t> results;

    for (int i = 0; i < grid.front().size(); ++i) {
        {
            History visited_copy(visited);

            results.push_back(
                traverse(grid, visited_copy, Coord(i, 0), Coord(i, -1))
            );
        }

        {
            History visited_copy(visited);

            results.push_back(
                traverse(grid, visited_copy, Coord(i, grid.size() - 1), Coord(i,grid.size()))
            );
        }
    }

    for (int i = 0; i < grid.size(); ++i) {
        {
            History visited_copy(visited);

            results.push_back(
                traverse(grid, visited_copy, Coord(0, i), Coord(-1, i))
            );
        }

        {
            History visited_copy(visited);

            results.push_back(
                traverse(grid, visited_copy, Coord(grid.front().size() - 1, i),
                         Coord(grid.size(), i))
            );
        }
    }

    return *std::max_element(results.begin(), results.end());
}

size_t traverse(const Grid& grid, History& visited, Coord pos, Coord prev) {
    Coord dir = pos - prev;
    size_t score = 1;

    if ((pos.real() < 0)
            || (pos.real() >= grid.front().size())
            || (pos.imag() < 0)
            || (pos.imag() >= grid.size())) {
        return 0;
    }

    if (visited[pos.imag()][pos.real()].first) {
        if (visited[pos.imag()][pos.real()].second == dir) {
            return 0;
        }

        score = 0;
    }

    visited[pos.imag()][pos.real()] = {true, dir};

    switch (grid[pos.imag()][pos.real()]) {
    case '.':
        score += traverse(grid, visited, pos + dir, pos);
        break;
    case '-':
        if (dir.imag())  {
            score += traverse(grid, visited, pos + Coord(-1, 0), pos)
                     + traverse(grid, visited, pos + Coord(1, 0), pos);
        } else {
            score += traverse(grid, visited, pos + dir, pos);
        }

        break;
    case '|':
        if (dir.real()) {
            score += traverse(grid, visited, pos + Coord(0, -1), pos)
                     + traverse(grid, visited, pos + Coord(0, 1), pos);
        } else {
            score += traverse(grid, visited, pos + dir, pos);
        }
        break;
    case '\\':
        score += traverse(grid, visited, pos + Coord(dir.imag(), dir.real()), pos);
        break;
    case '/':
        score += traverse(grid, visited, pos + Coord(-dir.imag(), -dir.real()), pos);
        break;
    default:
        break;
    }

    return score;
}