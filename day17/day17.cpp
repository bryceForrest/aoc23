#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <complex>
#include <functional>

using Grid = std::vector<std::vector<size_t>>;
using Coord = std::complex<int>;


struct Dist {
    size_t dist = std::numeric_limits<size_t>::max();

    void operator=(const size_t& d) {
        dist = d;
    }

    bool operator>(const size_t& d) {
        return dist > d;
    }

    bool operator<(const size_t& d) {
        return dist < d;
    }
};


struct Pos {
    Coord pos;
    Coord move;
    size_t heat_loss = -1;
    size_t consec = 0;

    friend bool operator> (const Pos& lhs, const Pos& rhs) {
        return rhs.heat_loss < lhs.heat_loss;
    }
};


using History = std::unordered_map<size_t, Dist>;

Grid parse(std::string path);
size_t part_1(const Grid& grid);
size_t part_2(const Grid& grid);
bool check_bounds(const Grid& grid, const Coord& pos);
size_t traverse(const Grid& grid, size_t min_consec, size_t max_consec);
size_t hash(const std::complex<int>& pos,
            const std::complex<int>& move,
            const size_t& consec);

int main() {
    std::string path = "/home/bforrest/aoc23/day17/input.txt";
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
        std::vector<size_t> row;

        while ((infile.get(ch)) && (ch != '\n')) {
            row.push_back(ch - '0');
        }

        grid.push_back(row);
    }

    return grid;
}


size_t part_1(const Grid& grid) {
    size_t score = 0;
    score = traverse(grid, 0, 3);

    return score;
}


size_t part_2(const Grid& grid) {
    size_t score = 0;
    score = traverse(grid, 4, 10);

    return score;
}


bool check_bounds(const Grid& grid, const Coord& pos) {
    return (
               (pos.real() >= 0) &&
               (pos.imag() >= 0) &&
               (pos.real() < grid.front().size()) &&
               (pos.imag() < grid.size())
           );
}


size_t traverse(const Grid& grid, size_t min_consec, size_t max_consec) {
    static const std::vector<Coord> dirs {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    std::priority_queue<Pos, std::vector<Pos>, std::greater<Pos>> queue;
    auto heat = [&grid](Coord c) {return grid[c.imag()][c.real()];};

    History visited;
    size_t best = std::numeric_limits<size_t>::max();

    Coord start = {0, 0};
    Coord end {static_cast<int>(grid.front().size() - 1),
               static_cast<int>(grid.size() - 1)};

    auto key = hash(start, start, min_consec + 1);
    visited[key] = 0;
    queue.push(Pos {start, start, 0, min_consec + 1});

    while (!queue.empty() && best == std::numeric_limits<size_t>::max()) {
        auto [cur, move, hl, consec] = queue.top();
        queue.pop();

        best = (cur == end) && (consec >= min_consec) ? hl : best;
        auto next_dirs = (consec >= min_consec) ? dirs : std::vector<Coord> {move};

        for (const auto d : next_dirs) {
            auto c = (d == move) ? consec + 1 : 1;
            auto next = cur + d;
            key = hash(next, d, c);

            if ((check_bounds(grid, next)) &&
                    (d != -(move)) &&
                    (c <= max_consec) &&
                    (visited[key] > (heat(next) + hl))) {

                visited[key] = heat(next) + hl;
                queue.push(Pos {next, d, heat(next) + hl, c});
            }
        }
    }

    return best;
}


size_t hash(const std::complex<int>& pos,
            const std::complex<int>& move,
            const size_t& consec) {

    size_t seed = 0;

    seed ^= static_cast<uint8_t>(pos.real());
    seed = seed << 8;
    seed ^= static_cast<uint8_t>(pos.imag());
    seed = seed << 8;
    seed ^= static_cast<int8_t>(move.real());
    seed = seed << 8;
    seed ^= static_cast<int8_t>(move.imag());
    seed = seed << 8;
    seed ^= static_cast<uint8_t>(consec);

    return seed;
}