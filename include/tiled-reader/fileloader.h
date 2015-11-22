#ifndef TILED_FILELOADER_H_
#define TILED_FILELOADER_H_

#include <string>
#include <memory>

namespace tiled {

class FileLoader {
public:
	struct File {
		virtual ~File() {}
	};

	virtual std::shared_ptr<File> OpenFile(const std::string& path) const = 0;
	virtual std::string GetContents(const std::shared_ptr<File>& file) const = 0;
};

} // namespace tiled

#endif // TILED_FILELOADER_H_
