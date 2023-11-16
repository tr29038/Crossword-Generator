#include "lcv_heuristic.h"

#include "crossword_utils.h"

#include <algorithm>
#include <climits>
#include <iostream>

//! @brief Build a mapping of overlapping entries.
//! @param puzzle The crossword puzzle.
//! @param entries List of all entries.
//! @return Mapping of overlapping entries.
std::unordered_map<int, Constraints> LCV_Heuristic::build_constraints(const std::vector<std::vector<char>>& puzzle, const std::vector<Crossword_Entry>& entries)
{
    std::unordered_map<int, Constraints> constraints;
    for (const auto& entry : entries)
    {
        auto directions = Crossword_Utils::get_entry_directions(puzzle, entry.x, entry.y);
        Constraints constraint(directions, entry.x, entry.y);
        constraints.emplace(entry.number, constraint);
    }

    for (auto& entry_checker : constraints)
    {
        auto x_checker = entry_checker.second.x;
        auto y_checker = entry_checker.second.y;
        for (auto& entry_checkee : constraints)
        {
            // Same entry, disregard.
            if (entry_checker.first == entry_checkee.first)
                continue;

            bool checker_has_across = std::find(entry_checker.second.directions.begin(), entry_checker.second.directions.end(), 'a') != entry_checker.second.directions.end();
            bool checker_has_down = std::find(entry_checker.second.directions.begin(), entry_checker.second.directions.end(), 'd') != entry_checker.second.directions.end();

            auto x_checkee = entry_checkee.second.x;
            auto y_checkee = entry_checkee.second.y;
            for (auto direction : entry_checkee.second.directions)
            {
                if (direction == 'a' && checker_has_down)
                {
                    auto length_checkee = Crossword_Utils::get_entry_length(puzzle, x_checkee, y_checkee, direction);
                    auto length_checker = Crossword_Utils::get_entry_length(puzzle, x_checker, y_checker, 'd');
                    if (x_checker < x_checkee + length_checkee && x_checker >= x_checkee && y_checkee < y_checker + length_checker && y_checkee >= y_checker)
                        entry_checker.second.shared_entries_down[entry_checkee.first].emplace_back(x_checker, y_checkee);
                }
                else if (direction == 'd' && checker_has_across)
                {
                    auto length_checkee = Crossword_Utils::get_entry_length(puzzle, x_checkee, y_checkee, direction);
                    auto length_checker = Crossword_Utils::get_entry_length(puzzle, x_checker, y_checker, 'a');
                    if (y_checker < y_checkee + length_checkee && y_checker >= y_checkee && x_checkee < x_checker + length_checker && x_checkee >= x_checker)
                        entry_checker.second.shared_entries_across[entry_checkee.first].emplace_back(x_checkee, y_checker);
                }
            }
        }
    }

    return constraints;
}

//! @brief Perform the least constraining value heuristic and get the next entry to fill.
//! @param puzzle The crossword puzzle.
//! @param entries All entries.
//! @param constrained_words Mapping of word length to dictionary words.
//! @param constraints Mapping of entries to overlapped entries.
//! @return The next entry and its direction to use.
std::pair<int, char> LCV_Heuristic::perform(const std::vector<std::vector<char>>& puzzle, const std::vector<Crossword_Entry>& entries, std::unordered_map<int, std::vector<std::string>>& constrained_words, std::unordered_map<int, Constraints>& constraints)
{
    int current_entry_index = -1;
    int current_similar_cells = INT_MIN;
    char current_direction;
    for (int i = 0; i < entries.size(); ++i)
    {
        auto constraint_directions = Crossword_Utils::get_entry_directions(puzzle, entries[i].x, entries[i].y);
        for (auto constraint_direction : constraint_directions)
        {
            if (Crossword_Utils::is_full(puzzle, entries[i].x, entries[i].y, constraint_direction))
                continue;

            int constraint_similars = 0;
            if (constraint_direction == 'a')
            {
                for (const auto& similar_entry : constraints.at(entries[i].number).shared_entries_down)
                {
                    for (const auto& pos : similar_entry.second)
                        // Is a filled cell (i.e., an alphabetic character)
                        if (puzzle[pos.second][pos.first] != ' ')
                            ++constraint_similars;
                }
            }
            else
            {
                for (const auto& similar_entry : constraints.at(entries[i].number).shared_entries_across)
                {
                    for (const auto& pos : similar_entry.second)
                    {
                        // Is a filled cell (i.e., an alphabetic character)
                        if (puzzle[pos.second][pos.first] != ' ')
                            ++constraint_similars;
                    }
                }
            }

            if (constraint_similars > current_similar_cells)
            {
                current_similar_cells = constraint_similars;
                current_entry_index = i;
                current_direction = constraint_direction;
            }
        }
    }

    // Should theoretically never happen
    if (current_entry_index == -1)
        throw std::runtime_error("bruh what");

    return {current_entry_index, current_direction};
}
