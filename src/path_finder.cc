#include "path_finder.hh"

#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "src/tilemap.hh"
#include "src/tilemap_woodland.hh"
#include "utils/coordinate.hh"
#include "utils/dijkstras.hh"

namespace path_finder {

//
// findPath() returns a path from any grid coordinate that can reach the
// specified target.
//
[[nodiscard]] auto findPath(const tilemap::Grid& grid, Utils::Coordinate target)
    -> std::unordered_map<Utils::Coordinate,
                          std::unordered_set<Utils::Coordinate>> {
  // adjacent() returns all non-elevated orthogonal neighbors of the given
  // coordinate position. The distance to all orthogonal neighobors is set to 1.
  const auto adjacent = [&](const auto& from) {
    return from.neighborsUpDownLeftRight()  //
           | std::views::filter([&](auto pos) {
               return grid.inBounds(pos) and
                      grid[pos] != tilemap::woodland::FORREST;
             })  //
           | std::views::transform(
                 [](auto pos) { return Dijkstra::Edge{1, pos}; })  //
           | std::ranges::to<std::vector>();
  };

  // The Dijkstra's path finding algorithm returns a pair of distances for each
  // graph node as well as the path to the target from each node. Since we don't
  // need the cost, the first return value is ignored.
  const auto& [_, previous] = Dijkstra::find({0, target}, adjacent);
  return previous;
}

//
// tracePath() returns a path for a given unit (if it can reach its target) or
// an empty vector if the unnit cannot.
//
[[nodiscard]] auto tracePath(const Dijkstra::PathMap& previous,
                             Utils::Coordinate unit, Utils::Coordinate target)
    -> std::vector<Utils::Coordinate> {
  auto path = std::vector<Utils::Coordinate>{unit};
  while (previous.contains(unit) and unit != target) {
    unit = *previous.at(unit).begin();
    path.push_back(unit);
  }
  return path;
}

//
// unitPaths() returns a path for each unit that can reach its matching target
//
[[nodiscard]] auto unitPaths(const tilemap::Grid& grid)
    -> std::unordered_map<Utils::Coordinate, std::vector<Utils::Coordinate>> {
  auto routes =
      std::unordered_map<Utils::Coordinate, std::vector<Utils::Coordinate>>{};
  for (const auto& route : tilemap::woodland::UNIT_TARGETS) {
    const auto maybe_target = grid.find(route.target_tile);
    if (!maybe_target) continue;

    const auto previous = findPath(grid, *maybe_target);

    for (const auto& unit_start : grid.findAll(route.unit_tile)) {
      if (!previous.contains(unit_start)) continue;
      routes[unit_start] = tracePath(previous, unit_start, *maybe_target);
    }
  }
  return routes;
}

}  // namespace path_finder
