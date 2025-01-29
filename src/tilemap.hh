#ifndef TILEMAP_HH
#define TILEMAP_HH

#include <string>

#include "utils/coordinate.hh"

namespace tilemap {

struct Info {
  Utils::Coordinate canvas_size;
  Utils::Coordinate tile_size;
  std::string texture_filename;
};

}  // namespace tilemap

#endif  // TILEMAP_HH
