#include "gtest/gtest.h"
#include <tiled-reader/map.h>

TEST(Map, ReadFromFile) {
	auto map = tiled::Map::ReadFromFile("data/sample1.json");
	EXPECT_EQ(20, map->height());
	EXPECT_EQ(20, map->width());
	EXPECT_EQ(32, map->tile_height());
	EXPECT_EQ(32, map->tile_width());
}
