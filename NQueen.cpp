// https://stackoverflow.com/questions/15704565/efficient-way-to-return-a-stdvector-in-c#:~:text=In%20C%2B%2B11%2C%20this,be%20elided%20by%20the%20compiler.
// https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
// https://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c
// https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const
// https://cdecl.plus/?q=int%20const%20%2A%20const%20%2A%2A%20const%20%2A%20const
// https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
// https://stackoverflow.com/questions/686353/random-float-number-generation
// https://stackoverflow.com/questions/10474417/how-to-delete-the-default-constructor
// https://en.cppreference.com/w/cpp/algorithm/swap
// https://www.nexthink.com/blog/c17-function
// https://en.cppreference.com/w/cpp/container/priority_queue/priority_queue

// TODO
// How do random number generators work?
// make functions private
// crossover can be made faster using enumeration
// declare type for repeated types (e.g., vvui16 or vector<uin16_t>)
// random numbers are not unique in Selection

#include "NQueen.hpp"

using std::iota;

NQueen::NQueen(uint16_t populationSize, uint16_t boardSize, 
                uint32_t maxIt, uint16_t tournamentSize, 
                float mutationProb, float crossoverProb): 
    mBoardSize(boardSize),
    mPopulationSize(populationSize),
    mMaxIt(maxIt),
    mMaxThreats(boardSize*(boardSize-1)/2),
    mTournamentSize(tournamentSize),
    mMutationProb(mutationProb),
    mCrossoverProb(crossoverProb),
    mPopulation(populationSize, vui16(boardSize, 0)) 
{
    srand (static_cast <unsigned> (time(0)));
}

// NQueen::~NQueen() {}

void NQueen::Solve() 
{
    auto bestBoardScore = 0;
    for (auto i = 0; i < mMaxIt && bestBoardScore < mMaxThreats; i++) {
        vvui16 children;
        children.reserve(mPopulationSize);
        auto fitnessValues = Fitness(mPopulation);
        bestBoardScore = GetBestBoardScore(fitnessValues);
        auto selected = Select(fitnessValues);
        for (size_t j = 0; j < mPopulationSize * 2; j+=2) {
            auto offsprings = Crossover(mPopulation[selected[j]], mPopulation[selected[j+1]]);
            Mutate(offsprings.first);
            Mutate(offsprings.second);
            children.push_back(offsprings.first);
            children.push_back(offsprings.second);
        }
        NextGenerationSelection(mPopulation, children);
    }
    auto fitnessValues = Fitness(mPopulation);
    bestBoardScore = GetBestBoardScore(fitnessValues);
}

uint32_t NQueen::GetBestBoardScore(vector<uint16_t>& fitnessValues) const {
    uint32_t max = 0;
    int idx = -1;
    for (size_t i = 0; i < fitnessValues.size(); i++) {
        if (fitnessValues[i] > max) {
            max = fitnessValues[i];
            idx = i;
        }
    }
    std::cout << "best score: " << max << "\n";
    auto& bb = mPopulation[idx];
    for (int i = 0; i < bb.size(); i++) {
        for (int j = 0; j < bb.size(); j++) {
            if (j == bb[i] - 1)
                printf("%02d ", bb[i]);
                // std::cout << bb[i] << " ";
            else
                std::cout << "#  ";
        }
        std::cout << std::endl;
    }
    return max;
}

std::pair<vui16, vui16> NQueen::Crossover(vui16 const& chrom1, vui16 const& chrom2) const 
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, chrom1.size()-1);
    vui16 child1(chrom2);
    vui16 child2(chrom1);
    if (mCrossoverProb > RandBetween0And1()) {
        uint16_t point1 = dist(rng);
        uint16_t point2 = dist(rng);
        if (point1 > point2) 
            std::swap(point1, point2);
        point2 += 1; // to make sure point2 and point1 are not equal
        std::copy(chrom1.begin() + point1, chrom1.begin() + point2, child1.begin() + point1);
        std::copy(chrom2.begin() + point1, chrom2.begin() + point2, child2.begin() + point1);
        // lost DNAs from chrom1 
        // should replace duplicate DNAs with the ones we lost
        for (size_t i = point1; i < point2; i++) {
            auto index = std::find(chrom2.begin(), chrom2.end(), chrom1[i]) - chrom2.begin();
            if (index < point2 && index >= point1)
                continue;
            uint16_t dna = chrom2[i]; 
            uint16_t j = point1; 
            do {
                j = std::find(chrom1.begin(), chrom1.end(), dna) - chrom1.begin();
                dna = chrom2[j];
            } while (j < point2 && j >= point1);
            child2[j] = chrom1[i];
        }
        for (size_t i = point1; i < point2; i++) {
            auto index = std::find(chrom1.begin(), chrom1.end(), chrom2[i]) - chrom1.begin();
            if (index < point2 && index >= point1)
                continue;
            uint16_t dna = chrom1[i]; 
            uint16_t j = point1; 
            do {
                j = std::find(chrom2.begin(), chrom2.end(), dna) - chrom2.begin();
                dna = chrom1[j];
            } while (j < point2 && j >= point1);
            child1[j] = chrom2[i];
        }
    }
    return std::make_pair(child1, child2);
}

