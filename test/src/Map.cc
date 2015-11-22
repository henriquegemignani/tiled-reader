#include "gtest/gtest.h"
#include "map.h"

TEST(Map, ReadFromFile) {
	auto map = tiled::Map::ReadFromFile("data/sample1.json");
}
