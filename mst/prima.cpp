#include "mst_algorithms.hpp"
#include <ranges>
#include <vector>

using namespace Mst;
using namespace std;
namespace views = std::views;
namespace ranges = std::ranges;

inline namespace {
    struct EdgeCompByWeight {
        bool operator() (const Mst::Edge& lhs, const Mst::Edge& rhs) const {
            return std::tuple(lhs.weight, lhs.from, lhs.to) 
                < std::tuple(rhs.weight, rhs.from, rhs.to);
        }
    };
}


Mst::Tree Mst::FindMstByPrima(const Mst::Tree& baseTree){
    if (baseTree.empty()) return baseTree;

    Tree resultTree(baseTree.size());
    set<int> includedIds{0};

    auto pq = multiset<Edge, EdgeCompByWeight>();
    auto used = vector<bool>(baseTree.size(), false);

    pq.insert({0, 0, 0});

    while (!pq.empty()) {
        Edge e = *pq.begin();
        pq.erase(pq.begin());

        if (used[e.to]) continue;
        used[e.to] = true;
        if (e.from != e.to)
            resultTree[e.from].push_back({e.to, e.weight});

        for (auto [v, w] : baseTree[e.to]) {
            if (!used[v])
                pq.insert({e.to, v, w});
        }
    }

    auto sizes = 
        resultTree
        | views::transform([](auto& incidents)
            { return incidents.size(); })
        | ranges::to<vector>();

    for (int v1 : views::iota(0, (int)resultTree.size())){
        auto& v1Incidents = resultTree[v1];
        for (auto& [v2, w] : v1Incidents | views::take(sizes[v1])){
            resultTree[v2].push_back({v1, w});
        }
    }

    return resultTree;
}