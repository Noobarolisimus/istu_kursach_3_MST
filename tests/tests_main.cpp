#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <array>
#include <ranges>
#include "utils/color_terminal.hpp"
#include "mst/mst_algorithms.hpp"

using namespace Mst;

#define WHERE (__FILE__":" + std::to_string(__LINE__))
#define ASSERT(boolStatement) _Assert(boolStatement, std::string(WHERE), #boolStatement)
#define ASSERT_MSG(boolStatement, msg) _Assert(boolStatement, std::string(WHERE), #boolStatement + std::string(".\n") + msg)
#define RUN(func) { std::cout << #func << " is running...\n"; func ; std::cout << "  Done.\n"; }

[[maybe_unused]]
inline void _Assert(bool statement, const std::string& where, const std::string& onFailure = "") {
    if (!statement) {
        std::cout << TermColor::FG_LRED << where << ": " << TermColor::FG_LBLUE 
            << "Assertion failure: " << onFailure << TermColor::DEFAULT << "\n";
        throw std::runtime_error(onFailure);
    }
    return;
}


int64_t GetWeightSum(const Tree& tree){
    int64_t sum = 0;
    for (auto inc : tree){
        for (auto [_, w] : inc){
            sum += w;
        }
    }
    sum /= 2;
    return sum;
}

// Возвращает отклонение в связности по модулю.
double AssertTree(const Tree& tree, int vertexCount,
    double targetConnectivity, double allowedConnectivityError)
{
    auto edgeCount = 0;
    for (auto& inc : tree) edgeCount += inc.size();

    auto treeConnectivity = (double)edgeCount / vertexCount / vertexCount;
    auto connError = abs(treeConnectivity - targetConnectivity);
    ASSERT(connError <= allowedConnectivityError);
    return connError;
}

void PrimaTest(){
    Tree tree = {
        {{1, 7}, {2, 8}},
        {{0, 7}, {2, 11}, {3, 2}},
        {{0, 8}, {1, 11}, {3, 6}, {4, 9}},
        {{1, 2}, {2, 6}, {4, 11}, {5, 9}},
        {{2, 9}, {3, 11}, {5, 10}},
        {{3, 9}, {4, 10}},
    };

    int sum = GetWeightSum(FindMstByPrima(tree));
    ASSERT(sum == 33);
}

void KruskalTest(){
    Tree tree = {
        {{1, 7}, {2, 8}},
        {{0, 7}, {2, 11}, {3, 2}},
        {{0, 8}, {1, 11}, {3, 6}, {4, 9}},
        {{1, 2}, {2, 6}, {4, 11}, {5, 9}},
        {{2, 9}, {3, 11}, {5, 10}},
        {{3, 9}, {4, 10}},
    };

    int sum = GetWeightSum(FindMstByKruskal(tree));
    ASSERT(sum == 33);
}

void GenerateTreeTest(){
    static constexpr std::array<int, 3> vs = {100, 1000, 2500};
    static constexpr std::array<double, 4> cs = {0.1, 0.35, 0.6, 0.9};
    static constexpr double allowedConnectivityError = 0.05;

    for (auto v : vs){
        for (auto c : cs){
            auto tree = GenerateTree(v, c);
            auto connError = AssertTree(tree, v, c, allowedConnectivityError);
            std::cout << "  {" << v << ", " << c << "} is passed (connError: "
                << connError << ")\n";
        }
    }
}

void TreeGeneratorMultiThreadedTest(){
    static constexpr std::array<int, 2> vs = { 100, 1500 };
    static constexpr std::array<double, 2> cs = { 0.35, 0.9 };
    static constexpr double allowedConnectivityError = 0.05;

    auto gen = TreeGeneratorMultiThreaded(100);

    for (auto v : vs) {
        for (auto c : cs) {
            auto trees = gen(v, c);
            auto connErrorSum = 0.;
            for (auto& t : trees) {
                connErrorSum += AssertTree(t, v, c, allowedConnectivityError);
            }
            std::cout << "  {" << v << ", " << c << "}*100 is passed (avr connError: "
                << connErrorSum / 100 << ")\n";
        }
    }

}

void RandomPrimaAndKruskalTest(){
    constexpr int interations = 20;
    for (int i = 0; i < interations; i++){
        // Примерно от 40 до 100.
        int vertexCount = 40 + (1 << i * 6 / interations);
        // Примерно от 0.1 до 0.85.
        double connectivity = 0.1 + (i % 10 / 12.);
        auto tree = GenerateTree(vertexCount, connectivity);

        Tree prima = FindMstByPrima(tree);
        Tree kruskal = FindMstByKruskal(tree);

        auto primaSum = GetWeightSum(prima);
        auto kruskalSum = GetWeightSum(kruskal);
        ASSERT(primaSum == kruskalSum);
    }
}


int main(){
    RUN(PrimaTest());
    RUN(KruskalTest());
    RUN(GenerateTreeTest());
    RUN(TreeGeneratorMultiThreadedTest());
    RUN(RandomPrimaAndKruskalTest());
    
    std::cout << TermColor::FG_LGREEN << "All tests are passed.\n" << TermColor::DEFAULT;
}