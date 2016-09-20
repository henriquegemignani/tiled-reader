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

	std::string decompress(const std::string& compressed_data) {
		throw tiled::BaseException("compression is NYI");
    }

	std::string identity(const std::string& compressed_data) {
		return compressed_data;
	}
}

Layer::Layer(const JSONNode& json_node) {
	width_ = json_node["width"].as_int();
	height_ = json_node["height"].as_int();
	offset_x_ = json_node["x"].as_int();
	offset_y_ = json_node["y"].as_int();

    
    try {
		type_ = supported_types.at(json_node["type"].as_string().c_str());
    }
    catch (std::out_of_range) {
        throw tiled::BaseException("Unsupported map orientation.");
    }

	auto is_base64 = false;
	auto encoding = json_node.find("encoding");
	if (encoding != json_node.end())
		is_base64 = encoding->as_string().compare("base64") == 0;

	auto is_compressed = false;
	auto decompression_function = is_compressed ? decompress : identity;

	auto data_node = json_node.find("data");
	if (data_node != json_node.end()) {
		auto num_tiles = width_ * height_;
		data_.reserve(num_tiles);
		if (is_base64) {
			auto binary_data_as_string = decompression_function(data_node->as_binary());
			auto binary_data = reinterpret_cast<const uint8_t*>(binary_data_as_string.data());

			for (std::size_t tile_index = 0; tile_index < binary_data_as_string.size(); tile_index += 4) {
				uint32_t id = binary_data[tile_index] |
							  binary_data[tile_index + 1] << 8 |
							  binary_data[tile_index + 2] << 16 |
							  binary_data[tile_index + 3] << 24;
				data_.emplace_back(id);
			}

		} else {
			for (const auto& index : *data_node) {
				data_.emplace_back(static_cast<uint32_t>(index));
			}
		}
	}
}

TileIndex Layer::tile_at(int col, int row) const {
	return data_[row * width_ + col];
}

TileIndex::TileIndex(uint32_t data)
	: gid(data & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG))
	, flipped_horizontally(data & FLIPPED_HORIZONTALLY_FLAG)
	, flipped_vertically(data & FLIPPED_VERTICALLY_FLAG)
	, flipped_diagonally(data & FLIPPED_DIAGONALLY_FLAG)
{
}

} // namespace tiled