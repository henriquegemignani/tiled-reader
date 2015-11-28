#ifndef TILED_TILESET_H_
#define TILED_TILESET_H_

#include <tiled-reader/fileloader.h>
#include <tiled-reader/layer.h>
#include <string>
#include <memory>
#include <unordered_map>

class JSONNode;

namespace tiled {

class Tileset;

struct TileInfo {
    const Tileset* tileset;
	int tile_width, tile_height;
	double p1_u, p1_v;
	double p2_u, p2_v;
};

class Tileset {
public:
	Tileset(const JSONNode& json);
	static std::unique_ptr<Tileset> ReadFromFile(const std::string&, const FileLoader& loader);

	TileInfo tileinfo_for(const TileIndex& tile, int first_gid) const;
	int columns() const;
	int rows() const;
    const std::string& asset_name() const { return image_; }

private:
	std::string image_;
	int tile_width_;
	int tile_height_;
	int image_width_;
	int image_height_;
	int margin_;
	int spacing_;
	std::unordered_map<std::string, std::string> properties_;
};

} // namespace tiled

#endif // TILED_TILESET_H_
