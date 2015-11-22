#ifndef TILED_TILESET_H_
#define TILED_TILESET_H_

#include <tiled-reader/fileloader.h>
#include <string>
#include <memory>
#include <unordered_map>

class JSONNode;

namespace tiled {

class Tileset {
public:
	Tileset(const JSONNode& json);
	static std::unique_ptr<Tileset> ReadFromFile(const std::string&, const FileLoader& loader);

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
