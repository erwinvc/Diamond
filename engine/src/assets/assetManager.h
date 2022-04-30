#pragma once
#include "stdincl.h"
#include "map"
#include "src/util/singleton.h"

namespace Diamond {
	class Asset;

	class AssetManager : public Singleton<AssetManager> {
	public:
		std::map<std::string, Asset*> m_assets;

		void addAsset(const std::string& name, Asset* asset) {
			m_assets.emplace(name, asset);
		}

		template<typename T>
		T* getAsset(const std::string& name) {
			return (T*)m_assets.at(name);
		}
	};

	inline AssetManager* getAssetManager() { return AssetManager::getInstance(); }
}