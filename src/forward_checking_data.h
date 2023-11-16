#pragma once

#include "crossword_entry.h"
#include "constraints.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

struct Forward_Checking_Data
{
    Forward_Checking_Data(
        const std::unordered_map<int, Constraints>&,
        const std::vector<std::vector<char>>&,
        const std::vector<Crossword_Entry>&,
        const std::unordered_map<int, std::vector<std::string>>&);

    std::vector<std::pair<int, std::string>> eliminate_words(const std::vector<std::vector<char>>&, const std::string&, int, int, int, char, const std::vector<Crossword_Entry>&);
    void add_words(const std::vector<std::pair<int, std::string>>&);

    std::unordered_map<int, std::vector<std::string>> eligible_words;
    std::unordered_map<int, Constraints> constraints;
};
