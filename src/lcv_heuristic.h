#pragma once

#include "crossword_entry.h"
#include "constraints.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

struct LCV_Heuristic
{
    static std::unordered_map<int, Constraints> build_constraints(const std::vector<std::vector<char>>&, const std::vector<Crossword_Entry>&);
    static std::pair<int, char> perform(const std::vector<std::vector<char>>& puzzle, const std::vector<Crossword_Entry>&, std::unordered_map<int, std::vector<std::string>>&, std::unordered_map<int, Constraints>&);
};
