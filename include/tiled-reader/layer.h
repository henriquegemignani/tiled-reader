#ifndef TILED_LAYER_H_
#define TILED_LAYER_H_

#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <cstdint>

class JSONNode;

namespace tiled {

class Map;

struct TileIndex {
	TileIndex(uint32_t data);

	uint32_t gid:29;
	bool flipped_horizontally:1;
	bool flipped_vertically:1;
	bool flipped_diagonally:1;
};

class Layer {
public:
	Layer(const JSONNode& json);
	const std::string& name() const { return name_; }

	int width() const { return width_; }
	int height() const { return height_; }
	TileIndex tile_at(int col, int row) const;

private:
	int width_;
	int height_;
	int offset_x_;
	int offset_y_;
	std::string name_;
	std::vector<TileIndex> data_;
	std::vector<void*> objects_;
	std::unordered_map<std::string, std::string> properties_;
};

} // namespace tiled

#endif // TILED_LAYER_H_
