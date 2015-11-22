#include "tileset.h"

namespace tiled {

Tileset::Tileset(const JSONNode& json) {
	throw "NYI";
}

std::unique_ptr<Tileset> Tileset::ReadFromFile(const std::string&, const FileLoader& loader) {
	return nullptr;
}

} // namespace tiled