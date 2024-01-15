#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cctype>
#include <sstream>

int parse_input_p1(const std::string& path);
int parse_input_p2(const std::string& path);
int check_neighbors_p1(const std::vector<std::string>&
                       schematic, std::vector<std::pair<int, int>>& indices);
int check_neighbors_p2(const std::vector<std::string>&
                       schematic, std::vector<std::pair<int, int>>& indices);
int trav(const std::vector<std::string>& schematic,
         std::vector<std::string>& map, const int& i, const int& j);
bool trav_left(const std::vector<std::string>& schematic,
               std::vector<std::string>& map, const int& i, const int& j);
bool trav_right(const std::vector<std::string>& schematic,
                std::vector<std::string>& map, const int& i, const int& j);
int map_sum(std::vector<std::string> map);
int map_prod(std::vector<std::string> map);

template<class T, class S>
std::ostream& operator << (std::ostream& os, const std::pair<T, S>& pair);
template < class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr);


int main() {
    std::string line = "";
    std::string file_path = "/home/bforrest/aoc23/day3/input_1.txt";

    int score_p1 = parse_input_p1(file_path);
    int score_p2 = parse_input_p2(file_path);

    // std::cout << map << std::endl;
    std::cout << "The solution for part 1 is: " << score_p1 << std::endl;
    std::cout << "The solution for part 2 is: " << score_p2 << std::endl;
    return 0;
}

/// @brief Opens input file and parses into vector of strings.
/// Collects indices of symbols to look around.
/// Calls check_neighbors_p1
/// @param path Path to input file
/// @return check_neighbors_p1 return, sum of valid part numbers
int parse_input_p1(const std::string& path) {
    std::vector<std::string> schematic;
    std::vector<std::pair<int, int>> indices;
    std::ifstream infile(path);

    std::string line = "";
    int row_index = 0;

    while (std::getline(infile, line)) {
        std::string row = "";
        int col_index = 0;

        for (auto& ch : line) {
            row.push_back(ch);

            if ((ch != '.') && std::ispunct(ch)) {
                indices.push_back(
                    std::pair<int, int> {row_index, col_index}
                );
            }

            ++col_index;
        }

        schematic.push_back(row);
        ++row_index;
    }

    return check_neighbors_p1(schematic, indices);
}

/// @brief Opens input file and parses into vector of strings.
/// Collects indices of *s to look around.
/// Calls check_neighbors_p2
/// @param path Path to input file
/// @return check_neighbors_p1 return, product of part numbers (gear ratio)
int parse_input_p2(const std::string& path) {
    std::vector<std::string> schematic;
    std::vector<std::pair<int, int>> indices;
    std::ifstream infile(path);

    std::string line = "";
    int row_index = 0;

    while (std::getline(infile, line)) {
        std::string row = "";
        int col_index = 0;

        for (auto& ch : line) {
            row.push_back(ch);

            if (ch == '*') {
                indices.push_back(
                    std::pair<int, int> {row_index, col_index}
                );
            }

            ++col_index;
        }

        schematic.push_back(row);
        ++row_index;
    }

    return check_neighbors_p2(schematic, indices);
}

/// @brief Iterates over indices vector to check around symbols.
/// Calls trav to traverse neighboring indices.
/// @param schematic schematic of parts read from input file
/// @param indices ij indices of symbols in input
/// @return map_sum return, sum of valid part numbers in schematic.
int check_neighbors_p1(const std::vector<std::string>&
                       schematic, std::vector<std::pair<int, int>>& indices) {
    std::vector<std::string> map (schematic.size(),
                                  std::string (schematic[0].size(), ' '));

    while (!indices.empty()) {
        std::pair<int, int> ij = indices.back();
        int i = ij.first;
        int j = ij.second;

        trav(schematic, map, i, j);

        indices.pop_back();
    }

    return map_sum(map);
}

/// @brief Iterates over indices vector to check around *s.
/// Calls trav to traverse neighboring indices. Stores a new vector
/// vector of indices if a gear has exactly two neighbors. Makes a new
/// map of only those neighboring parts for each gear.
/// @param schematic schematic of parts read from input file
/// @param indices ij indices of symbols in input
/// @return map_prod return, product of valid part numbers around gear.
int check_neighbors_p2(const std::vector<std::string>&
                       schematic, std::vector<std::pair<int, int>>& indices) {
    std::vector<std::string> map (schematic.size(),
                                  std::string (schematic[0].size(), ' '));
    std::vector<std::pair<int, int>> gear_indices;

    while (!indices.empty()) {
        std::pair<int, int> ij = indices.back();
        int i = ij.first;
        int j = ij.second;

        int nbors = trav(schematic, map, i, j);

        if (nbors == 2) {
            gear_indices.push_back(ij);
        }

        indices.pop_back();
    }

    int score = 0;

    while (!gear_indices.empty()) {
        std::pair<int, int> ij = gear_indices.back();
        int i = ij.first;
        int j = ij.second;
        std::vector<std::string> gear_map (schematic.size(),
                                           std::string (schematic[0].size(), ' '));

        trav(schematic, gear_map, i, j);
        score += map_prod(gear_map);
        gear_indices.pop_back();
    }

    return score;
}

