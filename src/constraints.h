#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

struct Constraints
{
    Constraints(std::vector<char> _directions, int _x, int _y) :
        directions(_directions),
        x(_x),
        y(_y)
    {}

    std::vector<char> directions;
    int x, y;
    std::unordered_map<int, std::vector<std::pair<int, int>>> shared_entries_down, shared_entries_across;
};
