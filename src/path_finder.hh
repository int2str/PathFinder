#ifndef PATH_FINDER_HH
#define PATH_FINDER_HH

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "src/tilemap.hh"
#include "utils/coordinate.hh"
#include "utils/dijkstras.hh"

namespace path_finder {

using Dijkstra = Utils::Dijkstra<int, Utils::Coordinate>;

//
// findPath() returns a path from any grid coordinate that can reach the
// specified target.
//
[[nodiscard]] auto findPath(const tilemap::Grid& grid, Utils::Coordinate target)
    -> std::unordered_map<Utils::Coordinate,
                          std::unordered_set<Utils::Coordinate>>;

//
// tracePath() returns a path for a given unit (if it can reach its target) or
// an empty vector if the unit cannot.
//
[[nodiscard]] auto tracePath(const Dijkstra::PathMap& previous,
                             Utils::Coordinate unit, Utils::Coordinate target)
    -> std::vector<Utils::Coordinate>;

//
// unitPaths() returns a path for each unit that can reach its matching target
//
[[nodiscard]] auto unitPaths(const tilemap::Grid& grid)
    -> std::unordered_map<Utils::Coordinate, std::vector<Utils::Coordinate>>;

}  // namespace path_finder

#endif  // PATH_FINDER_HH
