#include <set>
#include <vector>


namespace Mst {

struct Edge {
    int from;
    int to;
    int weight;

    bool operator== (const Edge& rhs) const {
        return from == rhs.from && to == rhs.to && weight == rhs.weight;
    }
};

struct EdgeHash {
    size_t operator() (const Mst::Edge& edge) const {
        return (size_t)edge.from << 32 | (size_t)edge.weight << 24 | edge.to;
    }
};

struct EdgeComp {
    bool operator() (const Mst::Edge& lhs, const Mst::Edge& rhs) const {
        return std::tuple(lhs.from, lhs.to, lhs.weight) 
            < std::tuple(rhs.from, rhs.to, rhs.weight);
    }
};

using Tree = std::vector<std::vector<std::tuple<int, int>>>;

} // namespace Mst.
