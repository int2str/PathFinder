#include "src/tilemap.hh"

#include <optional>
#include <string_view>
#include <utility>

#include "json/json.hh"
#include "tilemap_internal.hh"

namespace tilemap::internal {

//
// mapInfoFromJson() parses a RiskyLap Online TileMap Editor compatible JSON
// file and optionally returns a MapInfo object if all values were parsed
// successfully.
//
// NOTE(AE) - This function assumes (BEWARE!) that tilesets[0] has the correct
// tile widths and heights for the relevant layer and does not attempt to match
// tileset filenames etc.
//
[[nodiscard]] constexpr auto mapInfoFromJson(const json::Json& json)
    -> std::optional<Info> {
  auto map_info = Info{};

  // Tile size
  map_info.tile_size.x =
      static_cast<int>(json["tilesets"][0]["tilewidth"].number().value_or(0));
  map_info.tile_size.y =
      static_cast<int>(json["tilesets"][0]["tileheight"].number().value_or(0));
  if (map_info.tile_size.x == 0 or map_info.tile_size.y == 0)
    return std::nullopt;

  // Canvas size
  map_info.canvas_size.x =
      static_cast<int>(json["canvas"]["width"].number().value_or(0));
  map_info.canvas_size.y =
      static_cast<int>(json["canvas"]["height"].number().value_or(0));
  if (map_info.canvas_size.x == 0 or map_info.canvas_size.y == 0)
    return std::nullopt;

  // Tileset filename
  const auto maybe_filename = json["layers"][0]["tileset"].string();
  if (!maybe_filename) return std::nullopt;
  map_info.texture_filename = *maybe_filename;

  return map_info;
}

}  // namespace tilemap::internal

namespace tilemap {

//
// fromJson() attempts to parse a RiskyLab compatible JSON file containing a
// tilemap, and optionally returns a Utils::Grid<> containing the layers[0].data
// values.
//
// NOTE(AE) - This function is currently hard-coded to consider layer 0 only.
//
[[nodiscard]] auto gridFromJson(std::string_view json_text)
    -> std::optional<std::pair<Info, Grid>> {
  const auto maybe_json = json::Json::parse(json_text);
  if (!maybe_json) return std::nullopt;

  const auto maybe_map_info = internal::mapInfoFromJson(*maybe_json);
  if (!maybe_map_info) return std::nullopt;

  const auto& map_info = *maybe_map_info;
  const auto tiles =
      internal::coordinatesFromJsonData((*maybe_json)["layers"][0]["data"]);

  // Ensure the correct number of values was present in the .data
  const auto tiles_per = map_info.canvas_size / map_info.tile_size;
  const auto tile_count =
      static_cast<size_t>(tiles_per.x) * static_cast<size_t>(tiles_per.y);
  if (tiles.size() != tile_count) return std::nullopt;

  // Finally, convert to a Utils::Grid
  return std::make_pair(map_info,
                        Grid{static_cast<size_t>(tiles_per.x), tiles});
}

}  // namespace tilemap
