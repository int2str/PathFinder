#ifndef TILEMAP_HH
#define TILEMAP_HH

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "utils/coordinate.hh"
#include "utils/grid.hh"

namespace tilemap {

//
// Internal representation of a tilemap layer
//
using Grid = Utils::Grid<Utils::Coordinate>;

//
// Info extracted from JSON map file
//
struct Info {
  Utils::Coordinate canvas_size;
  Utils::Coordinate tile_size;
  std::string texture_filename;
};

//
// RouteMapping ties unit tile entities to their respective target tiles
//
struct RouteMapping {
  Utils::Coordinate unit_tile;
  Utils::Coordinate target_tile;
};

//
// fromJson() attempts to parse a RiskyLab compatible JSON file containing a
// tilemap, and optionally returns a Utils::Grid<> containing the layers[0].data
// values.
//
// Note that this is currently hard-coded to consider layer 0 only.
//
[[nodiscard]] auto gridFromJson(std::string_view json_text)
    -> std::optional<std::pair<Info, Grid>>;

}  // namespace tilemap

#endif  // TILEMAP_HH
