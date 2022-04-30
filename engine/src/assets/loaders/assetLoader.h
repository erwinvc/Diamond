#pragma once
#include "stdincl.h"

class AssetLoader {
protected:
	std::string m_name;
public:
	AssetLoader(const std::string& name) : m_name(name) {}
	virtual ~AssetLoader() {}

	const std::string& getName() { return m_name; }

	void load() {
		asyncLoad();
		syncLoad();
	}

	//Execute part of loading that can be done async
	virtual void asyncLoad() = 0;
	
	//Execute part of loading that has to be done sync. Async loading must be complete before this can be called
	virtual void syncLoad() = 0;

	virtual float getProgress() = 0;;
};

