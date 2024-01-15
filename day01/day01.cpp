#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <utility>

int return_number(std::string str);
std::string word_to_num(std::string str);

int main() {
    std::string line;
    std::ifstream infile("input_1.txt");
    std::ofstream outfile("test_output.txt");

    int total = 0;

    while (std::getline(infile, line)) {
        std::string temp = word_to_num(line);
        total += return_number(temp);
    }

    std::cout << total << std::endl;

    infile.close();
    outfile.close();

    return 0;
}

/// @brief      Solution for part 1, helper for part 2.
///             Collects digit characters from string, collects first and last
///             characters, turns them into an integer.
/// @param str  String to find integers in.
/// @return     First/last digits of string as integer (ex: "6i0z7" -> 67).
int return_number(std::string str) {
    std::string str_num = {};
    int total = {};

    for (const auto ch : str) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            str_num += ch;
        }
    }

    std::string first_last = str_num.substr(0,1)
                             + str_num.substr(str_num.length() - 1, 1);

    return std::stoi(first_last);
}

/// @brief      Main part of solution for part 2.
///             Finds digit names and append them as their numerical digit
///             character. Digit characters are appended as is.
/// @param str  String to find digits and digit names in.
/// @return     New string of numerical digit characters.
std::string word_to_num(std::string str) {
    const std::unordered_map<std::string, std::string> digits = {
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"}
    };

    std::string new_str = "";

    for (size_t i = 0; i < str.length(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(str[i]))) {
            new_str += str[i];
        }

        for (const std::pair<const std::string, std::string>& key_value : digits) {
            size_t pos = str.find(key_value.first, i);

            if (pos == i) {
                new_str += digits.at(key_value.first);
            }
        }

    }

    return new_str;
}