#pragma once
#ifndef NQUEEN_H
#define NQUEEN_H

#include <vector> 
#include <iostream>
#include <numeric>
#include <algorithm>
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>


using std::vector, std::priority_queue, std::greater;

typedef vector<uint16_t> vui16;
typedef vector<vui16> vvui16;

class NQueen {
public:
    const uint16_t mBoardSize;
    const uint16_t mPopulationSize;
    const uint16_t mTournamentSize;
    const float mMutationProb;
    const float mCrossoverProb;
    const uint32_t mMaxIt;
    const uint32_t mMaxThreats;
    vvui16 mPopulation;
public:
    NQueen() = delete;
    NQueen(uint16_t populationSize, uint16_t boardSize, 
            uint32_t maxIt, uint16_t tournamentSize, 
            float mutationProb = 0.01, float crossoverProb = 0.9);
    // ~NQueen();
    void Solve();
    uint32_t GetBestBoardScore(vector<uint16_t>& fitnessValues) const;
    vui16 Fitness(const vvui16 &chroms) const;
    vui16 Select(const vui16& scores) const;
    void NextGenerationSelection(vvui16& parents, const vvui16& children);
    void Mutate(vui16& v) const;
    std::pair<vui16, vui16> Crossover(const vui16& v1, const vui16& v2) const;
    float RandBetween0And1() const;
    void Initialize();
};

#endif 