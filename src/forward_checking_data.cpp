#include "forward_checking_data.h"

#include "crossword_utils.h"

#include <algorithm>

//! @brief Create a mapping of entries to the respective words of appropriate length.
Forward_Checking_Data::Forward_Checking_Data(
    const std::unordered_map<int, Constraints>& _constraints,
    const std::vector<std::vector<char>>& puzzle,
    const std::vector<Crossword_Entry>& entries,
    const std::unordered_map<int, std::vector<std::string>>& constrained_words) :
        constraints(_constraints)
{
    for (const auto& entry : entries)
    {
        auto entry_directions = Crossword_Utils::get_entry_directions(puzzle, entry.x, entry.y);
        for (auto direction : entry_directions)
        {
            auto entry_length = Crossword_Utils::get_entry_length(puzzle, entry.x, entry.y, direction);
            auto words = constrained_words.at(entry_length);
            eligible_words.emplace(entry.number, words);
        }
    }
}

//! @brief Eliminate eligible words proactively.
//! @param puzzle The crossword puzzle.
//! @param used_word The word to check against.
//! @param entry The entry number to check against.
//! @param x The first position of the entry.
//! @param y The first position of the entry.
//! @param direction The direction of the entry.
//! @param entries All entries.
//! @return All eliminated words with their respective entry number.
std::vector<std::pair<int, std::string>> Forward_Checking_Data::eliminate_words(const std::vector<std::vector<char>>& puzzle, const std::string& used_word, int entry, int x, int y, char direction, const std::vector<Crossword_Entry>& entries)
{
    std::vector<std::pair<int, std::string>> eliminated_words;
    const auto& constraint = constraints.at(entry);
    if (direction == 'a')
    {
        for (const auto& shared_entry : constraint.shared_entries_down)
        {
            auto entry_actual = std::find_if(entries.begin(), entries.end(), [&](const Crossword_Entry& entry_pred) { return entry_pred.number == entry; });
            int position = y - entry_actual->y;
            for (const auto& pos : shared_entry.second)
            {
                auto& word_candidates = eligible_words.at(entry);
                auto it = word_candidates.begin();
                while (it != word_candidates.end())
                {
                    if ((*it)[position] == puzzle[y][x])
                    {
                        eliminated_words.emplace_back(shared_entry.first, *it);
                        it = word_candidates.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }
    }
    else
    {
        for (const auto& shared_entry : constraint.shared_entries_across)
        {
            auto entry_actual = std::find_if(entries.begin(), entries.end(), [&](const Crossword_Entry& entry_pred) { return entry_pred.number == entry; });
            int position = x - entry_actual->x;
            for (const auto& pos : shared_entry.second)
            {
                auto& word_candidates = eligible_words.at(entry);
                auto it = word_candidates.begin();
                while (it != word_candidates.end())
                {
                    if ((*it)[position] == puzzle[y][x])
                    {
                        eliminated_words.emplace_back(shared_entry.first, *it);
                        it = word_candidates.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }
    }

    return eliminated_words;
}

//! @brief Add words to the eligible words list.
//! @param words Words to add.
void Forward_Checking_Data::add_words(const std::vector<std::pair<int, std::string>>& words)
{
    for (const auto& word : words)
        eligible_words.at(word.first).push_back(word.second);
}
