#ifndef TILED_STDIOFILELOADER_H_
#define TILED_STDIOFILELOADER_H_

#include <tiled-reader/fileloader.h>
#include <tiled-reader/exceptions.h>

#include <memory>
#include <cstdio>

namespace tiled {

class StdioFileLoader : public FileLoader {
public:
	struct StdioFile : public File {
		explicit StdioFile(FILE* file) : file_(file) {}
		~StdioFile() {
			std::fclose(file_);
		}

		FILE* file_;
	};

	std::shared_ptr<File> OpenFile(const std::string& path) const override {
		if (auto f = std::fopen(path.c_str(), "rb")) {
			return std::make_shared<StdioFile>(f);
		}
		else {
			return nullptr;
		}
	}

	std::string GetContents(const std::shared_ptr<File>& file) const override {
		if (auto stdio_file = dynamic_cast<const StdioFile*>(file.get())) {
			std::string contents;
			std::fseek(stdio_file->file_, 0, SEEK_END);
			contents.resize(std::ftell(stdio_file->file_));
			std::fseek(stdio_file->file_, 0, SEEK_SET);
			std::fread(&contents[0], sizeof(char), contents.size(), stdio_file->file_);
			return contents;
		}
		else {
			throw tiled::BaseException("Invalid file");
		}
	}
};

} // namespace tiled

#endif // TILED_STDIOFILELOADER_H_
