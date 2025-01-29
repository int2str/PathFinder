#include "src/path_finder.hh"
#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"

namespace {

constexpr auto THREE_BY_ONE_TEST_MAP =
    R"({"layers":[{"tileset":"MapEditor Tileset_woodland.png",
"data":[8.4,-1,0.6]}],"tilesets":[{"tilewidth":32,"tileheight":32}],
"canvas":{"width":96,"height":32}})";

constexpr auto FIVE_BY_FIVE_TEST_MAP =
    R"({"layers":[{"tileset":"MapEditor Tileset_woodland.png",
"data":[8.4,3,-1,-1,-1,-1,3,-1,-1,-1,-1,3,-1,3,-1,-1,-1,
-1,3,-1,-1,-1,-1,3,0.6]}],"tilesets":[{"tilewidth":32,"tileheight":32}],
"canvas":{"width":160,"height":160}})";

constexpr auto TWO_UNITS_TEST_MAP =
    R"({"layers":[{"name":"world","tileset":"MapEditor Tileset_woodland.png",
"data":[8.4,-1,0.6,-1,-1,8.4]}],"tilesets":[{"tilewidth":32,"tileheight":32}],
"canvas":{"width":192,"height":32}})";

}  // namespace

TEST(PathFinder_Finds_single_unobstructed_path) {
  const auto& maybe_map = tilemap::fromJson(THREE_BY_ONE_TEST_MAP);
  ASSERT_TRUE(maybe_map);

  const auto& [info, grid] = *maybe_map;
  const auto& unit_paths   = path_finder::unitPaths(grid);
  ASSERT_FALSE(unit_paths.empty());
  EXPECT_EQ(unit_paths.size(), 1);

  const auto expected_path = std::vector<Utils::Coordinate>{
      {.x = 0, .y = 0}, {.x = 1, .y = 0}, {.x = 2, .y = 0}};
  ASSERT_TRUE(unit_paths.contains({}));
  EXPECT_EQ(unit_paths.at({}).size(), expected_path.size());
  EXPECT_EQ(unit_paths.at({}), expected_path);
}

TEST(PathFinder_Navigates_simple_map) {
  const auto& maybe_map = tilemap::fromJson(FIVE_BY_FIVE_TEST_MAP);
  ASSERT_TRUE(maybe_map);

  const auto& [info, grid] = *maybe_map;
  const auto& unit_paths   = path_finder::unitPaths(grid);
  ASSERT_FALSE(unit_paths.empty());
  EXPECT_EQ(unit_paths.size(), 1);

  const auto unit          = Utils::Coordinate{};
  const auto expected_path = std::vector<Utils::Coordinate>{
      {.x = 0, .y = 0}, {.x = 0, .y = 1}, {.x = 0, .y = 2}, {.x = 0, .y = 3},
      {.x = 1, .y = 3}, {.x = 2, .y = 3}, {.x = 2, .y = 2}, {.x = 2, .y = 1},
      {.x = 3, .y = 1}, {.x = 4, .y = 1}, {.x = 4, .y = 2}, {.x = 4, .y = 3},
      {.x = 4, .y = 4}};
  ASSERT_TRUE(unit_paths.contains(unit));
  EXPECT_EQ(unit_paths.at(unit).size(), expected_path.size());
  EXPECT_EQ(unit_paths.at(unit), expected_path);
}

TEST(PathFinder_Finds_paths_for_multiple_units) {
  const auto& maybe_map = tilemap::fromJson(TWO_UNITS_TEST_MAP);
  ASSERT_TRUE(maybe_map);

  const auto& [info, grid] = *maybe_map;
  const auto& unit_paths   = path_finder::unitPaths(grid);
  ASSERT_FALSE(unit_paths.empty());
  EXPECT_EQ(unit_paths.size(), 2);

  const auto first_unit          = Utils::Coordinate{};
  const auto first_expected_path = std::vector<Utils::Coordinate>{
      {.x = 0, .y = 0}, {.x = 1, .y = 0}, {.x = 2, .y = 0}};
  ASSERT_TRUE(unit_paths.contains(first_unit));
  EXPECT_EQ(unit_paths.at(first_unit).size(), first_expected_path.size());
  EXPECT_EQ(unit_paths.at(first_unit), first_expected_path);

  const auto second_unit          = Utils::Coordinate{.x = 5, .y = 0};
  const auto second_expected_path = std::vector<Utils::Coordinate>{
      {.x = 5, .y = 0}, {.x = 4, .y = 0}, {.x = 3, .y = 0}, {.x = 2, .y = 0}};
  ASSERT_TRUE(unit_paths.contains(second_unit));
  EXPECT_EQ(unit_paths.at(second_unit).size(), second_expected_path.size());
  EXPECT_EQ(unit_paths.at(second_unit), second_expected_path);
}
