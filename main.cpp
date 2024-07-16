#include "NQueen.hpp"

void print1(vector<vector<uint16_t>> u) {
    for (auto sample : u) {
        for (size_t j = 0; j < sample.size(); j++) {
            std::cout << sample[j];
        }
        std::cout << std::endl;
    }    
}

void print2(vector<uint16_t> u) {
    auto i = 0;
    for (auto sample : u) {
        std::cout << i << ": " << sample << ' ';
    }    
    std::cout << std::endl;
}

void print_best_board(vector<uint16_t>& fitness_values, vector<vector<uint16_t>>& boards) {
    int max = -1, idx = -1;
    for (size_t i = 0; i < fitness_values.size(); i++) {
        if (fitness_values[i] > max) {
            max = fitness_values[i];
            idx = i;
        }
    }
    std::cout << "best score: " << max << "\n";
    auto& bb = boards[idx];
    for (int i = 0; i < bb.size(); i++) {
        for (int j = 0; j < bb.size(); j++) {
            if (j == bb[i] - 1)
                std::cout << bb[i] << " ";
            else
                std::cout << "# ";
        }
        std::cout << std::endl;
    }
}

void print_worst_board(vector<uint16_t>& fitness_values, vector<vector<uint16_t>>& boards) {
    int min = 10000, idx = -1;
    for (size_t i = 0; i < fitness_values.size(); i++) {
        if (fitness_values[i] < min) {
            min = fitness_values[i];
            idx = i;
        }
    }
    std::cout << "worst score: " << min << "\n";
    auto& bb = boards[idx];
    for (int i = 0; i < bb.size(); i++) {
        for (int j = 0; j < bb.size(); j++) {
            if (j == bb[i] - 1)
                std::cout << bb[i] << " ";
            else
                std::cout << "# ";
        }
        std::cout << std::endl;
    }
}

int main2()
{
    const uint16_t populationSize = 8; 
    const uint16_t boardSize = 8; 
    const uint32_t maxIt = 128; 
    const uint16_t tournamentSize = 4; 
    const uint16_t mutationProb = 0.01, crossoverProb = 0.9;
    NQueen nqueen = NQueen(populationSize, boardSize, maxIt, tournamentSize, mutationProb);
    nqueen.Initialize();
    print1(nqueen.mPopulation);
    auto fitness_values = nqueen.Fitness(nqueen.mPopulation);
    print2(fitness_values);
    std::cout << "===========best board============\n";
    print_best_board(fitness_values, nqueen.mPopulation);
    std::cout << "===========worst board============\n";
    print_worst_board(fitness_values, nqueen.mPopulation);
    std::cout << "============Selection=============\n";
    auto selected = nqueen.Select(fitness_values);
    print2(selected);
    vvui16 offsprings;
    offsprings.reserve(nqueen.mPopulationSize);
    std::cout << "=============Crossover============\n";
    auto v1 = nqueen.mPopulation[0];
    auto v2 = nqueen.mPopulation[1];
    std::cout << "parent1: " << "\n";
    print2(v1);
    std::cout << "parent2: " << "\n";
    print2(v2);
    auto children = nqueen.Crossover(v1, v2);
    std::cout << "child1: " << "\n";
    print2(children.first);
    std::cout << "child2: " << "\n";
    print2(children.second);
    offsprings.push_back(children.first);
    offsprings.push_back(children.second);
    std::cout << "=============Mutation=============\n";
    for (auto offspring : offsprings) {
        nqueen.Mutate(offspring);
    }
    auto to_mutate = nqueen.mPopulation[0];
    std::cout << "before mutation:\n";
    print2(to_mutate);
    nqueen.Mutate(to_mutate);
    std::cout << "after mutation:\n";
    print2(to_mutate);
    std::cout << "====Next Generation Selection=====\n";
    print1(nqueen.mPopulation);
    print1(offsprings);
    nqueen.NextGenerationSelection(offsprings, nqueen.mPopulation);
    print1(nqueen.mPopulation);
    return 0;
}

int main() {
    const uint16_t populationSize = 64; 
    const uint16_t boardSize = 16; 
    const uint32_t maxIt = 256; 
    const uint16_t tournamentSize = 8; 
    const float mutationProb = 0.2, crossoverProb = 0.80;
    NQueen nqueen = NQueen(populationSize, boardSize, maxIt, tournamentSize, mutationProb, crossoverProb);
    nqueen.Initialize();
    nqueen.Solve();
}