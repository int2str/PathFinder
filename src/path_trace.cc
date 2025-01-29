#include <fmt/core.h>
#include <fmt/ranges.h>

#include <span>

#include "src/path_finder.hh"
#include "src/tilemap.hh"
#include "utils/coordinate_formatter.hh"  // IWYU pragma: keep - Used by fmt::print
#include "utils/read_file.hh"

namespace {

void printAsJson(
    const std::unordered_map<Utils::Coordinate, std::vector<Utils::Coordinate>>&
        paths) {
  auto first = true;
  fmt::print("[\n");
  for (const auto& [from, path] : paths) {
    if (!first) fmt::print(",\n");
    first = false;
    fmt::print("  {{\n");
    fmt::print(R"(    "unit": "{}",{})", from, '\n');
    fmt::print(R"(    "path": ["{}"]{})", fmt::join(path, R"(",")"), '\n');
    fmt::print("  }}\n");
  }
  fmt::print("]\n");
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
  const auto& unit_paths   = path_finder::unitPaths(grid);
  if (unit_paths.empty()) {
    fmt::print(stderr,
               "Error: No units detected or no unit can reach its target\n");
    return 4;
  }

  printAsJson(unit_paths);
}
