#include "map.h"

#include "exceptions.h"
#include "stdiofileloader.h"

#include <libjson.h>

namespace tiled {

	namespace {
		std::unordered_map<std::string, Map::Orientation> supported_orientations = {
			{ "orthogonal", Map::Orientation::ORTHOGONAL },
		};
		std::unordered_map<std::string, Map::RenderOrder> supported_render_orders = {
			{ "right-down", Map::RenderOrder::RIGHT_DOWN },
		};
	}

Map::Map(const JSONNode& json_node, const FileLoader& fileloader)
	//: width_(json_node["width"].)
{
	width_ = json_node["width"].as_int();
	height_ = json_node["height"].as_int();
	tile_width_ = json_node["tilewidth"].as_int();
	tile_height_ = json_node["tileheight"].as_int();

	try {
		orientation_ = supported_orientations.at(json_node["orientation"].as_string());
	}
	catch (std::out_of_range) {
		throw tiled::BaseException("Unsupported map orientation.");
	}

	try {
		render_order_ = supported_render_orders.at(json_node["orientation"].as_string());
	}
	catch (std::out_of_range) {
		throw tiled::BaseException("Unsupported map renderorder.");
	}

	{
		JSONNode layers_node = json_node["layers"];
		layers_.reserve(layers_node.size());
		for (const auto& layer_config : layers_node)
			layers_.emplace_back(*this, layer_config);
	}

	{
		JSONNode tilesets_node = json_node["tilesets"];
		tilesets_.reserve(tilesets_node.size());
		/*for (const auto& tilesets_config : tilesets_node)
			tilesets_.emplace_back(tilesets_config);*/
	}

	//json_node["properties"]
}

std::unique_ptr<Map> Map::ReadFromFile(const std::string& filepath) {
	return ReadFromFile(filepath, tiled::StdioFileLoader());
}

std::unique_ptr<Map> Map::ReadFromFile(const std::string& filepath, const FileLoader& loader) {
	auto json_file = loader.OpenFile(filepath);
	if (!json_file)
		throw tiled::BaseException("File not found: %s\n", filepath.c_str());

	auto contents = loader.GetContents(json_file);
	if (!libjson::is_valid(contents))
		throw tiled::BaseException("Invalid json: %s\n", filepath.c_str());

	auto json_root = libjson::parse(contents);
	return std::make_unique<Map>(json_root, loader);
}

} // namespace shared