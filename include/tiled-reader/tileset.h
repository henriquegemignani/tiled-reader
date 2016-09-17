#ifndef TILED_TILESET_H_
#define TILED_TILESET_H_

#include <tiled-reader/fileloader.h>
#include <tiled-reader/layer.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <_internal/Source/JSON_Base64.h>

class JSONNode;

namespace tiled {

class Tileset;

struct UV {
	double u;
	double v;
};

struct TileInfo {
    const Tileset* tileset;
	int tile_width, tile_height;
	UV top_left, top_right, bot_left, bot_right;
};

class Tileset {
public:
	Tileset(const JSONNode& json);
	static std::unique_ptr<Tileset> ReadFromFile(const std::string&, const FileLoader& loader);

	TileInfo tileinfo_for(const TileIndex& tile, int first_gid) const;
	int columns() const;
	int rows() const;
	int tile_count() const { return tile_count_; }
    const std::string& asset_name() const { return image_; }
    int id() const { return id_; }

private:
    int id_;
	std::string image_;
	int tile_width_;
	int tile_height_;
	int tile_count_;
	int image_width_;
	int image_height_;
	int margin_;
	int spacing_;
	std::unordered_map<std::string, std::string> properties_;
	friend class Map;
};

} // namespace tiled

#endif // TILED_TILESET_H_
