#pragma once

#include "crossword_entry.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

struct MRV_Heuristic
{
    static std::pair<int, char> perform(const std::vector<std::vector<char>>& puzzle, const std::vector<Crossword_Entry>&, std::unordered_map<int, std::vector<std::string>>&);
};
