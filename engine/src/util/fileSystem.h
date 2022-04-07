#pragma once
#include "json.h"

namespace Diamond {
	class FileSystem {
	public:
		static std::string readFile(const std::string& path) {
			std::ifstream stream(path);
			std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
			stream.close();
			return str;
		}

		static bool doesFileExist(const std::string& path) {
			struct stat buffer;
			return (stat(path.c_str(), &buffer) == 0);
		}

		static void saveJsonToFile(const nlohmann::json& jsonOb, const std::string& name);

		static nlohmann::json loadJsonFromFile(const std::string& name);

		static void saveStringToFile(const std::string& str, const std::string& name);
		static void createFile(const std::string& file);

		static bool createFileIfDoesntExist(const std::string& file);

		static std::string getShortFilename(const std::string& filename);
		static void copyFile(const std::string& source, const std::string& dest);
	};
}