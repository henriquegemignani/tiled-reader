#ifndef TILED_MAP_H_
#define TILED_MAP_H_

#include <tiled-reader/layer.h>
#include <tiled-reader/tileset.h>
#include <tiled-reader/fileloader.h>

#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

class JSONNode;

namespace tiled {

class Map {
public:
	enum class Orientation {
		ORTHOGONAL
	};
	enum class RenderOrder {
		RIGHT_DOWN
	};

	Map(const std::string& filepath, const JSONNode& json, const FileLoader&);
	static std::unique_ptr<Map> ReadFromFile(const std::string&);
	static std::unique_ptr<Map> ReadFromFile(const std::string&, const FileLoader& loader);

	int width() const { return width_; }
	int height() const { return height_; }
	int tile_width() const { return tile_width_; }
	int tile_height() const { return tile_height_; }
	const std::string& filepath() const { return filepath_; }

	const std::vector<Layer>& layers() const {
		return layers_;
	}

    const Tileset* tileset(int index) const {
        return std::get<0>(tilesets_[index]).get();
    }
    std::size_t tileset_count() const {
        return tilesets_.size();
    }

	TileInfo tileinfo_for(const TileIndex& tile) const;

private:
	int width_;
	int height_;
	int tile_width_;
	int tile_height_;
	Orientation orientation_;
	RenderOrder render_order_;
	std::string filepath_;
	std::vector<Layer> layers_;
	std::vector<std::tuple<std::unique_ptr<Tileset>, int>> tilesets_;
	std::unordered_map<std::string, std::string> properties_;
};

} // namespace tiled

#endif // TILED_MAP_H_
