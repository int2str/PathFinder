#ifndef TILEMAP_WOODLAND_HH
#define TILEMAP_WOODLAND_HH

#include "src/tilemap.hh"
#include "utils/coordinate.hh"

namespace tilemap::woodland {

// These tile definitions based on the "MapEditor Tileset_woodlang.png" are used
// to map colored units to their respective targets. For sake of simplicity
// these are hard-coded for now. This could be extended in the future to be
// configurable from a file/other source of data.

// WARNING(AE) - Do not use "yellow" unit/target pairs...
// The RiskyLab.com tilemap editor contains a bug where coordinates are exported
// as floating point numbers. The yellow creature falls on coordinate x=8, y=10,
// which the editor exports as 8.1, thus loosing the trailing 0 value on the y
// coordinate.

constexpr auto TARGET_RED    = Utils::Coordinate{.x = 0, .y = 5};
constexpr auto UNIT_RED      = Utils::Coordinate{.x = 8, .y = 1};
constexpr auto TARGET_BLUE   = Utils::Coordinate{.x = 0, .y = 6};
constexpr auto UNIT_BLUE     = Utils::Coordinate{.x = 8, .y = 4};
constexpr auto TARGET_GREEN  = Utils::Coordinate{.x = 0, .y = 7};
constexpr auto UNIT_GREEN    = Utils::Coordinate{.x = 8, .y = 7};
constexpr auto TARGET_PURPLE = Utils::Coordinate{.x = 0, .y = 9};
constexpr auto UNIT_PURPLE   = Utils::Coordinate{.x = 8, .y = 13};

constexpr auto FORREST = Utils::Coordinate{.x = 3, .y = 0};
constexpr auto GRASS   = Utils::Coordinate{.x = 1, .y = 0};

constexpr auto UNIT_TARGETS = std::array<RouteMapping, 4>{{
    {.unit_tile = UNIT_RED, .target_tile = TARGET_RED},
    {.unit_tile = UNIT_BLUE, .target_tile = TARGET_BLUE},
    {.unit_tile = UNIT_GREEN, .target_tile = TARGET_GREEN},
    {.unit_tile = UNIT_PURPLE, .target_tile = TARGET_PURPLE},
}};

}  // namespace tilemap::woodland

#endif  // TILEMAP_WOODLAND_HH
