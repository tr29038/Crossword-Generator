#pragma once

#include "crossword_entry.h"
#include "crossword_utils.h"
#include "mrv_heuristic.h"
#include "lcv_heuristic.h"
#include "forward_checking_data.h"

#include <vector>
#include <unordered_map>
#include <optional>
#include <cctype>
#include <iostream>
#include <type_traits>

class Crossword_Constructor
{
    public:
        Crossword_Constructor() = delete;
        Crossword_Constructor(const std::vector<std::vector<char>>&, const std::vector<Crossword_Entry>&, const std::unordered_map<int, std::vector<std::string>>&);

        bool construct_via_backtracking(const std::vector<std::vector<char>>&);
        bool construct_via_backtracking(std::vector<std::vector<char>>, std::size_t, std::vector<char>);

        bool construct_via_mrv(std::vector<std::vector<char>>);
        bool construct_via_lcv(std::vector<std::vector<char>>, std::unordered_map<int, Constraints>&);
        bool construct_via_mrv_and_fc(std::vector<std::vector<char>>, Forward_Checking_Data&);

    private:
        std::vector<std::vector<char>> puzzle;
        std::vector<Crossword_Entry> entries;
        std::unordered_map<int, std::vector<std::string>> constrained_words;
};

Crossword_Constructor::Crossword_Constructor(const std::vector<std::vector<char>>& puzzle_, const std::vector<Crossword_Entry>& entries_, const std::unordered_map<int, std::vector<std::string>>& constrained_words_) :
    puzzle(puzzle_),
    entries(entries_),
    constrained_words(constrained_words_)
{}

//! @brief Generate a crossword puzzle via backtracking with no heuristics involved.
//! @param puzzle The puzzle to fill.
//! @return True if the puzzle frame was fillable, false otherwise.
bool Crossword_Constructor::construct_via_backtracking(const std::vector<std::vector<char>>& puzzle)
{
    return construct_via_backtracking(puzzle, 0, { });
}

//! @brief Generate a crossword puzzle via backtracking with no heuristics involved.
//! @param solution The puzzle filled with an intermediary solution amidst the backtracking route.
//! @param entry_index Index of the puzzle word entry to use.
//! @param directions The supported directions of the puzzle word entry.
//! @return True if the puzzle frame was fillable, false otherwise.
bool Crossword_Constructor::construct_via_backtracking(
    std::vector<std::vector<char>> solution,
    std::size_t entry_index,
    std::vector<char> directions)
{
    if (Crossword_Utils::is_full(solution))
    {
        Crossword_Utils::print(std::cout, solution);
        return true;
    }

    auto entry = entries[entry_index];
    int y = entry.y, x = entry.x;

    if (directions.empty())
        directions = Crossword_Utils::get_entry_directions(puzzle, x, y);

    char direction = directions.back();
    directions.pop_back();

    if (directions.empty())
        ++entry_index;

    int entry_length = Crossword_Utils::get_entry_length(puzzle, x, y, direction);

    for (const auto& word : constrained_words[entry_length])
    {
        bool finished_word = true;
        std::vector<std::pair<int, int>> revert_on_fail;
        for (auto c : word)
        {
            if (isalpha(solution[y][x]) && solution[y][x] != c)
            {
                finished_word = false;
                break;
            }

            if (solution[y][x] == ' ')
            {
                solution[y][x] = c;
                revert_on_fail.emplace_back(y, x);
            }

            if (direction == 'a')
                ++x;
            else
                ++y;
        }

        if (finished_word)
        {
            if (construct_via_backtracking(solution, entry_index, directions))
                return true;
        }

        while (!revert_on_fail.empty())
        {
            auto [y, x] = revert_on_fail.back();
            revert_on_fail.pop_back();

            solution[y][x] = ' ';
        }

        y = entry.y;
        x = entry.x;
    }

    return false;
}

