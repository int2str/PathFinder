#include "testrunner/testrunner.h"
#include "tilemap.hh"

TEST(Tileset_CanBeTested) {
  auto info = tilemap::Info{};
  ASSERT_TRUE(info.texture_filename.empty());
}
