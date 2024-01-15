#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <chrono>

///////////////////////////////////////////////////////////////////////////////
// Hand Struct
///////////////////////////////////////////////////////////////////////////////

struct Hand {
    Hand() = default;

    Hand(std::string card_labels, size_t bid_value) {
        cards = card_labels;
        bid = bid_value;

        std::map<char, size_t> card_count;
        std::vector<size_t> raw_counts;

        for (auto& card : card_labels) {
            card_count[card]++;
        }

        for (auto& card : card_count) {
            raw_counts.push_back(card.second);
        }

        std::sort(raw_counts.begin(), raw_counts.end(), std::greater<int>());

        for (auto& count : raw_counts) {
            hand_key += std::to_string(count);
        }

        first_ordering = HAND_TYPE.at(hand_key);
    }

    friend bool operator< (const Hand& hand_1, const Hand& hand_2);

    std::string cards;
    size_t first_ordering;
    std::string hand_key;
    size_t bid;

    static const inline std::map<std::string, size_t> HAND_TYPE = {
        {"5", 7},
        {"41", 6},
        {"32", 5},
        {"311", 4},
        {"221", 3},
        {"2111", 2},
        {"11111", 1}
    };

    static const inline std::map<char, size_t> CARD_TYPE = {
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };
};

bool operator< (const Hand& hand_1, const Hand& hand_2) {
    bool less_than = (hand_1.first_ordering < hand_2.first_ordering);

    if (hand_1.first_ordering == hand_2.first_ordering) {
        bool found_stronger = false;
        for (size_t i = 0; !found_stronger && i < 5; ++i) {
            size_t lhs = std::isdigit(hand_1.cards[i]) ? hand_1.cards[i] - '0' :
                         Hand::CARD_TYPE.at(hand_1.cards[i]);
            size_t rhs = std::isdigit(hand_2.cards[i]) ? hand_2.cards[i] - '0' :
                         Hand::CARD_TYPE.at(hand_2.cards[i]);

            if (rhs != lhs) {
                found_stronger = true;
                less_than = (lhs < rhs);
            }
        }
    }

    return less_than;
}

///////////////////////////////////////////////////////////////////////////////
// Joker_Wild struct
///////////////////////////////////////////////////////////////////////////////

struct Joker_Wild : Hand {
    Joker_Wild(std::string card_labels, size_t bid_value): Hand() {
        cards = card_labels;
        bid = bid_value;
        size_t joker_count = 0;

        std::map<char, size_t> card_count;
        std::vector<size_t> raw_counts;

        for (auto& card : card_labels) {
            if (card == 'J') {
                joker_count++;
            } else {
                card_count[card]++;
            }
        }

        for (auto& card : card_count) {
            raw_counts.push_back(card.second);
        }

        if (!raw_counts.empty()) {
            std::sort(raw_counts.begin(), raw_counts.end(), std::greater<int>());
            raw_counts[0] += joker_count;

            for (auto& count : raw_counts) {
                hand_key += std::to_string(count);
            }
        } else {
            hand_key = "5";
        }

        first_ordering = HAND_TYPE.at(hand_key);
    }

    friend bool operator< (const Joker_Wild& hand_1, const Joker_Wild& hand_2);

    static const inline std::map<char, size_t> CARD_TYPE = {
        {'T', 10},
        {'J', 1},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };
};

bool operator< (const Joker_Wild& hand_1, const Joker_Wild& hand_2) {
    bool less_than = (hand_1.first_ordering < hand_2.first_ordering);

    if (hand_1.first_ordering == hand_2.first_ordering) {
        bool found_stronger = false;
        for (size_t i = 0; !found_stronger && i < 5; ++i) {
            size_t lhs = std::isdigit(hand_1.cards[i]) ? hand_1.cards[i] - '0' :
                         Joker_Wild::CARD_TYPE.at(hand_1.cards[i]);
            size_t rhs = std::isdigit(hand_2.cards[i]) ? hand_2.cards[i] - '0' :
                         Joker_Wild::CARD_TYPE.at(hand_2.cards[i]);

            if (rhs != lhs) {
                found_stronger = true;
                less_than = (lhs < rhs);
            }
        }
    }

    return less_than;
}

///////////////////////////////////////////////////////////////////////////////


size_t part_1(std::string path);
size_t part_2(std::string path);

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string path = "/home/bforrest/aoc23/day7/input.txt";
    size_t winnings_1 = 0;
    size_t winnings_2 = 0;

    winnings_1 = part_1(path);
    winnings_2 = part_2(path);


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (stop - start).count() / 1e9;

    std::cout << "The solution to part 1 is: " << winnings_1 << std::endl;
    std::cout << "The solution to part 2 is: " << winnings_2 << std::endl;
    std::cout << duration << std::endl;

    return 0;
}

size_t part_1(std::string path) {
    std::ifstream infile(path);
    std::string cards;
    size_t bid;
    std::vector<Hand> hands;
    size_t winnings = 0;

    while(infile >> cards >> bid) {
        hands.push_back(Hand(cards, bid));
    }

    std::sort(hands.begin(), hands.end());

    for (size_t i = 0; i < hands.size(); ++i) {
        winnings += (hands[i].bid * (i + 1));
    }

    infile.close();

    return winnings;
}

size_t part_2(std::string path) {
    std::ifstream infile(path);
    std::string cards;
    size_t bid;
    std::vector<Joker_Wild> hands;
    size_t winnings = 0;

    while(infile >> cards >> bid) {
        hands.push_back(Joker_Wild(cards, bid));
    }

    std::sort(hands.begin(), hands.end());

    for (size_t i = 0; i < hands.size(); ++i) {
        winnings += (hands[i].bid * (i + 1));
    }

    infile.close();

    return winnings;
}