float NQueen::RandBetween0And1() const 
{
    return static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
}

void NQueen::Mutate(vui16& chrom) const 
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, chrom.size()-1);
    if (RandBetween0And1() < mMutationProb) {
        uint16_t DNA1 = dist(rng);
        uint16_t DNA2 = dist(rng);
        std::swap(chrom[DNA1], chrom[DNA2]);       
    }
}

void NQueen::NextGenerationSelection(vvui16& parents, const vvui16& children) 
{
    auto parentsScore = Fitness(parents);
    auto childrenScore = Fitness(children);
    // childrenScore.insert(childrenScore.end(), parentsScore.begin(), parentsScore.end());
   std::vector<std::pair<uint16_t, size_t>> indexedVec; // pair<value, original_index>
    for (size_t i = 0; i < parentsScore.size(); ++i) {
        indexedVec.emplace_back(parentsScore[i], i);
    }
    for (size_t i = 0; i < childrenScore.size(); ++i) {
        indexedVec.emplace_back(childrenScore[i], parentsScore.size() + i);
    }
    // what just happend here?
    using Element = std::pair<uint16_t, size_t>;
    auto cmp = [](const Element& e1, const Element& e2) {
        return e1.first > e2.first;
    };
    std::priority_queue<Element, vector<Element>, decltype(cmp)> minHeap(cmp);
    for (const auto& elem : indexedVec) {
        if (minHeap.size() < mPopulationSize) {
            minHeap.push(elem);
        } else if (elem.first > minHeap.top().first) {
            minHeap.pop();
            minHeap.push(elem);
        }
    }
    vvui16 nextGen;
    nextGen.reserve(minHeap.size());
    while (!minHeap.empty()) {
        // std::cout << minHeap.top().first << " " << minHeap.top().second << std::endl;
        auto& idx = minHeap.top().second;
        if (idx < parentsScore.size())
            nextGen.push_back(parents[idx]);
        else
            nextGen.push_back(children[idx-parentsScore.size()]);
        minHeap.pop();
    }
    // parents = std::move(nextGen);
    mPopulation = std::move(nextGen);
}

// champion selection 
vui16 NQueen::Select(const vui16& scores) const
{
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<int> dist {0, mPopulationSize - 1};
    auto gen = [&](){
                   return dist(mersenne_engine);
               };
    // contestant chromosomes
    vui16 contestants(mTournamentSize);
    vui16 winners(mPopulationSize * 2);
    for (int i = 0; i < mPopulationSize * 2; i++) {
        std::generate(contestants.begin(), contestants.end(), gen);
        int winner = contestants[0];
        for (auto contestant : contestants) {
            // std::cout << "(" << contestant << ", " << scores[contestant] << ")" << " ";
            if (scores[contestant] >= winner) {
                winner = contestant;
            }
        } 
        // std::cout << std::endl;
        winners[i] = winner;
    }
    return winners;
}


// maximum score - number of threats (for each sample)
vui16 NQueen::Fitness(const vvui16& chroms) const
{
    vui16 scores;
    scores.reserve(chroms.size());
    for (auto sample : chroms) {
        uint16_t threats = 0;
        for (size_t i = 0; i < sample.size(); i++) {
            for (size_t j = i+1; j < sample.size(); j++) {
                if (j - i == abs(sample[i] - sample[j])) {
                    threats++;
                }
            }
        }
        scores.emplace_back(mMaxThreats - threats);
    }
    return scores;
}

// https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
void NQueen::Initialize() 
{
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    for (auto& sample : mPopulation) {
        iota(sample.begin(), sample.end(), 1);
        std::shuffle(std::begin(sample), std::end(sample), rng);
    }
}
