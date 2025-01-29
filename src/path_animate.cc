#include <fmt/core.h>

#include <filesystem>
#include <string>

#include "path_finder.hh"
#include "src/tilemap.hh"
#include "src/tilemap_woodland.hh"
#include "src/window.hh"
#include "utils/coordinate.hh"
#include "utils/coordinate_formatter.hh"  // IWYU pragma: keep
#include "utils/one_of.hh"
#include "utils/read_file.hh"

namespace {

//
// canMoveTo() checks if a given tile coodinate can be occupied by a unit.
//
[[nodiscard]] constexpr auto canMoveTo(const tilemap::Grid& grid,
                                       Utils::Coordinate to) -> bool {
  return grid[to] == one_of(Utils::Coordinate{}, tilemap::woodland::TARGET_BLUE,
                            tilemap::woodland::TARGET_GREEN,
                            tilemap::woodland::TARGET_PURPLE,
                            tilemap::woodland::TARGET_RED);
}

//
// animate() shows the units travelling to their respective targets.
//
void animate(const tilemap::Info& map_info, const tilemap::Grid& map) {
  auto unit_paths = path_finder::unitPaths(map);
  auto grid       = map;

  auto paused = false;
  auto window = path_finder::Window(map_info);

  while (window.isOpen()) {
    if (auto event = window.handleEvents()) {
      if (event == path_finder::Event::Reset) {
        grid       = map;
        unit_paths = path_finder::unitPaths(map);

      } else if (event == path_finder::Event::PauseResume) {
        paused = !paused;
      }
    }
    window.draw(grid);

    if (paused) continue;

    for (auto& [unit, path] : unit_paths) {
      if (path.empty()) continue;

      const auto from = path.front();
      if (path.size() == 1) {
        grid[from] = map[from];  // Restore target tile
        path.clear();
        continue;
      }

      const auto to = *std::next(path.begin());
      if (canMoveTo(grid, to)) {
        grid[to] = std::exchange(grid[from], {});
        path.erase(path.begin());  // Pop front (inefficient)
      }
    }
  }
}

}  // namespace

auto main(int argc, char* argv[]) -> int {
  const auto args = std::span{argv, static_cast<size_t>(argc)};
  if (args.size() != 2) {
    fmt::print(stderr, "Usage: {} <map_file.json>\n", args.front());
    return 1;
  }

  const auto json_text = Utils::readFile(args[1]);
  if (json_text.empty()) {
    fmt::print(stderr, "Error: Unable to read map from file\n");
    return 2;
  }

  const auto maybe_tilemap = tilemap::fromJson(json_text);
  if (!maybe_tilemap) {
    fmt::print(stderr, "Error: Unable to parse JSON tilemap\n");
    return 3;
  }

  const auto& [info, grid] = *maybe_tilemap;
  animate(info, grid);
}
