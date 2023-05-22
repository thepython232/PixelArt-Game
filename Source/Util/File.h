#pragma once

#include "Types.h"
#include "Std.h"
#include "Result.h"

#include <filesystem>
#undef CreateFile
#undef CreateDirectory

namespace util {

	b8 FileExists(const std::string& path);
	Result<b8, std::string> IsDirectory(const std::string& path);

	void CreateFile(const std::string& filename);
	void CreateDirectory(const std::string& path);

	Result<std::vector<std::string>, std::string> ListFiles(const std::string& directory);

	Result<std::string, std::string> ReadFile(const std::string& file);
	Result<void, std::string> WriteFile(const std::string& file, const std::string& data);

	Result<std::vector<u8>, std::string> ReadFileBinary(const std::string& file);
	Result<void, std::string> WriteFileBinary(const std::string& file, const std::vector<u8>& data);

	Result<std::tuple<std::string, std::string, std::string>, std::string>
		SplitFile(const std::string& filepath);
}