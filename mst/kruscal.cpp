#include "mst_algorithms.hpp"
#include <algorithm>
#include <ranges>
#include <vector>

using namespace Mst;
using namespace std;
namespace views = std::views;
namespace ranges = std::ranges;

namespace {

class Dsu {
public:
    Dsu(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 1;
        }
    }

    int Find(int i) {
        return (parent[i] == i) ? i : (parent[i] = Find(parent[i]));
    }

    void Unite(int x, int y) {
        int s1 = Find(x), s2 = Find(y);
        if (s1 != s2) {
            if (rank[s1] < rank[s2]) parent[s1] = s2;
            else if (rank[s1] > rank[s2]) parent[s2] = s1;
            else parent[s2] = s1, rank[s1]++;
        }
    }

private:
    vector<int> parent, rank;
};

} // namespace.


Mst::Tree Mst::FindMstByKruskal(const Mst::Tree& baseTree){
    if (baseTree.empty()) return baseTree;
    Tree resultTree(baseTree.size());

    vector<Edge> sortedEdges;
    sortedEdges.reserve(baseTree.size() * baseTree.size());
    for (auto v1 : views::iota(0, (int)baseTree.size())){
        for (auto [v2, w] : baseTree[v1]){
            sortedEdges.emplace_back(v1, v2, w);
        }
    }
    ranges::sort(sortedEdges, {}, &Mst::Edge::weight);

    Dsu dsu(baseTree.size());
    int addedCount = 0;
    
    for (auto &e : sortedEdges) {
        if (dsu.Find(e.from) == dsu.Find(e.to)) continue;
        dsu.Unite(e.from, e.to);
        resultTree[e.from].push_back({e.to, e.weight});
        if (++addedCount == baseTree.size() - 1) break;
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