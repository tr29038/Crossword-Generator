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
