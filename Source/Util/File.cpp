#include "File.h"
#include "Math.h"

namespace util {

	b8 FileExists(const std::string& path) {
		return std::filesystem::exists(path);
	}

	Result<b8, std::string> IsDirectory(const std::string& path) {
		if (std::filesystem::exists(path)) {
			return Ok(std::filesystem::is_directory(path));
		}
		else {
			return Err("Directory " + path + " does not exist!");
		}
	}
	
	void CreateFile(const std::string& filename) {
		std::ofstream(filename).flush();
	}

	void CreateDirectory(const std::string& path) {
		std::filesystem::create_directory(path);
	}

	Result<std::vector<std::string>, std::string> ListFiles(const std::string& directory) {
		if (std::filesystem::exists(directory)) {
			if (std::filesystem::is_directory(directory)) {
				std::vector<std::string> files;

				try {
					for (const auto& file : std::filesystem::directory_iterator(directory)) {
						files.push_back(file.path().generic_string());
					}
				}
				catch (const std::exception& e) {
					return Err(e.what());
				}

				return Ok(files);
			}
			else {
				return Err(directory + " is not a directory!");
			}
		}
		else {
			return Err("Directory " + directory + " does not exist!");
		}
	}

	Result<std::string, std::string> ReadFile(const std::string& file) {
		std::ifstream handle(file, std::ios::in | std::ios::ate);
		if (!handle.good()) {
			return Err("File " + file + " does not exist");
		}

		std::stringstream buffer;
		buffer << handle.rdbuf();

		if (buffer.fail()) {
			return Err("Failed to read file " + file);
		}

		return Ok(buffer.str());
	}

	Result<void, std::string> WriteFile(const std::string& file, const std::string& data) {
		std::ofstream handle(file, std::ios::out);
		if (!handle.good()) {
			return Err("File " + file + " does not exist");
		}

		handle.write(data.data(), data.size());

		if (handle.fail()) {
			return Err("Failed to write file " + file);
		}

		return Ok();
	}

	Result<std::vector<u8>, std::string> ReadFileBinary(const std::string& file) {
		std::ifstream handle(file, std::ios::in | std::ios::ate | std::ios::binary);
		if (!handle.good()) {
			return Err("Failed to open file " + file);
		}

		std::vector<u8> buffer(handle.tellg());
		handle.seekg(0);
		handle.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		return Ok(std::move(buffer));
	}

	Result<void, std::string> WriteFileBinary(const std::string& file, const std::vector<u8>& data) {
		std::ofstream handle(file, std::ios::out | std::ios::binary);
		if (!handle.good()) {
			return Err("File " + file + " does not exist");
		}

		handle.write(reinterpret_cast<const char*>(data.data()), data.size());

		if (handle.fail()) {
			return Err("Failed to write file " + file);
		}

		return Ok();
	}

	Result<std::tuple<std::string, std::string, std::string>, std::string>
		SplitFile(const std::string& filepath) {
		if (!FileExists(filepath)) {
			return Err("File " + filepath + " does not exist!");
		}

		usize endOfDir = math::Max(
			filepath.find_last_of("/"),
			math::Max(
				filepath.find_last_of("\\"),
				filepath.find_last_of("\\\\")));

		std::string dir = filepath.substr(0, endOfDir);

		usize endOfFile = filepath.find_last_of('.');
		std::string file = filepath.substr(endOfDir, endOfFile - endOfDir);

		usize endOfPath = filepath.size();
		std::string extension = filepath.substr(endOfFile, endOfPath);

		return Ok(std::make_tuple(dir, file, extension));
	}
}