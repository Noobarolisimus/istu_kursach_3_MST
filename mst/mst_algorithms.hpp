#include "types.hpp"
#include <vector>
#include <mutex>

namespace Mst {
    
Tree FindMstByKruskal(const Mst::Tree& baseTree);
Tree FindMstByPrima(const Mst::Tree& baseTree);

// `connectivity` - процент связности графа, от 0 до 1.
Tree GenerateTree(int vertexCount, double connectivity);

class TreeGeneratorMultiThreaded {
public:
    TreeGeneratorMultiThreaded(int treeCount, int threadPoolCount = -1);    
    std::vector<Tree> operator() (int vertexCount, double connectivity);

private:
    std::vector<std::thread> threads;
    int treeCount;
    
    struct Context{
        std::mutex mutex;
        std::vector<Tree> result;
        int i;
    };

    static void Job(Context& context, int vertexCount, double connectivity);

};
} // namespace Mst.