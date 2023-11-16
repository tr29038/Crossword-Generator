#include "crossword_utils.h"
#include "crossword_constructor.h"
#include "mrv_heuristic.h"
#include "lcv_heuristic.h"
#include "forward_checking_data.h"

#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Correct usage: " << argv[0] << " <puzzle directory> <standard-backtracking|mrv|lcv|fc+mrv>\n";
        return 1;
    }

    std::string puzzle_directory = argv[1];
    if (puzzle_directory.back() == '/')
    {
        std::cout << "The provided puzzle directory argument should not end with a slash.\n";
        return 1;
    }

    std::string algorithm = argv[2];
    if (algorithm != "standard-backtracking" && algorithm != "mrv" && algorithm != "lcv" && algorithm != "fc+mrv")
    {
        std::cout << "Invalid algorithm provided. Valid options are standard-backtracking, mrv, lcv, and fc+mrv.\n";
        return 1;
    }

    auto [crossword_puzzle, crossword_entries] = Crossword_Utils::parse_puzzle(puzzle_directory);
    auto constrained_words = Crossword_Utils::get_constrained_words(puzzle_directory);

    Crossword_Constructor crossword_constructor(crossword_puzzle, crossword_entries, constrained_words);

    auto start_time = std::chrono::high_resolution_clock::now();

    bool generated = false;
    if (algorithm == "standard-backtracking")
    {
        generated = crossword_constructor.construct_via_backtracking(crossword_puzzle);
    }
    else if (algorithm == "mrv")
    {
        generated = crossword_constructor.construct_via_mrv(crossword_puzzle);
    }
    else
    {
        auto constraints = LCV_Heuristic::build_constraints(crossword_puzzle, crossword_entries);
        if (algorithm == "lcv")
        {
            generated = crossword_constructor.construct_via_lcv(crossword_puzzle, constraints);
        }
        else
        {
            Forward_Checking_Data checked_words(constraints, crossword_puzzle, crossword_entries, constrained_words);
            generated = crossword_constructor.construct_via_mrv_and_fc(crossword_puzzle, checked_words);
        }
    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(stop_time - start_time);
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

    if (!generated)
    {
        std::cout << "No valid solution exists for the given puzzle in combination with the given dictionary.\n";
        return 1;
    }
    else
    {
        std::cout
            << "Time to generate: "
            << duration_s.count() << '.' << duration_ms.count() << 's'
            << '\n';
    }

    return 0;
}
