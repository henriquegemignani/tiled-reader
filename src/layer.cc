#include "layer.h"

#include <libjson.h>

namespace tiled {

Layer::Layer(const JSONNode& json_node) {
	width_ = json_node["width"].as_int();
	height_ = json_node["height"].as_int();
	offset_x_ = json_node["x"].as_int();
	offset_y_ = json_node["y"].as_int();

	auto data_node = json_node["data"];
	data_.reserve(data_node.size());
	for (const auto& index : data_node)
		data_.emplace_back(index.as_int());

	try {
		for (const auto& property_node : json_node.at("properties")) {
			properties_[property_node.name()] = property_node.as_string();
		}
	} catch(std::out_of_range) {}
}

int Layer::tile_at(int col, int row) const {
	return data_[row * width_ + col];
}

} // namespace tiled