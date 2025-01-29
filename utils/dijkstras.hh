#ifndef UTILS_DIJKSTRAS_HH
#define UTILS_DIJKSTRAS_HH

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "default_map.hh"

namespace Utils {

//
// WeightedEdge defines a helper type that associates a given cost/distance
// value to a given graph edge.
//
// Note that the less-than comparison operator is reversed and returns a
// less-than comparison for the "other" value's point of view. This ensures that
// priority-queue ordering yields the shortest (i.e. least cost) results first.
//
template <typename DISTANCE, typename EDGE>
  requires std::is_integral_v<DISTANCE> or std::is_floating_point_v<DISTANCE>
struct WeightedEdge {
  DISTANCE distance;
  EDGE edge;

  [[nodiscard]] constexpr auto operator<(const WeightedEdge& other) const
      -> bool {
    // NOTE(AE): REVERSE ORDERING - MIN ELEMENT FIRST
    return other.distance < distance;
  }
};

//
// dijkstra() provides a generic implementation of Dijkstra's path finding
// algorithm.
//
// Details:
//   https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
//
// This algorithm calculates the lowest cost (distance) to any given point in a
// graph reachable through adjacent edges.
//
// The |adjacent| parameter specifies a callable operator or function that
// provides an iterable range of edges to traverse at any given point in the
// graph.
//
// This function returns a map of the calculated distance (read "cost") values
// accumulated for every node in the graph, as well as a list of edges for each
// node that points to the previous node in the path to the starting point.
//
// If a given edge is not present in either the distances or previous maps, the
// starting point cannot be reached from that graph lcoation.
//
template <typename DISTANCE, typename EDGE>
  requires std::is_integral_v<DISTANCE> or std::is_floating_point_v<DISTANCE>
struct Dijkstra {
  using DistanceMap = default_map<EDGE, DISTANCE>;
  using PathMap     = std::unordered_map<EDGE, std::unordered_set<EDGE>>;
  using Edge        = WeightedEdge<DISTANCE, EDGE>;

  [[nodiscard]] static constexpr auto find(Edge start, auto&& adjacent)
      -> std::pair<DistanceMap, PathMap> {
    auto distances = DistanceMap{};
    auto previous  = PathMap{};

    auto queue = std::priority_queue<Edge>{};
    queue.push(start);

    while (!queue.empty()) {
      const auto [distance, current] = queue.top();
      queue.pop();

      for (const auto [distance_to, other] : adjacent(current)) {
        if (distance + distance_to < distances.at_or_max(other)) {
          distances[other] = distance + distance_to;
          queue.push({distances[other], other});
        }
        if (distance + distance_to <= distances.at_or_max(other))
          previous[other].insert(current);
      }
    }

    return std::make_pair(distances, previous);
  }
};

}  // namespace Utils

#endif  // UTILS_DIJKSTRAS_HH
