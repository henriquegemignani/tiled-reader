#ifndef TILED_LAYER_H_
#define TILED_LAYER_H_

#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

class JSONNode;

namespace tiled {

class Map;

class Layer {
public:
	Layer(const JSONNode& json);
	const std::string& name() const { return name_; }
	int tile_at(int col, int row) const;

private:
	int width_;
	int height_;
	int offset_x_;
	int offset_y_;
	std::string name_;
	std::vector<int> data_;
	std::vector<void*> objects_;
	std::unordered_map<std::string, std::string> properties_;
};

} // namespace tiled

#endif // TILED_LAYER_H_
