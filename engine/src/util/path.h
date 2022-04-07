#pragma once

namespace Diamond {
	class Path {
	private:
		std::string m_dir;
		std::string m_fileName;
		std::string m_extention;

	public:
		Path() : m_dir(""), m_fileName(""), m_extention("") {}
		Path(const std::string& dir, const std::string& fileName, const std::string& extention) : m_dir(dir), m_fileName(fileName), m_extention(extention) {}
		Path(const std::string& fullPath);
		Path(const char* fullPath) : Path(std::string(fullPath)) {}

		std::string getFullPath() const;
		std::string getFullPathWithoutExtention() const;
		inline std::string getDirectory() const { return m_dir; }
		inline std::string getFileName() const { return m_fileName; }
		inline std::string getExtention() const { return m_extention; }
		bool Exists() const;
	};
}