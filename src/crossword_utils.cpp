#include "crossword_utils.h"

#include <fstream>
#include <iostream>
#include <cctype>

std::unordered_map<char, bool> Crossword_Utils::valid_cells = { { '#', true }, { ' ', true }, { '[', true }, { ']', true }, { '\n', true } };
std::string Crossword_Utils::puzzle_file = "puzzle.txt";
std::string Crossword_Utils::dictionary_file = "dictionary.txt";

//! @brief Parse the puzzle's text file and construct a 2 dimensional array from it.
//! @param crossword_directory The relative or absolute path to the crossword puzzle directory.
//! @return The puzzle file converted into a 2 dimensional array.
std::pair<std::vector<std::vector<char>>, std::vector<Crossword_Entry>>
Crossword_Utils::parse_puzzle(const std::string& crossword_directory)
{
    std::vector<std::vector<char>> parsed_puzzle;
    parsed_puzzle.push_back(std::vector<char>());
    std::vector<Crossword_Entry> entries;
    std::ifstream puzzle_input(crossword_directory + "/" + puzzle_file);

    while (puzzle_input.peek() != EOF)
    {
        char cell = puzzle_input.get();
        if (!valid_cells[cell])
            throw std::runtime_error("Invalid cell entry: " + cell);

        if (puzzle_input.peek() == EOF)
            break;

        if (cell == '\n')
        {
            parsed_puzzle.push_back(std::vector<char>());
        }
        else if (cell == '[')
        {
            std::string number;
            std::getline(puzzle_input, number, ']');
            entries.emplace_back(std::stoi(number), parsed_puzzle[parsed_puzzle.size() - 1].size(), parsed_puzzle.size() - 1);
            parsed_puzzle.back().push_back(' ');
        }
        else
        {
            parsed_puzzle.back().push_back(cell);
        }
    }

    puzzle_input.close();

    return { parsed_puzzle, entries };
}

//! @brief Get a mapping of each puzzle word entry to the words of appropriate length.
//! @param crossword_directory The relative or absolute path to the crossword puzzle directory.
//! @return A mapping of each puzzle word entry to the words of appropriate length.
std::unordered_map<int, std::vector<std::string>> Crossword_Utils::get_constrained_words(const std::string& crossword_directory)
{
    std::unordered_map<int, std::vector<std::string>> words;
    std::ifstream words_stream(crossword_directory + "/" + dictionary_file);

    while (words_stream.peek() != EOF)
    {
        std::string word;
        std::getline(words_stream, word);
        words[word.length()].push_back(word);
    }

    words_stream.close();

    return words;
}

//! @brief Get a puzzle word entry's length.
//! @param puzzle The crossword puzzle.
//! @param x The x coordinate of the first character of the entry.
//! @param y The y coordinate of the first character of the entry.
//! @param direction The direction of the entry (across/down).
//! @return The length of the entry.
int Crossword_Utils::get_entry_length(const std::vector<std::vector<char>>& puzzle, int x, int y, char direction)
{
    int length = 0;
    if (direction == 'a')
    {
        while (x < puzzle[y].size() && (puzzle[y][x] == ' ' || isalpha(puzzle[y][x])))
        {
            ++length;
            ++x;
        }
    }
    else if (direction == 'd')
    {
        while (y < puzzle.size() && (puzzle[y][x] == ' ' || isalpha(puzzle[y][x])))
        {
            ++length;
            ++y;
        }
    }

    return length;
}

//! @brief Get a puzzle word entry's supported directions.
//! @param puzzle The crossword puzzle.
//! @param x the x coordinate of the first character of the entry.
//! @param y the y coordinate of the first character of the entry.
//! @return The supported directions (can only be 'a' and/or 'd').
std::vector<char> Crossword_Utils::get_entry_directions(const std::vector<std::vector<char>>& puzzle, int x, int y)
{
    std::vector<char> directions;
    if ((x - 1 < 0 || puzzle[y][x - 1] == '#') && x + 1 < puzzle[y].size() && (puzzle[y][x + 1] == ' ' || isalpha(puzzle[y][x + 1])))
        directions.push_back('a');
    if ((y - 1 < 0 || puzzle[y - 1][x] == '#') && y + 1 < puzzle.size() && (puzzle[y + 1][x] == ' ' || isalpha(puzzle[y + 1][x])))
        directions.push_back('d');

    return directions;
}

//! @brief Check if every empty cell in the puzzle is filled.
//! @param puzzle The crossword puzzle.
//! @return True if the puzzle is filled, false otherwise.
bool Crossword_Utils::is_full(const std::vector<std::vector<char>>& puzzle)
{
    for (const auto& row : puzzle)
    {
        for (auto entry : row)
        {
            if (entry == ' ')
                return false;
        }
    }

    return true;
}

//! @brief Check if a puzzle word entry is filled with a word.
//! @param puzzle The crossword puzzle.
//! @param x the x coordinate of the first character of the entry.
//! @param y the y coordinate of the first character of the entry.
//! @param direction The direction of the entry.
//! @return True if the entry is filled, false otherwise.
bool Crossword_Utils::is_full(const std::vector<std::vector<char>>& puzzle, int x, int y, char direction)
{
    while (y < puzzle.size() && x < puzzle[y].size() && puzzle[y][x] != '#')
    {
        if (puzzle[y][x] == ' ')
            return false;

        if (direction == 'a')
            ++x;
        else
            ++y;
    }

    return true;
}

//! @brief Print the puzzle to the console.
//! @param output_stream Output stream.
//! @param puzzle The crossword puzzle.
void Crossword_Utils::print(std::ostream& output_stream, const std::vector<std::vector<char>>& puzzle)
{
    std::string border(puzzle[0].size() * 2 + 1, '-');
    for (const auto& row : puzzle)
    {
        output_stream << border << '\n';
        output_stream << '|';
        for (const auto& c : row)
        {
            output_stream << c << '|';
        }
        output_stream << '\n';
    }
    output_stream << border << '\n';
}
