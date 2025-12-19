#include "mst_algorithms.hpp"
#include <algorithm>
#include <random>
#include <ranges>
#include <vector>

using namespace std;
using namespace Mst;


Mst::Tree Mst::GenerateTree(int vertexCount, double connectivity){
    static constexpr auto sequenceGen = [](int from, int to){
        return 
            views::iota(from, to)
            | views::transform([](int i)
                { return tuple(i, 0); });
    };

    auto tree = Tree(vertexCount);    
    auto rand = mt19937(random_device()());
    
    for (int i = 0; i < vertexCount; i++){
        int preshuffleCount = vertexCount - i - 1;
        if (preshuffleCount == 0) continue;
        int finalCount = preshuffleCount * connectivity;
        if (finalCount == 0) finalCount = 1;

        auto& incidents = tree[i];
        incidents.reserve(max(preshuffleCount, finalCount * 2));
        incidents.resize(preshuffleCount);
        if (i + 1 < vertexCount)
            ranges::copy(
                sequenceGen(i + 1, vertexCount),
                incidents.begin());
        ranges::shuffle(incidents | views::drop(1), rand);
        incidents.resize(finalCount);
    }

    auto sizes = 
        tree
        | views::transform([](auto& incidents)
            { return incidents.size(); })
        | ranges::to<vector>();

    for (int v1 : views::iota(0, vertexCount)){
        auto& v1Incidents = tree[v1];
        for (auto& [v2, w] : v1Incidents | views::take(sizes[v1])){
            w = (int)rand() & 0xff;
            tree[v2].push_back({v1, w});
        }
    }

    return tree;
}


TreeGeneratorMultiThreaded::TreeGeneratorMultiThreaded(int treeCount, int threadPoolCount){
    this->treeCount = treeCount;
    if (threadPoolCount <= 0){
        threadPoolCount = thread::hardware_concurrency();
        if (threadPoolCount == 0)
            threadPoolCount = 1;
    }
    threads.resize(threadPoolCount);
}

vector<Tree> TreeGeneratorMultiThreaded::operator() (int vertexCount, double connectivity){
    Context context{ .i = treeCount };
    context.result.reserve(treeCount);

    for (auto& thread : threads){
        thread = ::thread(&TreeGeneratorMultiThreaded::Job, 
            ref(context), vertexCount, connectivity);
    }

    for (auto& t : threads) t.join();
    return std::move(context.result);
}

void TreeGeneratorMultiThreaded::Job(Context& context, int vertexCount, double connectivity){
    while (true){
        {
            auto _ = lock_guard(context.mutex);
            if (context.i == 0) return;
            context.i--;
        }
        Tree tree = GenerateTree(vertexCount, connectivity);
        {
            auto _ = lock_guard(context.mutex);
            context.result.push_back(std::move(tree));
        }
    }
}