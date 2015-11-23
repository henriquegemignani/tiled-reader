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
		JSONNode layers_node = json_node["layers"];
		layers_.reserve(layers_node.size());
		for (const auto& layer_config : layers_node)
			layers_.emplace_back(layer_config);
	}

	{
		JSONNode tilesets_node = json_node["tilesets"];
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
		}	
	}

	for (const auto& property_node : json_node["properties"]) {
		properties_[property_node.name()] = property_node.as_string();
	}
}

TileInfo Map::tileinfo_for(const TileIndex& tile) const {
	using TS_Pair = std::tuple<std::unique_ptr<Tileset>, int>;
	auto greater = std::upper_bound(tilesets_.begin(), tilesets_.end(),
		TS_Pair(nullptr, tile.gid),
		[](const TS_Pair& l, const TS_Pair& r) {
			return std::get<1>(l) < std::get<1>(r);
	});
	if (greater != tilesets_.begin()) {
		const TS_Pair& target_pair = *std::prev(greater);
		return std::get<0>(target_pair)->tileinfo_for(tile, std::get<1>(target_pair));
	}
	else {
		throw new BaseException("Unknown tile.");
	}
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