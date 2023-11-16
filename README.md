# Crossword-Generator
This project uses standard backtracking and heuristics such as least constraining value and minimum remaining value to generate a crossword puzzle solution given a crossword puzzle frame.

# Requirements
CMake version 3.22+  
GNU g++ version 11.4.0+  

# Getting Started
Run the following commands:  
```
git clone https://github.com/tr29038/Crossword-Generator.git
cd ./Crossword-Generator
```

# Building
Run the following commands:  
```
cd ./build
./build.sh
make
```

# Running
Run the following command:  
```
./crossword_generator ../puzzles/puzzle1 mrv
```

For a list of options, run the command:  
```
./crossword_generator
```

# Creating a Custom Puzzle
To run the executable with a custom puzzle, follow the below steps:
1. Create a directory and name it anything you like.
2. Add a `dictionary.txt` file to the directory.
3. Populate `dictionary.txt` with all eligible words (either consistently in lowercase or uppercase), where each word is delimited by a newline.
4. Add a `puzzle.txt` file to the directory.
5. Populate `puzzle.txt` with the puzzle structure. Mark all word entry starting positions with its corresponding number like so: `[#]`.
