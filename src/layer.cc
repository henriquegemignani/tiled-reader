#include <tiled-reader/layer.h>

#include <tiled-reader/exceptions.h>
#include <libjson.h>
#include <zlib.h>
#include <memory>


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
	auto compression = json_node.find("compression");
	if (compression != json_node.end())
		is_compressed = compression->as_string().compare("zlib") == 0;

	auto data_node = json_node.find("data");
	if (data_node != json_node.end()) {
		auto num_tiles = width_ * height_;
		data_.reserve(num_tiles);
		if (is_base64) {
			auto num_bytes = num_tiles * 4;
			auto data_node_binary = data_node->as_binary();
			const uint8_t* binary_data;
			std::unique_ptr<uint8_t> uncompressed_data;

			if (is_compressed) {
				//out = (unsigned *)malloc(outlen);
				unsigned long final_unzip_bytes;
				uncompressed_data.reset(new uint8_t[num_bytes]);
				uncompress(
					uncompressed_data.get(), &final_unzip_bytes,
					reinterpret_cast<const uint8_t*>(data_node_binary.c_str()),
					data_node_binary.size());
				binary_data = uncompressed_data.get();

			} else {
				binary_data = reinterpret_cast<const uint8_t*>(data_node_binary.data());
			}

			for (std::size_t tile_index = 0; tile_index < num_bytes; tile_index += 4) {
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