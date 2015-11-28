#include <tiled-reader/layer.h>

#include <tiled-reader/exceptions.h>
#include <libjson.h>

namespace tiled {

namespace {
	const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
	const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;

    namespace {
        std::unordered_map<std::string, Layer::Type> supported_types = {
            { "tilelayer", Layer::Type::TileLayer },
            { "objectgroup", Layer::Type::ObjectGroup },
            { "imagelayer", Layer::Type::ImageLayer },
        };
    }
}

Layer::Layer(const JSONNode& json_node) {
	width_ = json_node["width"].as_int();
	height_ = json_node["height"].as_int();
	offset_x_ = json_node["x"].as_int();
	offset_y_ = json_node["y"].as_int();

    
    try {
            type_ = supported_types.at(json_node["type"].as_string());
    }
    catch (std::out_of_range) {
        throw tiled::BaseException("Unsupported map orientation.");
    }

    try {
        auto data_node = json_node.at("data");
        data_.reserve(data_node.size());
        for (const auto& index : data_node) {
            data_.emplace_back(static_cast<uint64_t>(index));
        }
    }
    catch (std::out_of_range) {}
		

	try {
		for (const auto& property_node : json_node.at("properties")) {
			properties_[property_node.name()] = property_node.as_string();
		}
	} catch(std::out_of_range) {}
}

TileIndex Layer::tile_at(int col, int row) const {
	return data_[row * width_ + col];
}

TileIndex::TileIndex(uint64_t data)
	: flipped_horizontally(data & FLIPPED_HORIZONTALLY_FLAG)
	, flipped_vertically(data & FLIPPED_VERTICALLY_FLAG)
	, flipped_diagonally(data & FLIPPED_DIAGONALLY_FLAG)
	, gid(data & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG))
{
}

} // namespace tiled