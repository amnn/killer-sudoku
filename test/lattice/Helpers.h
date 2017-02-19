#include <lattice/Node.h>

#include <vector>

namespace lattice {
namespace helpers {

using namespace detail;

std::vector< std::vector<uint32_t> > ivals(Node *);

std::vector<uint32_t> rvals(Node *);
std::vector<uint32_t> cvals(Node *);

} // namespace helpers
} // namespace lattice