//! @brief Generate a crossword puzzle via backtracking using the MRV heuristic.
//! @param solution The puzzle filled with an intermediary solution amidst the backtracking route.
//! @return True if the puzzle frame was fillable, false otherwise.
bool Crossword_Constructor::construct_via_mrv(std::vector<std::vector<char>> solution)
{
    if (Crossword_Utils::is_full(solution))
    {
        Crossword_Utils::print(std::cout, solution);
        return true;
    }

    auto [entry_index, direction] = MRV_Heuristic::perform(solution, entries, constrained_words);
    auto entry = entries[entry_index];
    int x = entry.x, y = entry.y;
    int entry_length = Crossword_Utils::get_entry_length(puzzle, x, y, direction);

    for (const auto& word : constrained_words[entry_length])
    {
        bool finished_word = true;
        std::vector<std::pair<int, int>> revert_on_fail;
        for (auto c : word)
        {
            if (isalpha(solution[y][x]) && solution[y][x] != c)
            {
                finished_word = false;
                break;
            }

            if (solution[y][x] == ' ')
            {
                solution[y][x] = c;
                revert_on_fail.emplace_back(y, x);
            }

            if (direction == 'a')
                ++x;
            else
                ++y;
        }

        if (finished_word)
        {
            if (construct_via_mrv(solution))
                return true;
        }

        while (!revert_on_fail.empty())
        {
            auto [y, x] = revert_on_fail.back();
            revert_on_fail.pop_back();

            solution[y][x] = ' ';
        }

        y = entry.y;
        x = entry.x;
    }

    return false;
}

//! @brief Generate a crossword puzzle via backtracking using the LCV heuristic.
//! @param solution The puzzle filled with an intermediary solution amidst the backtracking route.
//! @return True if the puzzle frame was fillable, false otherwise.
bool Crossword_Constructor::construct_via_lcv(std::vector<std::vector<char>> solution, std::unordered_map<int, Constraints>& constraints)
{
    if (Crossword_Utils::is_full(solution))
    {
        Crossword_Utils::print(std::cout, solution);
        return true;
    }

    auto [entry_index, direction] = LCV_Heuristic::perform(solution, entries, constrained_words, constraints);
    auto entry = entries[entry_index];
    int x = entry.x, y = entry.y;
    int entry_length = Crossword_Utils::get_entry_length(puzzle, x, y, direction);

    for (const auto& word : constrained_words[entry_length])
    {
        bool finished_word = true;
        std::vector<std::pair<int, int>> revert_on_fail;
        for (auto c : word)
        {
            if (isalpha(solution[y][x]) && solution[y][x] != c)
            {
                finished_word = false;
                break;
            }

            if (solution[y][x] == ' ')
            {
                solution[y][x] = c;
                revert_on_fail.emplace_back(y, x);
            }

            if (direction == 'a')
                ++x;
            else
                ++y;
        }

        if (finished_word)
        {
            if (construct_via_lcv(solution, constraints))
                return true;
        }

        while (!revert_on_fail.empty())
        {
            auto [y, x] = revert_on_fail.back();
            revert_on_fail.pop_back();

            solution[y][x] = ' ';
        }

        y = entry.y;
        x = entry.x;
    }

    return false;
}

//! @brief Generate a crossword puzzle via backtracking using the MRV heuristic and forward checking.
//! @param solution The puzzle filled with an intermediary solution amidst the backtracking route.
//! @return True if the puzzle frame was fillable, false otherwise.
bool Crossword_Constructor::construct_via_mrv_and_fc(std::vector<std::vector<char>> solution, Forward_Checking_Data& checked_words)
{
    if (Crossword_Utils::is_full(solution))
    {
        Crossword_Utils::print(std::cout, solution);
        return true;
    }

    auto [entry_index, direction] = MRV_Heuristic::perform(solution, entries, constrained_words);
    auto entry = entries[entry_index];
    int x = entry.x, y = entry.y;
    int entry_length = Crossword_Utils::get_entry_length(puzzle, x, y, direction);

    const auto& eligible_words = checked_words.eligible_words.at(entry.number);
    for (const auto& word : eligible_words)
    {
        bool finished_word = true;
        std::vector<std::pair<int, int>> revert_on_fail;
        std::vector<std::pair<int, std::string>> eliminated_words;
        for (auto c : word)
        {
            if (isalpha(solution[y][x]) && solution[y][x] != c)
            {
                finished_word = false;
                break;
            }

            if (solution[y][x] == ' ')
            {
                solution[y][x] = c;
                revert_on_fail.emplace_back(y, x);
            }


            if (direction == 'a')
                ++x;
            else
                ++y;
        }

        if (finished_word)
        {
            eliminated_words = checked_words.eliminate_words(solution, word, entry.number, entry.x, entry.y, direction, entries);
            if (construct_via_mrv_and_fc(solution, checked_words))
                return true;
        }

        while (!revert_on_fail.empty())
        {
            auto [y, x] = revert_on_fail.back();
            revert_on_fail.pop_back();

            solution[y][x] = ' ';
        }

        if (!eliminated_words.empty())
        {
            checked_words.add_words(eliminated_words);
            eliminated_words.clear();
        }

        y = entry.y;
        x = entry.x;
    }

    return false;
}

