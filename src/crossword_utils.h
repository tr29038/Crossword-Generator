#pragma once

#include "crossword_entry.h"

#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

struct Crossword_Utils
{
    Crossword_Utils() = delete;

    static std::pair<std::vector<std::vector<char>>, std::vector<Crossword_Entry>> parse_puzzle(const std::string&);
    static std::unordered_map<int, std::vector<std::string>> get_constrained_words(const std::string&);
    static int get_entry_length(const std::vector<std::vector<char>>&, int, int, char);
    static std::vector<char> get_entry_directions(const std::vector<std::vector<char>>&, int, int);
    static bool is_full(const std::vector<std::vector<char>>&);
    static bool is_full(const std::vector<std::vector<char>>&, int, int, char);
    static void print(std::ostream&, const std::vector<std::vector<char>>&);

    private:
        static std::unordered_map<char, bool> valid_cells;
        static std::string puzzle_file;
        static std::string dictionary_file;
};
