#include <tiled-reader/tileset.h>

#include <tiled-reader/exceptions.h>
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

std::unique_ptr<Tileset> Tileset::ReadFromFile(const std::string& filepath, const FileLoader& loader) {
	auto json_file = loader.OpenFile(filepath);
	if (!json_file)
		throw tiled::BaseException("File not found: %s\n", filepath.c_str());

	auto contents = loader.GetContents(json_file);
	if (!libjson::is_valid(contents))
		throw tiled::BaseException("Invalid json: %s\n", filepath.c_str());

	auto json_root = libjson::parse(contents);
	return std::make_unique<Tileset>(json_root);
}

} // namespace tiled