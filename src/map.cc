#include <tiled-reader/map.h>

#include <tiled-reader/exceptions.h>
#include <tiled-reader/stdiofileloader.h>

#include <libjson.h>
#include <algorithm>

namespace tiled {

	namespace {
		std::unordered_map<std::string, Map::Orientation> supported_orientations = {
			{ "orthogonal", Map::Orientation::ORTHOGONAL },
		};
		std::unordered_map<std::string, Map::RenderOrder> supported_render_orders = {
			{ "right-down", Map::RenderOrder::RIGHT_DOWN },
		};
	}

Map::Map(const std::string& filepath, const JSONNode& json_node, const FileLoader& fileloader)
	: filepath_(filepath)
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
		render_order_ = supported_render_orders.at(json_node["renderorder"].as_string());
	}
	catch (std::out_of_range) {
		throw tiled::BaseException("Unsupported map renderorder.");
	}

	{
		const auto& layers_node = json_node["layers"];
		layers_.reserve(layers_node.size());
		for (const auto& layer_config : layers_node)
			layers_.emplace_back(layer_config);
	}

	{
		const auto& tilesets_node = json_node["tilesets"];
		tilesets_.reserve(tilesets_node.size());
		for (const auto& tileset_config : tilesets_node) {
			std::string source_path;
			try {
				source_path = tileset_config.at("source").as_string();
			}
			catch (std::out_of_range) {}

			if (source_path.empty()) {
				tilesets_.emplace_back(std::make_unique<Tileset>(tileset_config), tileset_config["firstgid"].as_int());
			}
			else {
				std::string fullpath = fileloader.GetDirnameOfPath(filepath_) + "/" + source_path;
				tilesets_.emplace_back(Tileset::ReadFromFile(fullpath, fileloader),
					tileset_config["firstgid"].as_int());                
			}	
			std::get<0>(tilesets_.back())->id_ = static_cast<int>(tilesets_.size()) - 1;
		}	
	}

	try {
		for (const auto& property_node : json_node.at("properties")) {
			properties_[property_node.name()] = property_node.as_string();
		}
	}
	catch (std::out_of_range) {}
}

TileInfo Map::tileinfo_for(const TileIndex& tile) const {
	int gid = static_cast<int>(tile.gid);
	for (const auto& pair : tilesets_) {
		const auto& tileset = std::get<0>(pair);
		int tileset_firstgid = std::get<1>(pair);
		if (tileset_firstgid <= gid && gid < tileset_firstgid + tileset->tile_count()) {
			return tileset->tileinfo_for(tile, tileset_firstgid);
		}
	}
    throw BaseException("Unknown tile.");
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
	return std::make_unique<Map>(filepath, json_root, loader);
}

} // namespace tiled