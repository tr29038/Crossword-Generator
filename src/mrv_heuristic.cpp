#include "mrv_heuristic.h"

#include "crossword_utils.h"

#include <climits>
#include <iostream>

//! @brief Perform minimum remaining values heuristic and get the next entry to use.
//! @param puzzle The crossword puzzle.
//! @param entries All entries.
//! @param constrained_words Mapping of word length to corresponding to dictionary words.
//! @return The next entry and its direction to use.
std::pair<int, char> MRV_Heuristic::perform(const std::vector<std::vector<char>>& puzzle, const std::vector<Crossword_Entry>& entries, std::unordered_map<int, std::vector<std::string>>& constrained_words)
{
    int entry_index_mrv = -1;
    int entry_candidate_words_mrv = INT_MAX;
    char entry_direction_mrv;
    for (int i = 0; i < entries.size(); ++i)
    {
        auto directions = Crossword_Utils::get_entry_directions(puzzle, entries[i].x, entries[i].y);
        for (auto direction : directions)
        {
            if (Crossword_Utils::is_full(puzzle, entries[i].x, entries[i].y, direction))
                continue;

            auto entry_length = Crossword_Utils::get_entry_length(puzzle, entries[i].x, entries[i].y, direction);
            auto entry_candidate_words = constrained_words[entry_length].size();
            if (entry_candidate_words < entry_candidate_words_mrv)
            {
                entry_candidate_words_mrv = entry_candidate_words;
                entry_index_mrv = i;
                entry_direction_mrv = direction;
            }
        }
    }

    if (entry_index_mrv == -1)
        throw std::runtime_error("bruh what");

    return {entry_index_mrv, entry_direction_mrv};
}
