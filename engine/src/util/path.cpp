#include "stdafx.h"
#include "path.h"
#include "utils.h"
#include "fileSystem.h"

namespace Diamond {
	Path::Path(const std::string& fullPath) {
		std::string _fullPath = fullPath;
		for (auto it = _fullPath.find('\\'); it != std::string::npos; it = _fullPath.find('\\')) _fullPath.replace(it, 1, "/");
		auto periodIndex = _fullPath.find_last_of('.');
		if (periodIndex == std::string::npos) m_extention = "";
		else m_extention = _fullPath.substr(periodIndex + 1, _fullPath.size());
		auto dashIndex = _fullPath.find_last_of('/') + 1;
		m_fileName = _fullPath.substr(dashIndex, periodIndex + 1 - dashIndex - 1);
		m_dir = _fullPath.substr(0, dashIndex - 1);
	}

	std::string Path::getFullPath() const {
		return formatString("%s/%s.%s", m_dir.c_str(), m_fileName.c_str(), m_extention.c_str());
	}

	std::string Path::getFullPathWithoutExtention() const {
		return formatString("%s/%s", m_dir.c_str(), m_fileName.c_str());
	}

	bool Path::Exists() const {
		return FileSystem::doesFileExist(getFullPath());
	}

}