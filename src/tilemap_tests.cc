#include "json/json.hh"
#include "src/tilemap.hh"
#include "testrunner/testrunner.h"
#include "tilemap.hh"
#include "tilemap_internal.hh"
#include "utils/coordinate.hh"

namespace {

constexpr auto TWO_BY_ONE_TEST_MAP = R"(
{
  "layers": [{
    "tileset": "MapEditor Tileset_woodland.png",
    "data": [ 8.4, 3 ]
  }],
  "tilesets": [{
    "tilewidth": 32,
    "tileheight": 32
  }],
  "canvas": { "width": 64, "height": 32 }
}
)";

constexpr auto FIVE_BY_FIVE_TEST_MAP = R"(
{
  "layers": [{
      "name": "world",
      "tileset": "MapEditor Tileset_woodland.png",
      "data": [
        8.4, 3, -1, -1, -1, -1, 3, -1, -1, -1, -1, 3, -1,
        3, -1, -1, -1, -1, 3, -1, -1, -1, -1, 3, 0.6]
  }],
  "tilesets": [{
      "name": "MapEditor Tileset_woodland.png",
      "image": "MapEditor Tileset_woodland.png",
      "imagewidth": 512,
      "imageheight": 512,
      "tilewidth": 32,
      "tileheight": 32
  }],
  "canvas": { "width": 160, "height": 160 }
}
)";

}  // namespace

TEST(Tileset_Can_deduce_number_types) {
  const auto from_float = tilemap::internal::coordinateFrom(1.2F);
  EXPECT_EQ(from_float, (Utils::Coordinate{.x = 1, .y = 2}));

  const auto from_double = tilemap::internal::coordinateFrom(2.1);
  EXPECT_EQ(from_double, (Utils::Coordinate{.x = 2, .y = 1}));
}

TEST(Tileset_Can_resolve_coordinates) {
  const auto empty_coordinate = tilemap::internal::coordinateFrom(0.F);
  EXPECT_EQ(empty_coordinate, Utils::Coordinate{});

  const auto two_point_one = tilemap::internal::coordinateFrom(2.1F);
  EXPECT_EQ(two_point_one, (Utils::Coordinate{.x = 2, .y = 1}));

  const auto ten_point_three = tilemap::internal::coordinateFrom(10.3F);
  EXPECT_EQ(ten_point_three, (Utils::Coordinate{.x = 10, .y = 3}));
}

TEST(Tileset_Can_handle_high_floating_point_precision) {
  const auto high_precision = tilemap::internal::coordinateFrom(1.23456F);
  EXPECT_EQ(high_precision, (Utils::Coordinate{.x = 1, .y = 23456}));
}

TEST(Tileset_Implements_RiskyLab_number_bug) {
  // NOTE(AE) - See description for coordinateFrom() for an explanation...
  const auto two_point_one         = tilemap::internal::coordinateFrom(2.1F);
  const auto two_point_onethousand = tilemap::internal::coordinateFrom(2.1000F);
  EXPECT_EQ(two_point_one, two_point_onethousand);  // ;)
}

TEST(Tileset_Can_parse_JSON_tileset_map_data) {
  const auto expected = std::vector<Utils::Coordinate>{
      {.x = 0, .y = 0}, {.x = 1, .y = 2}, {.x = 2, .y = 1}, {.x = 3, .y = 0}};
  const auto json = json::Json::parse(R"({"data": [0, 1.2, 2.1, 3]})");
  ASSERT_TRUE(json);

  const auto coordinates =
      tilemap::internal::coordinatesFromJsonData((*json)["data"]);
  EXPECT_EQ(coordinates.size(), expected.size());
  EXPECT_EQ(coordinates, expected);
}

TEST(Tileset_Can_parse_minimal_asymetrical_map) {
  const auto& maybe_grid = tilemap::gridFromJson(TWO_BY_ONE_TEST_MAP);
  ASSERT_TRUE(maybe_grid);

  const auto& [info, grid] = *maybe_grid;
  EXPECT_EQ(info.canvas_size.x, 64);
  EXPECT_EQ(info.canvas_size.y, 32);
  EXPECT_EQ(info.tile_size.x, 32);
  EXPECT_EQ(info.tile_size.y, 32);
  EXPECT_EQ(info.texture_filename, "MapEditor Tileset_woodland.png");

  EXPECT_EQ(grid.width(), 2);
  EXPECT_EQ(grid.height(), 1);
}

TEST(Tileset_Can_parse_5x5_map) {
  const auto& maybe_grid = tilemap::gridFromJson(FIVE_BY_FIVE_TEST_MAP);
  ASSERT_TRUE(maybe_grid);

  const auto& [info, grid] = *maybe_grid;
  EXPECT_EQ(info.canvas_size.x, 160);
  EXPECT_EQ(info.canvas_size.y, 160);
  EXPECT_EQ(info.tile_size.x, 32);
  EXPECT_EQ(info.tile_size.y, 32);
  EXPECT_EQ(info.texture_filename, "MapEditor Tileset_woodland.png");

  EXPECT_EQ(grid.width(), 5);
  EXPECT_EQ(grid.height(), 5);
}
