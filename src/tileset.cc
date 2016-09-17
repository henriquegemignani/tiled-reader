#include <tiled-reader/tileset.h>

#include <tiled-reader/exceptions.h>
#include <libjson.h>

namespace tiled {

Tileset::Tileset(const JSONNode& json_node) 
    : id_(-1)
{
	image_ = json_node["image"].as_string();
	tile_width_ = json_node["tilewidth"].as_int();
	tile_height_ = json_node["tileheight"].as_int();
	tile_count_ = json_node["tilecount"].as_int();
	image_width_ = json_node["imagewidth"].as_int();
	image_height_ = json_node["imageheight"].as_int();
	margin_ = json_node["margin"].as_int();
	spacing_ = json_node["spacing"].as_int();

	auto tiles_properties = json_node.find("tileproperties");
	auto tiles_properties_types = json_node.find("tilepropertytypes");
	if (tiles_properties != json_node.end()) {
		if (tiles_properties_types == json_node.end()) {
			throw tiled::BaseException("Tileset has tileproperties but not tilepropertytypes.");
		}
		for (const auto& tile_properties : *tiles_properties) {
			const auto& properties_types = tiles_properties_types->at(tile_properties.name());
			int tile_index = std::stoi(tile_properties.name());
			
			auto& properties = tile_properties_[tile_index];
			for(const auto& property_value : tile_properties) {
				properties.emplace(std::piecewise_construct, // ("foo": Property)
					std::make_tuple(property_value.name()),
					std::make_tuple(property_value, properties_types[property_value.name()]));
			}
		}
	}

	try {
		
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

TileInfo Tileset::tileinfo_for(const TileIndex& tile_index, int first_gid) const
{
	int tile = tile_index.gid - first_gid;
	int num_cols = columns();
	int col = tile % num_cols;
	int row = tile / num_cols;
	
	float left_u = float(margin_ + col * (tile_width_ + spacing_)) / image_width_;
	float top_v = float(margin_ + row * (tile_height_ + spacing_)) / image_height_;
	float right_u = left_u + float(tile_width_) / image_width_;
	float bot_v = top_v + float(tile_height_) / image_height_;

	if (tile_index.flipped_horizontally) {
		std::swap(left_u, right_u);
	}

	if (tile_index.flipped_vertically) {
		std::swap(top_v, bot_v);
	}

	TileInfo result = {
		this,
		tile_width_,
		tile_height_,
		{ left_u, top_v },
		{ right_u, top_v },
		{ left_u, bot_v },
		{ right_u, bot_v },
	};

	if (tile_index.flipped_diagonally) {
		std::swap(result.top_left, result.bot_right);
	}

	return result;
}

int Tileset::columns() const
{
	return (image_width_ - margin_ + spacing_) / (tile_width_ + spacing_);
}

int Tileset::rows() const
{
	return (image_height_ - margin_ + spacing_) / (tile_height_ + spacing_);
}

} // namespace tiled