/// @brief Calls directional traversal around ij index of schematic
/// @param schematic Schematic we are traversing
/// @param map Fills in with valid part numbers from schematic
/// @param i row index
/// @param j column index
/// @return The number of neighbors around ij (for part 2)
int trav(const std::vector<std::string>& schematic,
         std::vector<std::string>& map, const int& i, const int& j) {
    // use up all the variables in the world
    bool left_up = false;
    bool right_up = false;
    bool left_up_diag = false;
    bool right_up_diag = false;
    bool left_adj = false;
    bool right_adj = false;
    bool left_down = false;
    bool right_down = false;
    bool left_down_diag = false;
    bool right_down_diag = false;

    // trav above
    left_up = trav_left(schematic, map, i - 1, j);
    right_up = trav_right(schematic, map, i - 1, j);

    // trav above diagonal if directly above found nothing
    if (!(left_up || right_up)) {
        left_up_diag = trav_left(schematic, map, i - 1, j - 1);
        right_up_diag = trav_right(schematic, map, i - 1, j + 1);
    }

    // trav adjacecent
    left_adj = trav_left(schematic, map, i, j - 1);
    right_adj = trav_right(schematic, map, i, j + 1);

    // trav below
    left_down = trav_left(schematic, map, i + 1, j);
    right_down = trav_right(schematic, map, i + 1, j);

    // trav below diagonal if directly below found nothing
    if (!(left_down || right_down)) {
        left_down_diag = trav_left(schematic, map, i + 1, j - 1);
        right_down_diag = trav_right(schematic, map, i + 1, j + 1);
    }

    int top_nbors = static_cast<int>(left_up_diag) + static_cast<int>
                    (right_up_diag) + static_cast<int>(left_up || right_up);
    int adj_nbors = static_cast<int>(left_adj) + static_cast<int>(right_adj);
    int bottom_nbors = static_cast<int>(left_down_diag) + static_cast<int>
                       (right_down_diag) + static_cast<int>(left_down || right_down);

    return top_nbors + adj_nbors + bottom_nbors;
}

/// @brief Recursive traversal to the left of ij
/// @param schematic Schematic we are traversing
/// @param map Fills in with valid part numbers from schematic
/// @param i row index
/// @param j column index
/// @return True if we hit a number somewhere (a neighbor was found)
bool trav_left(const std::vector<std::string>& schematic,
               std::vector<std::string>& map, const int& i, const int& j) {
    if (i < 0 || i >= schematic.size() ||
            j < 0 || j >= schematic[0].size()) {
        return false;
    }

    if (std::ispunct(schematic[i][j])) {
        return false;
    }

    trav_left(schematic, map, i, j - 1);
    map[i][j] = schematic[i][j];

    return true;
}

/// @brief Recursive traversal to the right of ij
/// @param schematic Schematic we are traversing
/// @param map Fills in with valid part numbers from schematic
/// @param i row index
/// @param j column index
/// @return True if we hit a number somewhere (a neighbor was found)
bool trav_right(const std::vector<std::string>& schematic,
                std::vector<std::string>& map, const int& i, const int& j) {
    if (i < 0 || i >= schematic.size() ||
            j < 0 || j >= schematic[0].size()) {
        return false;
    }

    if (std::ispunct(schematic[i][j])) {
        return false;
    }

    trav_right(schematic, map, i, j + 1);
    map[i][j] = schematic[i][j];

    return true;
}

/// @brief Given a map of valid part numbers, read in as stringstream
/// and add up the part numbers
/// @param map map of valid part numbers
/// @return sum of part numbers
int map_sum(std::vector<std::string> map) {
    int score = 0;

    for (auto& row : map) {
        std::stringstream ss (row);
        int temp = 0;

        while (ss >> temp) {
            score += temp;
        }
    }

    return score;
}

/// @brief Given a map of valid part numbers, read in as stringstream
/// and multiply the part numbers
/// @param map map of valid part numbers
/// @return product of part numbers
/// @note This function is used for part 2, and assume there will only be
/// the neighboring part numbers for a single gear in a map.
int map_prod(std::vector<std::string> map) {
    int score = 1;

    for (auto& row : map) {
        std::stringstream ss (row);
        int temp = 0;

        while (ss >> temp) {
            score *= temp;
        }
    }

    return score;
}

///////////////////////////////////////////////////////////////////////////////
// Helper prints
template<class T, class S>
std::ostream& operator << (std::ostream& os, const std::pair<T, S>& pair) {
    std::cout << "(" << pair.first << ", " << pair.second << ")";

    return os;
}

template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& arr) {
    for (auto& col : arr) {
        std::cout << col << "\n";
    }

    return os;
}