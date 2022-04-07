#include "stdafx.h"
#include "fileSystem.h"
#include "util/utils.h"

namespace Diamond {
	void FileSystem::saveJsonToFile(const nlohmann::json& jsonOb, const std::string& name) {
		std::string file = formatString("%s.json", name.c_str());
		std::ofstream i(file);
		i << std::setw(4) << jsonOb;
		i.close();
	}

	nlohmann::json FileSystem::loadJsonFromFile(const std::string& name) {
		std::string file = formatString("%s.json", name.c_str());
		std::ifstream i(file);
		if (i.fail()) LOG_ERROR("[~bJson~x] Failed to open json file");
		nlohmann::json jsonOb = nlohmann::json::parse(i);
		i.close();
		return jsonOb;
	}

	void FileSystem::saveStringToFile(const std::string& str, const std::string& name) {
		std::string file = formatString("%s.txt", name.c_str());
		std::ofstream i(file);
		i << str;
		i.close();
	}

	void FileSystem::createFile(const std::string& file) {
		std::ofstream i(file);
		i.close();
	}

	bool FileSystem::createFileIfDoesntExist(const std::string& file) {
		bool exists = doesFileExist(file);
		if (!exists) createFile(file);
		return !exists;
	}

	std::string FileSystem::getShortFilename(const std::string& filename) {
		const char* lastSlash = strrchr(filename.c_str(), '/');
		if (lastSlash == nullptr) {
			lastSlash = strrchr(filename.c_str(), '\\');
		}
		std::string shortFilename = lastSlash != nullptr ? lastSlash + 1 : filename;
		return shortFilename;
	}

	void FileSystem::copyFile(const std::string& source, const std::string& dest) {
		if (doesFileExist(source)) {
			std::ifstream src(source.c_str(), std::ios::binary);
			std::ofstream dst(dest.c_str(), std::ios::binary);
			dst << src.rdbuf();
			dst.close();
		}
	}
}