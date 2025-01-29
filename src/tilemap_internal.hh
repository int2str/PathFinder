#ifndef TILEMAP_INTERNAL_HH
#define TILEMAP_INTERNAL_HH

#include <fmt/core.h>

#include <vector>

#include "json/json.hh"
#include "utils/coordinate.hh"

namespace tilemap::internal {

//
// coordinateFrom<>() returns a Utils::Coordinate<> by parsing the integer
// digits of the floating point number as the X coordinate, while the factional
// part is parsed to be the Y coordinate.
//
// WARNING(AE) - Since coordinates are parsed from a floating point number,
// fractional components with trailing zeros are not correctly passed. This is a
// bug inherent in the RiskyLab tile editor.
//
template <typename NUMBER>
  requires std::is_floating_point_v<NUMBER>
[[nodiscard]] constexpr auto coordinateFrom(NUMBER number)
    -> Utils::Coordinate {
  if (number <= 0) return {};
  // NOTE(AE) - An itermediate string representation is used here to avoid
  // floating point erros when attempting to use interger math/modulus
  // operations on the fractional value.
  const auto xy_string = fmt::format("{}", number);
  const auto period_at = xy_string.find('.');
  const auto x         = std::atoi(xy_string.substr(0, period_at).data());
  const auto y         = period_at == std::string::npos
                             ? 0
                             : std::atoi(xy_string.substr(period_at + 1).data());
  return {.x = x, .y = y};
}

//
// coordinatesFromJsonData() return a vector of coordinates from a given json
// array of numbers.
//
template <typename T>
[[nodiscard]] constexpr auto coordinatesFromJsonData(
    const json::ValueIterator<T>& data) -> std::vector<Utils::Coordinate> {
  auto coordinates = std::vector<Utils::Coordinate>{};
  for (const auto& value : data) {
    const auto float_value = value.number().value_or(.0);
    coordinates.emplace_back(coordinateFrom(float_value));
  }
  return coordinates;
}

}  // namespace tilemap::internal

#endif  // TILEMAP_INTERNAL_HH
