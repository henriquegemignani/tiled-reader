#include "map.h"

#include "exceptions.h"
#include "stdiofileloader.h"

#include <libjson.h>

namespace tiled {

Map::Map(const JSONNode& json_node, const FileLoader& fileloader)
	//: width_(json_node["width"].)
{
	width_ = json_node["width"].as_int();
	height_ = json_node["height"].as_int();
	tile_width_ = json_node["tilewidth"].as_int();
	tile_height_ = json_node["tileheight"].as_int();
	if (json_node["orientation"].as_string() != "orthogonal")
		throw tiled::BaseException("Map orientation not orthogonal.");
	if (json_node["renderorder"].as_string() != "right-down")
		throw tiled::BaseException("Map renderorder not right-down.");

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