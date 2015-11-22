#include <tiled-reader/fileloader.h>

namespace tiled {

std::string FileLoader::GetDirnameOfPath(const std::string & path) const {
	size_t found = path.find_last_of("/\\");
	return path.substr(0, found);
}

} // namespace tiled