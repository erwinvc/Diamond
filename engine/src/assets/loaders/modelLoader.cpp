#include "stdafx.h"
#include "modelLoader.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "src/util/timer.h"
#include "assimp/Importer.hpp"
#include "assets/mesh.h"
#include "src/graphics/vertexArray.h"
#include "src/graphics/buffers/vertexBuffer.h"
#include "src/graphics/buffers/indexBuffer.h"
#include "src/assets/assetManager.h"
#include "src/assets/model.h"

namespace Diamond {
	ModelLoader::MeshData::~MeshData() {
		delete[] m_vertices;
	}

	void ModelLoader::asyncLoad() {
		if (!m_path.Exists()) {
			LOG_ERROR("[~gModel~x] model ~1%s~x does not exist!", m_path.getFullPath().c_str());
			return;
		}
		Timer timer;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_path.getFullPath().c_str(), m_importFlags);
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LOG_ERROR("[~gModel~x] model ~1%s~x failed to load: ~1%s", m_name.c_str(), importer.GetErrorString());
			return;
		}

		m_meshData.reserve(scene->mNumMeshes);
		m_materialData.reserve(scene->mNumMaterials);
		for (int i = 0; i < (int)scene->mNumMaterials; i++) {
			//Material* mat = GetMaterialManager()->Create(formatChar("%s_%s", m_name.c_str(), scene->mMaterials[i]->GetName().C_Str()));
			//m_materialData.push_back(MaterialData(mat));
		}

		//if (m_loadMaterials && scene->HasMaterials()) LoadMaterials(scene);

		ProcessNode(scene->mRootNode, scene);
		LOG("[~gModel~x] loaded ~1%s~x in %.2fms", m_name.c_str(), timer.get());
	}


	void ModelLoader::syncLoad() {
		//std::vector<Mesh*> meshes;
		//meshes.reserve(m_meshData.size());
		//
		////for (auto& textureData : m_textureData) {
		////	textureData.second.SyncLoad(GetAssetManager());
		////}
		////
		////for (auto& materialData : m_materialData) {
		////	if (materialData.m_roughness != "") materialData.m_material->SetRoughnessIfAvailable(GetAssetManager()->Get<Texture>(materialData.m_roughness));
		////	if (materialData.m_metallic != "") materialData.m_material->SetMetallicIfAvailable(GetAssetManager()->Get<Texture>(materialData.m_metallic));
		////	if (materialData.m_albedo != "") materialData.m_material->SetAlbedoIfAvailable(GetAssetManager()->Get<Texture>(materialData.m_albedo));
		////	if (materialData.m_normal != "") materialData.m_material->SetNormalIfAvailable(GetAssetManager()->Get<Texture>(materialData.m_normal));
		////	if (materialData.m_emission != "") materialData.m_material->SetEmissionIfAvailable(GetAssetManager()->Get<Texture>(materialData.m_emission));
		////}
		//
		//for (auto& meshData : m_meshData) {
		//	VertexArray* vaoModel = new VertexArray();
		//	VertexBuffer vertexBuffer = VertexBuffer((float*)meshData.m_vertices, meshData.m_numVertices, m_layout);
		//	vaoModel->addBuffer(std::move(vertexBuffer));
		//	vaoModel->bind();
		//	vaoModel->applyLayouts();
		//
		//	//Mesh* mesh = new Mesh(meshData.m_name, vaoModel, new IndexBuffer(meshData.m_indices.data(), (uint32_t)meshData.m_indices.size()), m_materialData[meshData.m_materialIndex].m_material);
		//	Mesh* mesh = new Mesh(meshData.m_name, vaoModel, new IndexBuffer(meshData.m_indices.data(), (uint32_t)meshData.m_indices.size()), nullptr);
		//
		//	float sizeX = glm::abs(meshData.m_posMax.x - meshData.m_posMin.x) / 2;
		//	float sizeY = glm::abs(meshData.m_posMax.y - meshData.m_posMin.y) / 2;
		//	float sizeZ = glm::abs(meshData.m_posMax.z - meshData.m_posMin.z) / 2;
		//	//mesh->m_size = glm::vec3(sizeX, sizeY, sizeZ);
		//
		//	float posX = (meshData.m_posMin.x + meshData.m_posMax.x) / 2.0f;
		//	float posY = (meshData.m_posMin.y + meshData.m_posMax.y) / 2.0f;
		//	float posZ = (meshData.m_posMin.z + meshData.m_posMax.z) / 2.0f;
		//	//mesh->m_center = glm::vec3(posX, posY, posZ);
		//	meshes.push_back(mesh);
		//}
		//
		//getAssetManager()->addAsset(m_name, new Model(m_name, meshes));
		//manager->AddAsset<Model>(m_name, NEW(Model(meshes)));
	}

	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene) {
		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshData.emplace_back(m_name + " | " + mesh->mName.C_Str(), mesh, scene);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	float ModelLoader::getProgress() {
		return 0;
	}
}