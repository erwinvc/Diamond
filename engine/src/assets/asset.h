#pragma once
#include "stdincl.h"

namespace Diamond {
	class Asset {
	protected:
		std::string m_name;

	public:
		Asset(const std::string& name) : m_name(name) {}
		inline const std::string& getName() { return m_name; }
		virtual ~Asset() {}
	};
}