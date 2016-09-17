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

TileInfo Tileset::tileinfo_for(const TileIndex& tile_index, int first_gid) const
{
	int tile = tile_index.gid - first_gid;
	int num_cols = columns();
	int col = tile % num_cols;
	int row = tile / num_cols;

	TileInfo result;
	result.tileset = this;
	result.tile_width = tile_width_;
	result.tile_height = tile_height_;
	result.p1_u = double(margin_ + col * (tile_width_ + spacing_)) / image_width_;
	result.p1_v = double(margin_ + row * (tile_height_ + spacing_)) / image_height_;
	result.p2_u = result.p1_u + double(tile_width_) / image_width_;
	result.p2_v = result.p1_v + double(tile_height_) / image_height_;

	if (tile_index.flipped_diagonally) {
		std::swap(result.p1_u, result.p1_v);
		std::swap(result.p2_u, result.p2_v);
	}

	if (tile_index.flipped_horizontally) {
		std::swap(result.p1_u, result.p2_u);
	}

	if (tile_index.flipped_vertically) {
		std::swap(result.p1_v, result.p2_v);
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