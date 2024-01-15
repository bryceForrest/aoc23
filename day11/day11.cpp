#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

struct Sparse {
    Sparse(const std::vector<size_t> row_indices,
           const std::vector<size_t> col_indices,
           const std::pair<size_t, size_t> dim_input) {
        row = row_indices;
        col = col_indices;
        dim = dim_input;
    }

    void insert_row(const size_t row_num, const size_t count) {
        for (auto& r : row) {
            if (r > row_num) {
                r += count;
            }
        }

        dim.first += count;
    }

    void insert_col(const size_t col_num,  const size_t count) {
        for (auto& c : col) {
            if (c > col_num) {
                c += count;
            }
        }

        dim.second += count;
    }

    std::vector<size_t> row;
    std::vector<size_t> col;
    std::pair<size_t, size_t> dim;
};

Sparse parse(std::string path);
void expand(Sparse& image, size_t expand_size = 1);
size_t pairwise_distance(Sparse& image);

int main() {
    std::string path = "/home/bforrest/aoc23/day11/input.txt";
    Sparse image = parse(path);
    expand(image);

    auto score_1 = pairwise_distance(image);

    image = parse(path);
    expand(image, 1000000 - 1);

    auto score_2 = pairwise_distance(image);

    std::cout << "The solution to part 1 is: " << score_1 << std::endl;
    std::cout << "The solution to part 2 is: " << score_2 << std::endl;

    return 0;
}

Sparse parse(std::string path) {
    std::ifstream infile(path);
    std::vector<size_t> row_indices;
    std::vector<size_t> col_indices;
    std::pair<size_t, size_t> dim;

    char ch;

    size_t i = 0;
    size_t j = 0;

    while (infile) {
        while (infile.get(ch) && ch != '\n') {
            if (ch == '#') {
                row_indices.push_back(i);
                col_indices.push_back(j);
            }

            ++j;
        }

        dim.second = std::max(dim.second, j);

        j = 0;
        ++i;
    }

    dim.first = i - 1;

    return Sparse(row_indices, col_indices, dim);
}

void expand(Sparse& image, size_t expand_size) {
    for (size_t i = 0; i < image.dim.first; ++i) {
        if (std::find(image.row.begin(), image.row.end(), i) == image.row.end()) {
            image.insert_row(i, expand_size);
            i += expand_size;
        }
    }

    for (size_t i = 0; i < image.dim.second; ++i) {
        if (std::find(image.col.begin(), image.col.end(), i) == image.col.end()) {
            image.insert_col(i, expand_size);
            i += expand_size;
        }
    }
}

size_t pairwise_distance(Sparse& image) {
    size_t total_dist = 0;
    for (size_t i = 0; i < (image.row.size() - 1); ++i) {
        auto row_pos = image.row[i];
        auto col_pos = image.col[i];

        for (size_t j = i + 1; j < image.row.size(); ++j) {
            total_dist += abs(image.row[i] - image.row[j]) +
                          abs(image.col[i] - image.col[j]);
        }
    }

    return total_dist;
}