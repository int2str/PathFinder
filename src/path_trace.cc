#include <fmt/core.h>

#include <span>

auto main(int argc, char* argv[]) -> int {
  const auto args = std::span{argv, static_cast<size_t>(argc)};
  if (args.size() != 2) {
    fmt::print(stderr, "Usage: {} <map_file.json>\n", args.front());
    return 1;
  }
}
