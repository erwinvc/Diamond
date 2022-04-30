#pragma once
#include "stdincl.h"
#include "assets/asset.h"

namespace Diamond {
	class Mesh;
	//static bool MeshSort(Mesh* mesh1, Mesh* mesh2) { return (mesh1->GetMaterial()->GetID() < mesh2->GetMaterial()->GetID()); }

	class Model : public Asset {
	private:
		std::vector<Mesh*> m_meshes;
		std::string m_name;

	public:
		Model(const std::string& name, std::vector<Mesh*> meshes) : Asset(name), m_meshes(meshes) {
			//sort(m_meshes.begin(), m_meshes.end(), MeshSort);
		}

		Model(const std::string& name, Mesh* mesh) : Asset(name), m_meshes({ mesh }) {}
		Model() : Asset("Unnamed Model") {}
		~Model() {}

		inline std::vector<Mesh*>& getMeshes() { return m_meshes; }

		//void SetMaterial(Material* material);
	};
}