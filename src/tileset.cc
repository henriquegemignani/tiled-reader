#include <tiled-reader/tileset.h>

#include <libjson.h>

namespace tiled {

Tileset::Tileset(const JSONNode& json_node) {
	image_ = json_node["image"].as_string();
	tile_width_ = json_node["tilewidth"].as_int();
	tile_height_ = json_node["tileheight"].as_int();
	image_width_ = json_node["imagewidth"].as_int();
	image_height_ = json_node["imageheight"].as_int();
	margin_ = json_node["margin"].as_int();
	spacing_ = json_node["spacing"].as_int();

	try {
		for (const auto& property_node : json_node.at("properties")) {
			properties_[property_node.name()] = property_node.as_string();
		}
	}
	catch (std::out_of_range) {}
}

std::unique_ptr<Tileset> Tileset::ReadFromFile(const std::string&, const FileLoader& loader) {
	return nullptr;
}

} // namespace tiled