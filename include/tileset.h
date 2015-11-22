#ifndef TILED_TILESET_H_
#define TILED_TILESET_H_

#include "fileloader.h"
#include <string>
#include <memory>

class JSONNode;

namespace tiled {

class Tileset {
public:
	Tileset(const JSONNode& json);
	static std::unique_ptr<Tileset> ReadFromFile(const std::string&, const FileLoader& loader);
};

} // namespace tiled

#endif // TILED_TILESET_H_
