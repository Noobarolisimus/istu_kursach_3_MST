#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include "mst/mst_algorithms.hpp"

using namespace std;
using namespace std::literals;
using namespace chrono;
using namespace Mst;

using Megabytes = double;


template<class Func, class... Args>
auto GetExecutionTime(Func&& func, Args... args) -> high_resolution_clock::duration {
    auto begin = high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    return end - begin;
}

double ToSeconds(high_resolution_clock::duration timespan){
    return duration_cast<nanoseconds>(timespan).count() / 1e9;
}


void MtsBenchmark(ostream& out, int repeats, 
    const vector<int>& vertexCounts, const vector<double>& connectivities)
{
    for (int vertexCount : vertexCounts){
        auto execTimeSums = vector<high_resolution_clock::duration>(connectivities.size() * 2);
        for (auto connIndex : views::iota(0, (int)connectivities.size())){
            auto conn = connectivities[connIndex];
            for (auto _ : views::iota(0, repeats)){
                auto tree = GenerateTree(vertexCount, conn);
                auto primaTime = GetExecutionTime(FindMstByPrima, tree);
                auto kruskalTime = GetExecutionTime(FindMstByKruskal, tree);
                execTimeSums[connIndex * 2] += primaTime;
                execTimeSums[connIndex * 2 + 1] += kruskalTime;
            }
        }
        out << vertexCount << "\t";
        for (auto timeSum : execTimeSums)
            out << ToSeconds(timeSum / repeats) << "\t";
        out << endl;
    }
}

int main(){
    auto file = ofstream("result.csv", ios::out | ios::binary);
    file << scientific;

    auto vertexCounts = vector{
        100, 500, 1000, 1500, 2000, 
        2500, 3000, 5000, 10000 };
    auto connectivities = vector{ 0.2, 0.9 };

    MtsBenchmark(file, 100, vertexCounts, connectivities);
}