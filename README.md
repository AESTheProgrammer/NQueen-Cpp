# NQueen-Cpp
Implementation of evolutionary algorithm for solving N queen problem using C++.
# Usage
1. Windows

    g++ main.cpp NQueen.cpp -o main.exe; ./main.exe
2. Linux

    g++ main.cpp NQueen.cpp -o main.out; ./main.out
# Steps
1. Initial Population: Random
2. Selection: Tournament
3. Fitness: Maximum number of threats - number of threats
4. Crossover: Partially mapped crossover (PMX)
5. Mutate: Swap
6. Select next generatino: From both parents and children
7. Repeat 2 to 6 until either problem is solved (no queen is threating the other) or the maximum number of iterations is reached.
